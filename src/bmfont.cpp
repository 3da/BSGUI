#include "bsgui/bmfont.h"

#include <cstdio>
#include <cstring>
#include <cassert>

#include "bsgui/imageopengl.h"

#ifdef _MSC_VER
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif // _MSC_VER

namespace BSGUI
{

using std::memset;
using std::strncpy;
using std::fopen;
using std::fscanf;

static char sGetEscapedChar(char successor)
{
    switch (successor)    // todo: wider support for different escapes
    {
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    }
    return successor; // unknown
}


BMFont::FontData::FontData() : pages(0), kernings(0), chars(0)
{
}

BMFont::FontData::~FontData()
{
    Reset();
}

void BMFont::FontData::Reset()
{
    delete[] pages;
    delete[] kernings;
    delete[] chars;
    pages = 0;
    kernings = 0;
    chars = 0;
}

BMFont::Page::Page() : fontImage(0)
{
}

BMFont::Page::~Page()
{
    delete fontImage;
}


BMFont::BMFont(const char *descriptionFile) :
    mSpacing(0)
{
    memset(mFontData.info.face, 0, MAX_FIELD_LENGTH);
    memset(mFontData.info.charset, 0, MAX_FIELD_LENGTH);
    memset(mFileName, 0, MAX_FILENAME_LENGTH);
    strncpy(mFileName, descriptionFile, MAX_FILENAME_LENGTH - 1);
}

BMFont::~BMFont()
{
}


bool BMFont::Init()
{
    return Reload();
}


bool BMFont::Reload()
{
    long a;

    mLoaded = false;

    mFontData.Reset();

    FILE *fp = fopen(mFileName, "r");

    if (fp == 0)
        return false;

    /* Note: fails in following cases:
     * - if charset includes whitespace
     * - if face name is longer than max field length-1
     * - if charset is longer than max field length-2
     */
    // keep the %NUM as (MAX_FIELD_LENGTH-1)
    long size, stretchH, padding[4], spacing[2];
    long outline, aa, bold, italic, unicode, smooth;
    if (fscanf(fp, "info face=\"%63[^\"]\" size=%d bold=%d italic=%d charset=\"%63s unicode=%d stretchH=%d smooth=%d aa=%d padding=%d,%d,%d,%d spacing=%d,%d outline=%d%*[\n\r]",
               mFontData.info.face,
               &size,
               &bold,
               &italic,
               mFontData.info.charset,
               &unicode,
               &stretchH,
               &smooth,
               &aa,
               &padding[0],
               &padding[1],
               &padding[2],
               &padding[3],
               &spacing[0],
               &spacing[1],
               &outline) != 16)
    {
        fclose(fp);
        return false;
    }
    mFontData.info.size = (unsigned int)size;
    mFontData.info.bold = bold ? true : false;
    mFontData.info.italic = italic ? true : false;
    mFontData.info.unicode = unicode ? true : false;
    mFontData.info.stretchH = (int)stretchH;
    mFontData.info.smooth = smooth ? true : false;
    mFontData.info.aa = (char)aa;
    mFontData.info.padding[PADDING_UP] = (int)padding[0];
    mFontData.info.padding[PADDING_RIGHT] = (int)padding[1];
    mFontData.info.padding[PADDING_DOWN] = (int)padding[2];
    mFontData.info.padding[PADDING_LEFT] = (int)padding[3];
    mFontData.info.horizSpacing = (int)spacing[0];
    mFontData.info.vertSpacing = (int)spacing[1];
    mFontData.info.outline = (int)outline;

    // Charset is handled differently as it can be zero size string
    mFontData.info.charset[strlen(mFontData.info.charset) - 1] = 0; // delete ending quote

    long lineHeight, base, scaleW, scaleH, pages, packed;
    long alphaChnl, redChnl, greenChnl, blueChnl;
    if (fscanf(fp, "common lineHeight=%d base=%d scaleW=%d scaleH=%d pages=%d packed=%d alphaChnl=%d redChnl=%d greenChnl=%d blueChnl=%d%*[\n\r]",
               &lineHeight,
               &base,
               &scaleW,
               &scaleH,
               &pages,
               &packed,
               &alphaChnl,
               &redChnl,
               &greenChnl,
               &blueChnl) != 10)
    {
        fclose(fp);
        return false;
    }

    mFontData.common.lineHeight = (unsigned int)lineHeight;
    mFontData.common.base = (int)base;
    mFontData.common.scaleW = (int)scaleW;
    mFontData.common.scaleH = (int)scaleH;
    mFontData.common.pages = (unsigned int)pages;
    mFontData.common.packed = packed ? true : false;
    mFontData.common.alphaChnl = (unsigned char)alphaChnl;
    mFontData.common.redChnl = (unsigned char)redChnl;
    mFontData.common.greenChnl = (unsigned char)greenChnl;
    mFontData.common.blueChnl = (unsigned char)blueChnl;

    mFontData.pages = new Page[mFontData.common.pages];
    if (!mFontData.pages)
    {
        fclose(fp);
        return false;
    }

    for (a = 0; a < mFontData.common.pages; ++a)
    {
        long id;
        // Note: keep the %NUM as (MAX_FILENAME_LENGTH-1)
        if (fscanf(fp, "page id=%d file=\"%127[^\"]\"%*[\n\r]",
                   &id,
                   mFontData.pages[a].file) != 2)
        {
            fclose(fp);
            return false;
        }
        mFontData.pages[a].id = (unsigned int)id;
    }

    if (fscanf(fp, "chars count=%d\n", &mFontData.charCount) != 1)
    {
        fclose(fp);
        return false;
    }

    bool foundSpaceChar = false;
    int spacePos = 0;
    // allocate one extra for the potential explicitly added space
    mFontData.chars = new Char[mFontData.charCount + 1];

    for (a = 0; a < mFontData.charCount; ++a)
    {
        long x, y, width, height, xoffset, yoffset;
        long xadvance, page, chnl;
        if (fscanf(fp, "char%*[ ]id=%d%*[ ]x=%d%*[ ]y=%d%*[ ]width=%d%*[ ]height=%d%*[ ]xoffset=%d%*[ ]yoffset=%d%*[ ]xadvance=%d%*[ ]page=%d%*[ ]chnl=%d%*[\n\r]",
                   &mFontData.chars[a].id,
                   &x,
                   &y,
                   &width,
                   &height,
                   &xoffset,
                   &yoffset,
                   &xadvance,
                   &page,
                   &chnl) != 10)
        {
            fclose(fp);
            return false;
        }
        mFontData.chars[a].x = (unsigned int)x;
        mFontData.chars[a].y = (unsigned int)y;
        mFontData.chars[a].width = (unsigned int)width;
        mFontData.chars[a].height = (unsigned int)height;
        mFontData.chars[a].xoffset = (int)xoffset;
        mFontData.chars[a].yoffset = (int)yoffset;
        mFontData.chars[a].xadvance = (int)xadvance;
        mFontData.chars[a].page = (unsigned int)page;
        mFontData.chars[a].chnl = (unsigned char)chnl;
        mFontData.chars[a].firstKernIndex = -1;

        if (mFontData.chars[a].id == 32)
            foundSpaceChar = true;
        if (mFontData.chars[a].id < 32)
            ++spacePos;
    }

    if (!foundSpaceChar)
    {
        long charsToMove = mFontData.charCount - spacePos;
        if (charsToMove > 0)
            memmove(&mFontData.chars[spacePos + 1],
                    &mFontData.chars[spacePos],
                    charsToMove * sizeof(Char));
        Char &space = mFontData.chars[spacePos];
        space.id = 32;
        space.x = space.y = 0;
        space.width = space.height = 0;
        space.xoffset = space.yoffset = 0;
        // default space width is third of font size
        space.xadvance = mFontData.info.size / 3;
        space.page = 0;
        space.chnl = 0;
        ++mFontData.charCount;
    }

    if (fscanf(fp, "kernings count=%d%*[\n\r]", &mFontData.kerningCount) != 1)
        mFontData.kerningCount = 0;

    if (mFontData.kerningCount > 0)
    {
        mFontData.kernings = new Kerning[mFontData.kerningCount];
        if (!mFontData.kernings)
        {
            fclose(fp);
            return false;
        }

        for (a = 0; a < mFontData.kerningCount; ++a)
        {
            long amount;
            if (fscanf(fp, "kerning first=%d%*[ ]second=%d%*[ ]amount=%d%*[ \n\r]",
                       &mFontData.kernings[a].first,
                       &mFontData.kernings[a].second,
                       &amount) != 3)
            {
                fclose(fp);
                return false;
            }
            mFontData.kernings[a].amount = (int)amount;
            long idx = FindChar(mFontData.kernings[a].first);
            if (idx >= 0 && mFontData.chars[idx].firstKernIndex == -1)
                mFontData.chars[idx].firstKernIndex = a;
        }
    } // kerningCount > 0

    fclose(fp);

    // Try to load images
    for (a = 0; a < mFontData.common.pages; ++a)
    {
        mFontData.pages[a].fontImage = ImageOpenGL::LoadImage(mFontData.pages[0].file);
        if (mFontData.pages[a].fontImage == 0)
        {
            // failed - try to load it relatively at the same path as the description file
            char dir[128];
            char *last = strrchr(mFileName, '/');
            if (last == 0)
                last = strrchr(mFileName, '\\');
            long len = (long)(last - mFileName) + 1;
            if (last != 0 && len < 128)
            {
                strncpy(dir, mFileName, len);
                dir[len] = 0;
                char fullpath[256] = { 0 };
                snprintf(fullpath, 255, "%s%s", dir, mFontData.pages[a].file);
                mFontData.pages[a].fontImage = ImageOpenGL::LoadImage(fullpath);
                if (mFontData.pages[a].fontImage == 0)
                    return false;
            }
        }
        assert( mFontData.pages[a].fontImage );
    }

    mLoaded = true;

    return true;
}


long BMFont::FindChar(unsigned long ch)
{
    // Performs a binary search.
    // Assumes that the chars array is sorted.
    long l = 0;
    long r = mFontData.charCount - 1;
    while (l <= r)
    {
        long m = (l + r) / 2;
        const unsigned long id = mFontData.chars[m].id;
        if (id > ch)
            r = m - 1;
        else if (id < ch)
            l = m + 1;
        else
            return m;
    }
    return -1;
}


long BMFont::FindKern(long first, long second)
{
    return FindKern(first, second, FindChar(first));
}

long BMFont::FindKern(long first, long second, long firstCharIndex)
{
    long a, start = 0;
    if (firstCharIndex >= 0)
    {
        start = mFontData.chars[firstCharIndex].firstKernIndex;
        if (start < 0)
            return 0;
    }
    const long kerningCount = mFontData.kerningCount;
    for (a = start; a < kerningCount; ++a)
    {
        if (mFontData.kernings[a].first == first)
        {
            if (mFontData.kernings[a].second == second)
                return mFontData.kernings[a].amount;
        }
        else
        {
            if (a > 0 && mFontData.kernings[a - 1].first == first)
                break; // reached end of candidates
        }
    }
    return 0;
}


const char * BMFont::GetFontName() const
{
    return mFontData.info.face;
}


long BMFont::GetFontHeight()
{
    return mFontData.common.lineHeight;
}


float BMFont::GetFontHeight(float scale)
{
    return mFontData.common.lineHeight * scale;
}


bool BMFont::HasCharacter(unsigned long character)
{
    return FindChar(character) >= 0;
}


float BMFont::GetStringWidth(const char *string, float scale, unsigned long substrLength)
{
    const long maxLength = substrLength ? substrLength : 0x7fffffff;
    long a, w = 0, last = -1;
    bool escape = false;
    for (a = 0; a < maxLength && string[a]; ++a)
    {
        char currentChar = string[a];
        if (GetFlags() & Font::FF_HANDLE_BACKSLASH_ESCAPES)
        {
            if (escape)
            {
                currentChar = sGetEscapedChar(currentChar);
                escape = false;
            }
            else if (currentChar == '\\')
            {
                escape = true;
                continue;
            }
        }

        unsigned long ch = currentChar & 0xff;
        long id = FindChar(ch);
        if (id == -1)
        {
            last = -1;
            continue;
        }
        if (last != -1)
            w += FindKern(last, ch);
        w += mFontData.chars[id].xadvance + mSpacing;
        last = ch;
    }
    // subtract the spacing after last character from result, scale final result
    return (w - mSpacing) * scale;
}


float BMFont::DrawString(float x, float y, const char *string, unsigned long flags, float scale, unsigned long substrLength)
{
    long a, last = -1, lastIdx = -1, maxLength = substrLength ? (long)substrLength : 0x7fffffff;
    Rectangle rect;
    float xoff = x;
    float yoff = y;
    float stringWidth = 0;

    if ((flags & IF_ALIGN_HORIZ_CENTER) != 0)
        xoff -= GetStringWidth(string, scale, substrLength) / float(2);
    else if ((flags & IF_ALIGN_RIGHT) != 0)
        xoff -= GetStringWidth(string, scale, substrLength);
    if ((flags & IF_ALIGN_VERT_CENTER) != 0)
        yoff -= GetFontHeight(scale) / float(2);
    else if ((flags & IF_ALIGN_BOTTOM) != 0)
        yoff -= GetFontHeight(scale);

    if (scale == float(1))
    {
        x = (float)((long)x);
        y = (float)((long)y);
    }
    for (a = 0; a < mFontData.common.pages; ++a)
    {
        assert(mFontData.pages[a].fontImage);
        mFontData.pages[a].fontImage->SetScale(scale);
    }

    flags = IF_ALIGN_TOP_LEFT | IF_COLOR | IF_ALPHA;
    if (scale != (float)1)
        flags |= IF_TRANSFORM | IF_SCALE;

    bool escape = false;

    for (a = 0; a < maxLength && string[a]; ++a)
    {
        char currentChar = string[a];
        if (GetFlags() & Font::FF_HANDLE_BACKSLASH_ESCAPES)
        {
            if (escape)
            {
                currentChar = sGetEscapedChar(currentChar);
                escape = false;
            }
            else if (currentChar == '\\')
            {
                escape = true;
                continue;
            }
        }

        unsigned long ch = currentChar & 0xff;
        long id = FindChar(ch);

        if (last != -1)
            xoff += FindKern(last, ch, lastIdx) * scale;
        if (id == -1)
        {
            last = -1;
            continue;
        }
        rect.x = static_cast<float>(mFontData.chars[id].x);
        rect.y = static_cast<float>(mFontData.chars[id].y);
        rect.width = static_cast<float>(mFontData.chars[id].width);
        rect.height = static_cast<float>(mFontData.chars[id].height);

        float x, y;
        float gw = float(mFontData.chars[id].xadvance + mSpacing);
        if (scale != (float)1)
        {
            x = xoff + mFontData.chars[id].xoffset * scale;
            y = yoff + mFontData.chars[id].yoffset * scale;
            gw *= scale;
        }
        else
        {
            x = (float)((long)(xoff + mFontData.chars[id].xoffset));
            y = (float)((long)(yoff + mFontData.chars[id].yoffset));
        }
        stringWidth += gw;
        xoff += gw;
        long page = mFontData.chars[id].page;
        mFontData.pages[page].fontImage->Draw(mDrawMode, flags, x, y, &rect);

        last = ch;
        lastIdx = id;
    } // a < len

    return stringWidth - mSpacing * scale;
}
}

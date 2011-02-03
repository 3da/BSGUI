
#include "bsgui/font.h"

namespace BSGUI
{

Font::StringRowParser::StringRowParser(Font *font, const wchar_t *string, float scale, float wrapAreaWidth, unsigned long substrLength)
{
    Reset(font, string, scale, wrapAreaWidth, substrLength);
}


bool Font::StringRowParser::Reset(Font *font, const wchar_t *string, float scale, float wrapAreaWidth, unsigned long substrLength)
{
    if (string == 0 || string[0] == 0 || font == 0)
    {
        mFont = 0;
        mString = 0;
        mMaxLength = 0;
        mResultRowCount = 0;
        mDone = true;
        return false;
    }
    const int SPACE = 32;
    mFont = font;
    mString = string;
    mScale = scale;
    mWrapAreaWidth = wrapAreaWidth;
    mMaxLength = substrLength ? substrLength : 0x7fffffff;
    mWordWrapping = (wrapAreaWidth > 0);
    mSpacing = font->GetCharacterSpacing() * scale;
    if (font->HasCharacter(SPACE))
    {
    	mSpaceWidth = font->GetStringWidth(L" ", 1);
    }
    else
    	mSpaceWidth = font->GetFontHeight() / 3 * scale;
    mResultWidth = 0;
    mResultRowCount = 1;
    mTotalChars = 0;
    mCurrentRowStart = string;
    mCurrentWordStart = string;
    mCurrentRowLength = 0;
    mCurrentWordLength = 0;
    //mCurrentRowWordCount = 0;
    mCurrentRowWidth = 0;
    mDone = false;
    return true;
}


bool Font::StringRowParser::GetNextRow(const wchar_t *&oRowStart, unsigned long &oRowLength, float *oRowWidth)
{
    if (Finished())
    {
        oRowStart = &mString[mTotalChars];
        oRowLength = 0;
        if (oRowWidth)
            *oRowWidth = 0;
        return false;
    }

    const int SPACE = 32, NEWLINE = '\n', TERMINATOR = 0;
    bool rowDone = false, escape = false;

    long prevTotalChars;
    char currentChar;
    do {
        currentChar = mString[mTotalChars];
        if (mFont->GetFlags() & Font::FF_HANDLE_BACKSLASH_ESCAPES)
        {
            if (escape)
            {
                switch (currentChar)    // todo: wider support for different escapes
                {
                case 'n': currentChar = '\n'; break;
                case 'r': currentChar = '\r'; break;
                case 't': currentChar = '\t'; break;
                }
                escape = false;
            }
            else if (currentChar == '\\')
            {
                escape = true;
                ++mTotalChars;
                continue;
            }
        }

        bool newLineAlreadyHandled = false;
        if (mWordWrapping)
        {
            if (currentChar != SPACE && currentChar != NEWLINE && currentChar != TERMINATOR && mTotalChars < mMaxLength)
                ++mCurrentWordLength;
            else
            {
                // reached newline or space (a word has just ended if prev. char wasn't space)
                bool wrapToNextLine = false, startNewRow = false;
                float newWordWidth = 0;
                if (mCurrentWordLength > 0)
                {
                    // end of new word reached - check if row would extend beyond wrap area
                    newWordWidth = mFont->GetStringWidth(mCurrentWordStart, mScale, mCurrentWordLength);
                    if (mCurrentRowWidth + newWordWidth > mWrapAreaWidth)
                    {
                        if (mCurrentRowWidth > 0)
                        {
                            // word goes beyond wrap area width
                            wrapToNextLine = true;
                            newWordWidth += mSpacing;
                        } else {
                            // let word overflow from wrapped area since it doesn't fit
                            startNewRow = true;
                        }
                    }
                    else
                        mCurrentRowWidth += newWordWidth + mSpacing;
                }
                if (!wrapToNextLine && mCurrentRowWidth + mSpaceWidth >= mWrapAreaWidth && currentChar != TERMINATOR && mTotalChars < mMaxLength)
                    startNewRow = true;
                if (wrapToNextLine || startNewRow)
                {
                    // start new line (wrap current word to beginning of new line)
                    if (oRowWidth)
                        *oRowWidth = mCurrentRowWidth;
                    oRowStart = mCurrentRowStart;
                    // when wrapping, subtract current word length from result row, or when
                    // starting new row proactively (next space wouldn't fit), include current char
                    oRowLength = mCurrentRowLength + (wrapToNextLine ? -mCurrentWordLength : 1);
                    ++mResultRowCount;
                    mCurrentRowStart = wrapToNextLine ? mCurrentWordStart : &mString[mTotalChars + 1];
                    mCurrentRowWidth = wrapToNextLine ? newWordWidth + 1 : 0;
                    mCurrentRowLength = wrapToNextLine ? mCurrentWordLength + 1 : 0;
                    //mCurrentRowWordCount = 0;
                    newLineAlreadyHandled = true; // signal that newline has been handled by word wrapping
                    rowDone = true;
                }
                else if (currentChar == SPACE)
                {
                    // space is added to current line
                    mCurrentRowWidth += mSpaceWidth + mSpacing;
                }
                // start next word from next char (current is space or newline)
                if (currentChar != TERMINATOR)
                    mCurrentWordStart = &mString[mTotalChars + 1];
                //if (!wrapToNextLine)
                mCurrentWordLength = 0;
                //++mCurrentRowWordCount;
            } // current char == SPACE or NEWLINE
        } // wordWrapping

        if (!rowDone && currentChar == NEWLINE)
        {
            if (!newLineAlreadyHandled)
            {
                if (!mWordWrapping && oRowWidth)
                    *oRowWidth = mFont->GetStringWidth(mCurrentRowStart, mScale, mCurrentRowLength);
                oRowStart = mCurrentRowStart;
                oRowLength = mCurrentRowLength;
                ++mResultRowCount;
                mCurrentRowStart = &mString[mTotalChars + 1];
                mCurrentRowWidth = 0;
                mCurrentRowLength = 0;
                //mCurrentRowWordCount = 0;
                newLineAlreadyHandled = true;
                rowDone = true;
            }
        } // current char == NEWLINE

        if (!newLineAlreadyHandled)
            ++mCurrentRowLength;

        prevTotalChars = mTotalChars;
        if (mTotalChars < mMaxLength && currentChar != TERMINATOR)
            ++mTotalChars;
    } while (!rowDone && prevTotalChars < mMaxLength && currentChar != TERMINATOR);

    if (!rowDone && !mDone && (mTotalChars >= mMaxLength || mString[mTotalChars] == TERMINATOR))
    {
        mDone = true;
        if (mCurrentRowLength > 0)
        {
            if (!mWordWrapping && oRowWidth)
                *oRowWidth = mFont->GetStringWidth(mCurrentRowStart, mScale, mCurrentRowLength);
            oRowStart = mCurrentRowStart;
            if (mTotalChars >= mMaxLength)
                --mCurrentRowLength; // subtract last char (already went past asked length)
            oRowLength = mCurrentRowLength;
            rowDone = true;
        }
    }

    return rowDone;
}


long Font::GetStringMetrics(float &oWidth, float &oHeight, const wchar_t *string,
                           float scale, float wrapAreaWidth, unsigned long substrLength)
{
    StringRowParser srp(this, string, scale, wrapAreaWidth, substrLength);
    if (srp.Finished())
        return 0;

    float resultWidth = 0;
    const wchar_t *rowStart;
    unsigned long rowLength;
    float rowWidth;
    while (srp.GetNextRow(rowStart, rowLength, &rowWidth))
    {
        if (resultWidth < rowWidth)
            resultWidth = rowWidth;
    }
    oWidth = resultWidth;
    oHeight = GetFontHeight(scale) * srp.GetResultRowCount();
    return srp.GetResultRowCount();
}


long Font::DrawText(float x, float y, const wchar_t *text,
                   unsigned long flags, float scale, float wrapAreaWidth, unsigned long substrLength)
{
    // align vertically first when needed
    if ((flags & IF_ALIGN_VERT_CENTER) || (flags & IF_ALIGN_BOTTOM))
    {
        float totalWidth = 0, totalHeight = 0;
        GetStringMetrics(totalWidth, totalHeight, text, scale, wrapAreaWidth, substrLength);
        if ((flags & IF_ALIGN_VERT_CENTER) != 0)
            y -= totalHeight / 2;
        else if ((flags & IF_ALIGN_BOTTOM) != 0)
            y -= totalHeight;
        flags &= ~(IF_ALIGN_VERT_CENTER | IF_ALIGN_BOTTOM); // mask away vertical align flags
    }

    StringRowParser srp(this, text, scale, wrapAreaWidth, substrLength);
    if (srp.Finished())
        return 0;

    const wchar_t *rowStart;
    unsigned long rowLength;
    float rowWidth, rowHeight = GetFontHeight(scale);
    while (srp.GetNextRow(rowStart, rowLength, &rowWidth))
    {
        DrawString(x, y, rowStart, flags, scale, rowLength);
        y += rowHeight;
    }
    return srp.GetResultRowCount();
}

}


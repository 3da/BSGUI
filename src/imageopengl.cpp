#include "bsgui/imageopengl.h"

#include <GL\gl.h>
#include <GL\glu.h>
#include <cmath>





namespace BSGUI
{

static inline unsigned long sNextPowerOfTwo(unsigned long input)
{
     --input;
     input |= input >> 16;
     input |= input >> 8;
     input |= input >> 4;
     input |= input >> 2;
     input |= input >> 1;
     return input + 1;
}

ImageOpenGL::ImageOpenGL(unsigned long width, unsigned long height, unsigned long flags,
                         unsigned long glId, const float *uv) :
    Image(width, height, IDM_NORMAL, flags),
    mGLId(glId),
    mHalfWidth((float)width / 2.0f),
    mHalfHeight((float)height / 2.0f),
    mTargetWidth((float)width),
    mTargetHeight((float)height),
    mAngle(0),
    mScaleX(1), mScaleY(1),
    mUsingExplicitTargetSize(false)
{
    unsigned long a;
    for (a = 0; a < 4; ++a)
        mUV[a] = uv[a];
}


ImageOpenGL::~ImageOpenGL()
{
    glDeleteTextures(1, (const GLuint *)&mGLId);
}


void ImageOpenGL::Draw(IMAGE_DRAW_MODE drawMode, unsigned long flags, float x, float y, const Rectangle *sourceRect)
{
    bool usingSrcArea = true;
    Rectangle srcArea;
    if (sourceRect)
        srcArea = *sourceRect;
    else if (!sourceRect && mUsingSourceRectangle)
        srcArea = mSourceRectangle;
    else
    {
        srcArea = Rectangle(0, 0, (float)mWidth, (float)mHeight);
        usingSrcArea = false;
    }

    // other than normal drawMode needs storing of attributes
    const bool storeAttrib = (drawMode != IDM_NORMAL);

	glPushAttrib(GL_COLOR_BUFFER_BIT);

    switch (drawMode)
    {
    case IDM_ADD:
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        break;
    case IDM_MULTIPLY:
        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        break;
    case IDM_PREMULT:
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        break;
    default:;
        // blend func is assumed to be (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) by default
    }

    //glColor4fv(color);

    glBindTexture(GL_TEXTURE_2D, mGLId);

    float uv[4];
    memcpy(uv, mUV, 4 * sizeof(float));
    if (flags & IF_FLIP_HORIZ)
    {
        uv[0] = mUV[2];
        uv[2] = mUV[0];
    }
    if (flags & IF_FLIP_VERT)
    {
        uv[1] = mUV[3];
        uv[3] = mUV[1];
    }
    float halfWidth = mHalfWidth, halfHeight = mHalfHeight;
    if (usingSrcArea)
    {
        float uvW = uv[2] - uv[0];
        float uvH = uv[3] - uv[1];
        uv[0] += uvW * srcArea.x / mWidth;
        uv[1] += uvH * srcArea.y / mHeight;
        uv[2] = uv[0] + uvW * srcArea.width / mWidth;
        uv[3] = uv[1] + uvH * srcArea.height / mHeight;
        halfWidth = srcArea.width * 0.5f;
        halfHeight = srcArea.height * 0.5f;
    }

    if (flags & IF_TRANSFORM)
    {
        float pivotX = -halfWidth, pivotY = -halfHeight; // transform center adjusted to top-left (default)
        if (flags & IF_ALIGN_RIGHT)
            pivotX = halfWidth;
        if (flags & IF_ALIGN_HORIZ_CENTER)
            pivotX = (mUsingCustomPivot ? mPivotX : halfWidth) - halfWidth;
        if (flags & IF_ALIGN_BOTTOM)
            pivotY = halfHeight;
        if (flags & IF_ALIGN_VERT_CENTER)
            pivotY = (mUsingCustomPivot ? mPivotY : halfHeight) - halfHeight;

        float vx1[4], vy1[4], vx2[4], vy2[4];
        float *vx = vx1, *vy = vy1;
        vx[0] = -halfWidth - pivotX; vy[0] = -halfHeight - pivotY;
        vx[1] = halfWidth - pivotX; vy[1] = -halfHeight - pivotY;
        vx[2] = -halfWidth - pivotX; vy[2] = halfHeight - pivotY;
        vx[3] = halfWidth - pivotX; vy[3] = halfHeight - pivotY;
        if (flags & IF_SCALE)
        {
            vx[0] *= mScaleX; vy[0] *= mScaleY;
            vx[1] *= mScaleX; vy[1] *= mScaleY;
            vx[2] *= mScaleX; vy[2] *= mScaleY;
            vx[3] *= mScaleX; vy[3] *= mScaleY;
        }
        if (flags & IF_ROTATE)
        {
            float c = cosf(mAngle), s = sinf(mAngle);
            // rotate current vertices to temp vertices
            vx2[0] = vx[0] * c - vy[0] * s; vy2[0] = vx[0] * s + vy[0] * c;
            vx2[1] = vx[1] * c - vy[1] * s; vy2[1] = vx[1] * s + vy[1] * c;
            vx2[2] = vx[2] * c - vy[2] * s; vy2[2] = vx[2] * s + vy[2] * c;
            vx2[3] = vx[3] * c - vy[3] * s; vy2[3] = vx[3] * s + vy[3] * c;
            // switch temp vertices to final ones
            vx = vx2; vy = vy2;
        }

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(uv[0], uv[1]); glVertex2f(vx[0] + x, vy[0] + y);
        glTexCoord2f(uv[2], uv[1]); glVertex2f(vx[1] + x, vy[1] + y);
        glTexCoord2f(uv[0], uv[3]); glVertex2f(vx[2] + x, vy[2] + y);
        glTexCoord2f(uv[2], uv[3]); glVertex2f(vx[3] + x, vy[3] + y);
        glEnd();
    }
    else
    {
        float targetWidth = mTargetWidth, targetHeight = mTargetHeight;
        if (usingSrcArea && !mUsingExplicitTargetSize)
        {
            targetWidth = srcArea.width;
            targetHeight = srcArea.height;
        }
        float topLeftX = x, topLeftY = y; // top-left (default)
        if (flags & IF_ALIGN_RIGHT)
            topLeftX -= targetWidth;
        if (flags & IF_ALIGN_HORIZ_CENTER)
        {
            const float normalized = ((mUsingCustomPivot ? mPivotX : halfWidth) - halfWidth) / halfWidth; // range -1..1
            const float halfTargetWidth = targetWidth * 0.5f;
            topLeftX -= halfTargetWidth + halfTargetWidth * normalized;
        }
        if (flags & IF_ALIGN_BOTTOM)
            topLeftY -= targetHeight;
        if (flags & IF_ALIGN_VERT_CENTER)
        {
            const float normalized = ((mUsingCustomPivot ? mPivotY : halfHeight) - halfHeight) / halfHeight; // range -1..1
            const float halfTargetHeight = targetHeight * 0.5f;
            topLeftY -= halfTargetHeight + halfTargetHeight * normalized;
        }

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(uv[0], uv[1]); glVertex2f(topLeftX, topLeftY);
        glTexCoord2f(uv[2], uv[1]); glVertex2f(topLeftX + targetWidth, topLeftY);
        glTexCoord2f(uv[0], uv[3]); glVertex2f(topLeftX, topLeftY + targetHeight);
        glTexCoord2f(uv[2], uv[3]); glVertex2f(topLeftX + targetWidth, topLeftY + targetHeight);
        glEnd();
    }

	glPopAttrib();
	glBindTexture(GL_TEXTURE_2D, 0);
}

Image * ImageOpenGL::LoadImage(const char *fileName, unsigned long flags)
{
    SDL_RWops *rw = SDL_RWFromFile(fileName, "rb");
    if (rw == 0)
        return 0;
    return LoadImage(rw, true, flags);
}


Image * ImageOpenGL::LoadImage(SDL_RWops *source, bool deleteSource,
                                     unsigned long flags)
{
    SDL_Surface *surface;

    surface = LoadSurface(source, deleteSource);
    if (surface == 0)
        return 0;

    Image *res = CreateImage(surface, flags);
    SDL_FreeSurface(surface);

    return res;
}


SDL_Surface * ImageOpenGL::LoadSurface(const char *fileName)
{
    SDL_RWops *rw = SDL_RWFromFile(fileName, "rb");
    if (rw == 0)
        return 0;
    return LoadSurface(rw, true);
}


SDL_Surface * ImageOpenGL::LoadSurface(SDL_RWops *source, bool deleteSource)
{
    return IMG_Load_RW(source, deleteSource ? 1 : 0);
}


Image * ImageOpenGL::CreateImage(SDL_Surface *surface, unsigned long flags,
                                       PixelRectangle *sourceRect)
{
//    if (flags == 0xffffffff)
//        flags = mDefaultFlags;

    bool success;
    unsigned long texture;
    float uv[4];

    if (surface == 0)
        return 0;

    success = CreateTexture(texture, uv, surface, flags, sourceRect);
    if (!success)
        return 0;

    if (sourceRect == 0)
        return new ImageOpenGL(surface->w, surface->h, flags, texture, uv);
    else
        return new ImageOpenGL(sourceRect->width, sourceRect->height, flags, texture, uv);
}


static void sSimpleFixAlphaChannel(SDL_Surface *image, const SDL_Rect &srcr)
{
    if (image->w < 2 || image->h < 2)
        return;

    if (SDL_LockSurface(image) != 0)
        return;

    Uint32 *pixels = (Uint32 *)image->pixels;
    long x, y;
    long w = image->w, h = image->h;
    const unsigned long alphaMask = image->format->Amask;
    const unsigned long colorMask =
        image->format->Rmask | image->format->Gmask | image->format->Bmask;

    long co = 0;
    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x)
        {
            if ((pixels[co] & alphaMask) == 0)
            {
                // iterate through 3x3 window around pixel
                long left = x - 1, right = x + 1, top = y - 1, bottom = y + 1;
                if (left < 0) left = 0;
                if (right >= w) right = w - 1;
                if (top < 0) top = 0;
                if (bottom >= h) bottom = h - 1;
                long x2, y2, colors = 0, co2 = top * w + left;
                long red = 0, green = 0, blue = 0;
                for (y2 = top; y2 <= bottom; ++y2)
                {
                    for (x2 = left; x2 <= right; ++x2)
                    {
                        Uint32 px = pixels[co2++];
                        if (px & alphaMask)
                        {
                            red += (px & image->format->Rmask) >> image->format->Rshift;
                            green += (px & image->format->Gmask) >> image->format->Gshift;
                            blue += (px & image->format->Bmask) >> image->format->Bshift;
                            ++colors;
                        }
                    }
                    co2 += w - (right - left + 1);
                }
                if (colors > 0)
                {
                    pixels[co] &= alphaMask;
                    pixels[co] |= (red / colors) << image->format->Rshift;
                    pixels[co] |= (green / colors) << image->format->Gshift;
                    pixels[co] |= (blue / colors) << image->format->Bshift;
                }
            }
            ++co;
        }
    }

    // if rect is smaller than texture, copy rightmost/bottom col/row (and pixel at w,h)
    // as is from the inner one so that linear sampling works like clamp_to_edge
    if (srcr.w < image->w)
    {
        for (y = 0; y < srcr.h; ++y)
            pixels[y * w + srcr.w] = pixels[y * w + srcr.w - 1];
    }
    if (srcr.h < image->h)
    {
        for (x = 0; x < srcr.w; ++x)
            pixels[srcr.h * w + x] = pixels[(srcr.h - 1) * w + x];
    }
    if (srcr.w < image->w && srcr.h < image->h)
        pixels[srcr.h * w + srcr.w] = pixels[(srcr.h - 1) * w + srcr.w - 1];

    /*
    // Test code to see all pixels with full opacity
    for (y = 0; y < h; ++y)
    {
        for (x = 0; x < w; ++x)
        {
            pixels[y * w + x] |= alphaMask;
        }
    }
    */

    SDL_UnlockSurface(image);
}


// Based on SDL examples.
bool ImageOpenGL::CreateTexture(unsigned long &oTexture, float *oUV, SDL_Surface *surface,
                                      unsigned long flags, PixelRectangle *sourceRect)
{
    unsigned long w, h;
    SDL_Surface *image;
    SDL_Rect area;
    SDL_Rect srcr;
    unsigned long savedFlags;
    unsigned char savedAlpha;
    unsigned long srcw, srch;

    if (sourceRect != 0)
    {
        srcw = sourceRect->width;
        srch = sourceRect->height;
    }
    else
    {
        srcw = surface->w;
        srch = surface->h;
    }

    // Use the surface width and height expanded to powers of 2.
    w = sNextPowerOfTwo(srcw);
    h = sNextPowerOfTwo(srch);
    oUV[0] = 0;                  // Min X
    oUV[1] = 0;                  // Min Y
    oUV[2] = ((GLfloat)srcw/* - 0.0001f*/) / w;  // Max X
    oUV[3] = ((GLfloat)srch/* - 0.0001f*/) / h;  // Max Y

    image = SDL_CreateRGBSurface(
        SDL_HWSURFACE | SDL_SRCALPHA,
        w, h, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
        0x000000FF,
        0x0000FF00,
        0x00FF0000,
        0xFF000000
#else
        0xFF000000,
        0x00FF0000,
        0x0000FF00,
        0x000000FF
#endif
        );
    if (image == 0)
        return false;

    // Save the alpha blending attributes.
    savedFlags = surface->flags & (SDL_SRCALPHA | SDL_RLEACCELOK);
    savedAlpha = surface->format->alpha;
    if ((savedFlags & SDL_SRCALPHA) == SDL_SRCALPHA)
        SDL_SetAlpha(surface, 0, 0);

    // Copy the surface into the GL texture image.
    area.x = 0;
    area.y = 0;
    area.w = srcw;
    area.h = srch;
    if (sourceRect != 0)
    {
        srcr.x = sourceRect->x;
        srcr.y = sourceRect->y;
        srcr.w = sourceRect->width;
        srcr.h = sourceRect->height;
    }
    else
        srcr = area;
    SDL_BlitSurface(surface, &srcr, image, &area);

    // TODO: Should this "fix" be optional?
    sSimpleFixAlphaChannel(image, area);

    // Restore the alpha blending attributes.
    if ((savedFlags & SDL_SRCALPHA) == SDL_SRCALPHA)
        SDL_SetAlpha(surface, savedFlags, savedAlpha);

    // Create an OpenGL texture from the image.
    glGenTextures(1, (GLuint *)&oTexture);
    glBindTexture(GL_TEXTURE_2D, oTexture);

    if (flags & IF_FILTER_LINEAR_MIPMAP)
        flags |= IF_USE_MIPMAPS;

    if (flags & IF_FILTER_LINEAR)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        if (flags & IF_USE_MIPMAPS)
        {
            if (flags & IF_FILTER_LINEAR_MIPMAP)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            else
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        }
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        if (flags & IF_USE_MIPMAPS)
        {
            if (flags & IF_FILTER_LINEAR_MIPMAP)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            else
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        }
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    if (flags & IF_WRAP_REPEAT)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    // for some tests - bright green border color
    //GLfloat col[4] = { 0, 1, 0, 1 };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, col);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    if (flags & IF_USE_MIPMAPS)
    {
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    }

    SDL_FreeSurface(image); // No longer needed.

    return true;
}


}

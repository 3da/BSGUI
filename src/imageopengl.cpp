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

ImageOpenGL::ImageOpenGL(unsigned long width, unsigned long height/*, unsigned long flags*/,
                         unsigned long glId, const float *uv) :
    Image(width, height),
    mGLId(glId),
    mHalfWidth((float)width / 2.0f),
    mHalfHeight((float)height / 2.0f),
    mTargetWidth((float)width),
    mTargetHeight((float)height)//,
{
    unsigned long a;
    for (a = 0; a < 4; ++a)
        mUV[a] = uv[a];
}


ImageOpenGL::~ImageOpenGL()
{
    glDeleteTextures(1, (const GLuint *)&mGLId);
}


void ImageOpenGL::Draw(unsigned long flags, float x, float y, float r, float sx, float sy, const Rectangle *sourceRect)
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


	glPushAttrib(GL_COLOR_BUFFER_BIT);

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

	float pivotX = -halfWidth, pivotY = -halfHeight; // transform center adjusted to top-left (default)
    if (flags & IF_ALIGN_RIGHT)
        pivotX = halfWidth;
	if (flags & IF_ALIGN_HORIZ_CENTER)
        pivotX = 0;
    if (flags & IF_ALIGN_BOTTOM)
        pivotY = halfHeight;
    if (flags & IF_ALIGN_VERT_CENTER)
        pivotY = 0;

    float vx1[4], vy1[4], vx2[4], vy2[4];
    float *vx = vx1, *vy = vy1;
    vx[0] = -halfWidth - pivotX; vy[0] = -halfHeight - pivotY;
    vx[1] = halfWidth - pivotX; vy[1] = -halfHeight - pivotY;
    vx[2] = -halfWidth - pivotX; vy[2] = halfHeight - pivotY;
    vx[3] = halfWidth - pivotX; vy[3] = halfHeight - pivotY;
    if (sx != 1.0 || sy != 1.0)
    {
        vx[0] *= sx; vy[0] *= sy;
        vx[1] *= sx; vy[1] *= sy;
        vx[2] *= sx; vy[2] *= sy;
        vx[3] *= sx; vy[3] *= sy;
	}
    if (r)
    {
        float c = cosf(r), s = sinf(r);
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

	glPopAttrib();
	glBindTexture(GL_TEXTURE_2D, 0);
}

Image * ImageOpenGL::LoadImage(const char *fileName)
{
    SDL_RWops *rw = SDL_RWFromFile(fileName, "rb");
    if (rw == 0)
        return 0;
    return LoadImage(rw, true);
}


Image * ImageOpenGL::LoadImage(SDL_RWops *source, bool deleteSource)
{
    SDL_Surface *surface;

    surface = LoadSurface(source, deleteSource);
    if (surface == 0)
        return 0;

    Image *res = CreateImage(surface);
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


Image * ImageOpenGL::CreateImage(SDL_Surface *surface, PixelRectangle *sourceRect)
{
    bool success;
    unsigned long texture;
    float uv[4];

    if (surface == 0)
        return 0;

    success = CreateTexture(texture, uv, surface, sourceRect);
    if (!success)
        return 0;

    if (sourceRect == 0)
        return new ImageOpenGL(surface->w, surface->h, texture, uv);
    else
        return new ImageOpenGL(sourceRect->width, sourceRect->height, texture, uv);
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

    SDL_UnlockSurface(image);
}


// Based on SDL examples.
bool ImageOpenGL::CreateTexture(unsigned long &oTexture, float *oUV, SDL_Surface *surface, PixelRectangle *sourceRect)
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

    sSimpleFixAlphaChannel(image, area);

    // Restore the alpha blending attributes.
    if ((savedFlags & SDL_SRCALPHA) == SDL_SRCALPHA)
        SDL_SetAlpha(surface, savedFlags, savedAlpha);

    // Create an OpenGL texture from the image.
    glGenTextures(1, (GLuint *)&oTexture);
    glBindTexture(GL_TEXTURE_2D, oTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    SDL_FreeSurface(image); // No longer needed.

    return true;
}


}

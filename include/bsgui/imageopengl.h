#ifndef IMAGEOPENGL_H_INCLUDED
#define IMAGEOPENGL_H_INCLUDED

#include "SDL/SDL.h"

#include "image.h"

#include "SDL/SDL_opengl.h"
#include "SDL_image.h"
#undef LoadImage

namespace BSGUI
{
//! Implementation of Turska Image which uses OpenGL.
/*! \note In this implementation the IF_COLOR and IF_ALPHA flags
 *        are always assumed to be enabled.
 */
class ImageOpenGL : public Image
{
public:

	//! Constructs a new ImageOpenGL from given texture and U,V coordinates.
	/*!
	 * \param width original width of the image in pixels.
	 * \param height original height of the image in pixels.
	 * \param flags initial flags to use for the image.
	 * \param glId OpenGL texture name for the texture image,
	 *             ownership is transferred.
	 * \param uv top-left and bottom-right U,V coordinates in
	 *           texture (4 floats).
	 */
	ImageOpenGL(unsigned long width, unsigned long height, unsigned long flags, unsigned long glId, const float *uv);
	//! Destructor deletes the texture.
	virtual ~ImageOpenGL();

	// From Image:

	virtual void SetColor(const Color &color) {
		mColor = color;
	}

	virtual void SetTargetSize(float width, float height) {
		mTargetWidth = width;
		mTargetHeight = height;
		mUsingExplicitTargetSize = true;
	}
	virtual void ResetTargetSize() {
		mTargetWidth = (float)mWidth;
		mTargetHeight = (float)mHeight;
		mUsingExplicitTargetSize = false;
	}

	virtual void SetAngle(float angle) {
		mAngle = angle;
	}

	virtual void SetScale(float scale) {
		mScaleX = mScaleY = scale;
	}
	virtual void SetScale(float scaleX, float scaleY) {
		mScaleX = scaleX;
		mScaleY = scaleY;
	}

	virtual void Draw(float x, float y) {
		Draw(mDrawMode, mFlags, x, y);
	}
	virtual void Draw(IMAGE_DRAW_MODE drawMode, unsigned long flags, float x, float y, const Rectangle *sourceRect = 0);

	// ImageOpenGL additions:

	inline unsigned long GetOpenGLId() const {
		return mGLId;
	}
	inline float *GetUV() {
		return mUV;
	}

	static Image * LoadImage(const char *fileName,
									 unsigned long flags = 0xffffffff);

	// GraphicsSDLOpenGL additions:

	//! Loads and constructs an Image from given SDL_RWops object.
	/*!
	 * \param source data object to load image from.
	 * \param deleteSource true if source should be deleted after loading
	 *                     the image or false otherwise.
	 * \param flags image flags to use when loading image.
	 *              \sa turska::IMAGE_FLAG
	 * \return image object if successful, or 0 if unsuccessful.
	 */
	static Image * LoadImage(SDL_RWops *source, bool deleteSource,
									 unsigned long flags = 0xffffffff);

	//! Loads a SDL_Surface from given file name.
	/*!
	 * \param fileName file name of image to load.
	 * \return surface object if successful, or 0 if unsuccessful.
	 */
	static SDL_Surface * LoadSurface(const char *fileName);

	//! Loads a SDL_Surface from given SDL_RWops object.
	/*!
	 * \param source data object to load image from.
	 * \param deleteSource true if source should be deleted after loading
	 *                     the image or false otherwise.
	 * \return surface object if successful, or 0 if unsuccessful.
	 */
	static SDL_Surface * LoadSurface(SDL_RWops *source, bool deleteSource);

	//! Constructs an Image from given SDL_Surface.
	/*!
	 * \param surface surface to create Image from.
	 * \param flags image flags to use when constructing image.
	 *              \sa turska::IMAGE_FLAG
	 * \param sourceRect optional source rectangle to crop from the surface,
	 *                   or 0 to use the whole surface.
	 */
	static Image * CreateImage(SDL_Surface *surface, unsigned long flags,
									   PixelRectangle *sourceRect = 0);

	//! Creates an OpenGL texture from given SDL_Surface.
	/*!
	 * \param oTexture resulting OpenGL texture name, when successful.
	 * \param oUV resulting top-left and bottom-right U,V coordinates
	 *            in texture (4 floats), when successful.
	 * \param surface surface to create texture from.
	 * \param flags image flags to use when constructing image.
	 *              \sa turska::IMAGE_FLAG
	 * \param sourceRect optional source rectangle to crop from the surface,
	 *                   or 0 to use the whole surface.
	 * \return true if successful, or false if unsuccessful.
	 */
	static bool CreateTexture(unsigned long &oTexture, float *oUV,
									  SDL_Surface *surface, unsigned long flags,
									  PixelRectangle *sourceRect = 0);


protected:

	unsigned long mGLId;
	float mUV[4];
	const float mHalfWidth;
	const float mHalfHeight;
	Color mColor;
	float mTargetWidth;
	float mTargetHeight;
	float mAngle;
	float mScaleX, mScaleY;
	bool mUsingExplicitTargetSize;
}; // ImageOpenGL


}


#endif // IMAGEOPENGL_H_INCLUDED

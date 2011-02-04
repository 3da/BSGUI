#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <GL\gl.h>

namespace BSGUI
{

struct Color
{
	union {
		float c[4];
		struct {
			float r;
			float g;
			float b;
			float a;
		};
	};

	Color()
	{
		r=1;
		g=1;
		b=1;
		a=1;
	}
	Color(float red, float green, float blue, float alpha = (float)1.0f)
	{
		Set(red, green, blue, alpha);
	}




	inline void Set(float red, float green, float blue, float alpha = (float)1.0f)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	inline void Use()
	{
		glColor4fv(c);
	}
};

struct Rectangle
{
	Rectangle(float x = 0, float y = 0, float width = 0, float height = 0) :
		x(x), y(y), width(width), height(height) {}
	//inline Rectangle(const PixelRectangle &pr);
	float x, y;
	float width, height;
	bool isInside(float x, float y) const
	{
		const Rectangle &r = *this;
		return x >= r.x && y >= r.y && x < r.x + r.width && y < r.y + r.height;
	}
};

    //! Defines a rectangle inside pixel image data.
    struct PixelRectangle
    {
        PixelRectangle(int x = 0, int y = 0, int width = 0, int height = 0) :
            x(x), y(y), width(width), height(height) {}
        inline PixelRectangle(const Rectangle &r) :
            x((int)r.x), y((int)r.y), width((int)r.width), height((int)r.height) {}
        int x, y;
        int width, height;
    };


//! Image drawing modes.
enum IMAGE_DRAW_MODE
{
	IDM_NORMAL = 0, //!< Normal image drawing, with alpha if supported.
	IDM_ADD,        //!< Additive image drawing.
	IDM_MULTIPLY,   //!< Multiplicative image drawing.
	IDM_PREMULT,    //!< For premultiplied alpha image.
	//! Count of image drawing modes.
	IMAGE_DRAW_MODE_COUNT // Do not add new draw modes after this one.
};

//! Drawing flags for image features to use when drawing.
/*! Note that implementations are free to ignore selected flags.
 * For example, a simple software rendered implementation might
 * ignore rotation or filtering flags.
 *
 * The vertical alignment flags are mutually exclusive with each
 * other, and similarly horizontal flags are mutually exclusive
 * with each other. If mutually exclusive alignment flags are
 * combined, center flag is given higher priority.
 */
enum IMAGE_FLAG
{
	//! Multiplies source image with color when drawing.
	IF_COLOR                = (1 << 0),
	//! Multiplies source image with alpha when drawing.
	IF_ALPHA                = (1 << 1),

	//! Align horizontally to left edge (default).
	IF_ALIGN_LEFT           = 0,
	//! Align horizontally to pivot (center by default).
	IF_ALIGN_HORIZ_CENTER   = (1 << 2),
	//! Align horizontally to right edge.
	IF_ALIGN_RIGHT          = (1 << 3),
	//! Align vertically to top edge (default).
	IF_ALIGN_TOP            = 0,
	//! Align vertically to pivot (center by default).
	IF_ALIGN_VERT_CENTER    = (1 << 4),
	//! Align vertically to bottom.
	IF_ALIGN_BOTTOM         = (1 << 5),

	// Alignment shortcuts:
	//! Align to left center.
	IF_ALIGN_LEFT_VCENTER   = IF_ALIGN_LEFT | IF_ALIGN_VERT_CENTER,
	//! Align to top-left corner.
	IF_ALIGN_TOP_LEFT       = IF_ALIGN_LEFT | IF_ALIGN_TOP,
	//! Align to top center.
	IF_ALIGN_TOP_HCENTER    = IF_ALIGN_HORIZ_CENTER | IF_ALIGN_TOP,
	//! Align to top-right corner.
	IF_ALIGN_TOP_RIGHT      = IF_ALIGN_RIGHT | IF_ALIGN_TOP,
	//! Align to bottom-left corner.
	IF_ALIGN_BOTTOM_LEFT    = IF_ALIGN_LEFT | IF_ALIGN_BOTTOM,
	//! Align to bottom center.
	IF_ALIGN_BOTTOM_HCENTER = IF_ALIGN_HORIZ_CENTER | IF_ALIGN_BOTTOM,
	//! Align to bottom-right corner.
	IF_ALIGN_BOTTOM_RIGHT   = IF_ALIGN_RIGHT | IF_ALIGN_BOTTOM,
	//! Align to right center.
	IF_ALIGN_RIGHT_VCENTER  = IF_ALIGN_RIGHT | IF_ALIGN_VERT_CENTER,
	//! Center image with given pivot (default is center).
	IF_ALIGN_CENTER         = IF_ALIGN_HORIZ_CENTER | IF_ALIGN_VERT_CENTER,

	//! Flip (mirror) image horizontally.
	IF_FLIP_HORIZ           = (1 << 6),
	//! Flip (mirror) image vertically.
	IF_FLIP_VERT            = (1 << 7),

	/*! Enables transformation. When IF_TRANSFORM is disabled, drawing
	 *  uses target width & height and alignment flags. When IF_TRANSFORM
	 *  is enabled, drawing uses IF_ROTATE and IF_SCALE flags.
	 */
	IF_TRANSFORM            = (1 << 8),
	//! Rotates image when drawing.
	IF_ROTATE               = (1 << 9),
	//! Applies scaling to image. See setScale() methods.
	IF_SCALE                = (1 << 10),

	//! Uses bilinear filtering for image sampling if available.
	/*! Recommended for use when images will be scaled.
	 * Default is to use nearest neighbor sampling.
	 * This flag should be set when loading images.
	 */
	IF_FILTER_LINEAR        = (1 << 16),

	//! Uses linear filtering between mipmaps.
	/*! Recommended for use when images may be scaled very small.
	 */
	IF_FILTER_LINEAR_MIPMAP = (1 << 17),

	//! Uses/generates mipmaps.
	/*! Recommended for use when images may be scaled very small.
	 * \note Using IF_FILTER_LINEAR_MIPMAP without IF_FILTER_LINEAR
	 *       is allowed, but may not be supported by all implementations.
	 */
	IF_USE_MIPMAPS          = (1 << 18),

	//! Uses mipmaps and tri-linear filtering.
	IF_FILTER_TRILINEAR     = IF_FILTER_LINEAR | IF_FILTER_LINEAR_MIPMAP | IF_USE_MIPMAPS,


	//! Configures image wrap mode to clamp to edge.
	/*! On hardware accelerated implementations, wrap mode should
	 * be configured clamp (to edge) by default, even without this
	 * flag. When this flag is present, Graphics implementation
	 * may add custom insets to images, because hardware APIs
	 * sometimes can't be trusted to support correct wrap mode.
	 */
	IF_WRAP_CLAMP_EDGE      = (1 << 19),

	//! Configures image wrap mode to repeat. Default is to clamp.
	/*! This flag should be set when loading images. You can assume
	 *  that repeat wrapping works properly only with power of two
	 *  image sizes, at least for most implementations.
	 */
	IF_WRAP_REPEAT          = (1 << 20),

	//! Default flags for image.
	IF_DEFAULT_FLAGS        = IF_COLOR | IF_ALPHA | IF_ALIGN_TOP_LEFT |
	IF_FILTER_LINEAR | IF_WRAP_CLAMP_EDGE
};


//! Abstract interface for a container of a static UI image.
/*! \note Implementation of this class may not support all
 *        features defined by the interface.
 */
class Image
{
public:

	//! Initializes image with given values.
	/*!
	 * \param width width of the image.
	 * \param height height of the image.
	 * \param drawMode initial drawing mode to use, default is IDM_NORMAL.
	 * \param flags initial flags to use. \sa IF_DEFAULT_FLAGS
	 */
	Image(unsigned long width, unsigned long height,
		  IMAGE_DRAW_MODE drawMode = IDM_NORMAL,
		  unsigned long flags = IF_DEFAULT_FLAGS) :
		mWidth(width), mHeight(height), mFlags(flags),
		mDrawMode(drawMode), mUsingSourceRectangle(false),
		mPivotX(0), mPivotY(0), mUsingCustomPivot(false) // pivot reference point is center of image
	{}

	virtual ~Image() {}


	inline float GetWidth() {
		return mUsingSourceRectangle ? mSourceRectangle.width : (float)mWidth;
	}
	inline float GetHeight() {
		return mUsingSourceRectangle ? mSourceRectangle.height : (float)mHeight;
	}

	inline unsigned long GetOriginalWidth() {
		return mWidth;
	}
	inline unsigned long GetOriginalHeight() {
		return mHeight;
	}

	inline void SetFlags(unsigned long flags) {
		mFlags = flags;
	}
	inline unsigned long GetFlags() const {
		return mFlags;
	}
	inline void SetFlag(unsigned long flag) {
		mFlags |= flag;
	}
	inline void ClearFlag(unsigned long flag) {
		mFlags &= ~flag;
	}
	inline void SetFlagState(unsigned long flag, bool enabled)
	{
		enabled ? SetFlag(flag) : ClearFlag(flag);
	}
	inline bool IsFlag(unsigned long flag) const {
		return (mFlags & flag) == flag;
	}

	//! Sets drawing mode. \sa IMAGE_DRAW_MODE
	inline void SetDrawMode(IMAGE_DRAW_MODE drawMode) {
		mDrawMode = drawMode;
	}

	//! Returns current drawing mode. \sa IMAGE_DRAW_MODE
	inline IMAGE_DRAW_MODE GetDrawMode() {
		return mDrawMode;
	}


	//! Sets the color and alpha to use when IF_COLOR or IF_ALPHA flags are enabled.
	virtual void SetColor(const Color &color) = 0;


	//! Sets target rectangle size to use when IF_TRANSFORM flag is disabled.
	/*! \note Value scale is the display unit size in use, typically pixels.
	 */
	virtual void SetTargetSize(float width, float height) = 0;

	//! Disables previously set explicit target rectangle size.
	virtual void ResetTargetSize() = 0;

	//! Sets rotation angle to use when IF_ROTATE flag is enabled.
	/*! \param angle rotation angle in radians.
	 */
	virtual void SetAngle(float angle) = 0;

	//! Sets the scaling factor to use when IF_SCALE flag is enabled.
	virtual void SetScale(float scale) = 0;

	//! Sets non-uniform scaling factors to use when IF_SCALE flag is enabled.
	virtual void SetScale(float scaleX, float scaleY) = 0;

	//! Sets pivot to use when center alignment flag is enabled. Default is the center of image.
	virtual void SetPivot(float centerX, float centerY) {
		mPivotX = centerX;
		mPivotY = centerY;
		mUsingCustomPivot = true;
	}
	//! Resets pivot to center of image.
	virtual void ResetPivot() {
		mPivotX = mPivotY = 0;
		mUsingCustomPivot = false;
	}

	//! Sets source rectangle. Use null to reset source rectangle.
	virtual void SetSourceRectangle(const Rectangle *r)
	{
		mUsingSourceRectangle = false;
		if (r)
		{
			mSourceRectangle = *r;
			mUsingSourceRectangle = true;
		}
	}

	//! Returns true if source rectangle is being used, or false if not.
	bool IsUsingSourceRectangle() const {
		return mUsingSourceRectangle;
	}
	//! Returns current source rectangle. Remember to check isUsingSourceRectangle() as well.
	const Rectangle & GetSourceRectangle() {
		return mSourceRectangle;
	}


	//! Draws image using current flags, with reference point (x,y).
	virtual void Draw(float x, float y) = 0;


	//! Draws image using given drawing mode and flags.
	virtual void Draw(IMAGE_DRAW_MODE drawMode, unsigned long flags,
					  float x, float y, const Rectangle *sourceRect = 0) = 0;






protected:

	unsigned long mWidth;      //!< Image width.
	unsigned long mHeight;     //!< Image height.
	unsigned long mFlags;      //!< Current flag state. \sa FLAGS
	IMAGE_DRAW_MODE mDrawMode; //!< Current drawing mode. \sa DRAW_MODE
	bool mUsingSourceRectangle;
	Rectangle mSourceRectangle;
	float mPivotX, mPivotY;
	bool mUsingCustomPivot;

}; // class Image
}

#endif // IMAGE_H_INCLUDED

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
	float x, y;
	float width, height;
	bool IsInside(float x, float y) const
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
	Image(unsigned long width, unsigned long height):
		mWidth(width), mHeight(height), mUsingSourceRectangle(false)
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


	//! Draws image using given drawing mode and flags.
	virtual void Draw(unsigned long flags,
					  float x, float y,
					  float rotation, float scaleX, float scaleY,
					  const Rectangle *sourceRect = 0) = 0;






protected:

	unsigned long mWidth;      //!< Image width.
	unsigned long mHeight;     //!< Image height.
	unsigned long mFlags;      //!< Current flag state. \sa FLAGS
	bool mUsingSourceRectangle;
	Rectangle mSourceRectangle;


}; // class Image
}

#endif // IMAGE_H_INCLUDED

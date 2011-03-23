#ifndef ALIGNMENT_H_INCLUDED
#define ALIGNMENT_H_INCLUDED

namespace BSGFX
{
	enum ALIGNMENT
	{
		//! Align horizontally to left edge (default).
		A_LEFT           = 0,
		//! Align horizontally to pivot (center by default).
		A_HORIZ_CENTER   = (1 << 2),
		//! Align horizontally to right edge.
		A_RIGHT          = (1 << 3),
		//! Align vertically to top edge (default).
		A_TOP            = 0,
		//! Align vertically to pivot (center by default).
		A_VERT_CENTER    = (1 << 4),
		//! Align vertically to bottom.
		A_BOTTOM         = (1 << 5),

		// Alignment shortcuts:
		//! Align to left center.
		A_LEFT_VCENTER   = A_LEFT | A_VERT_CENTER,
		//! Align to top-left corner.
		A_TOP_LEFT       = A_LEFT | A_TOP,
		//! Align to top center.
		A_TOP_HCENTER    = A_HORIZ_CENTER | A_TOP,
		//! Align to top-right corner.
		A_TOP_RIGHT      = A_RIGHT | A_TOP,
		//! Align to bottom-left corner.
		A_BOTTOM_LEFT    = A_LEFT | A_BOTTOM,
		//! Align to bottom center.
		A_BOTTOM_HCENTER = A_HORIZ_CENTER | A_BOTTOM,
		//! Align to bottom-right corner.
		A_BOTTOM_RIGHT   = A_RIGHT | A_BOTTOM,
		//! Align to right center.
		A_RIGHT_VCENTER  = A_RIGHT | A_VERT_CENTER,
		//! Center image with given pivot (default is center).
		A_CENTER         = A_HORIZ_CENTER | A_VERT_CENTER,

		//! Flip (mirror) image horizontally.
		A_FLIP_HORIZ           = (1 << 6),
		//! Flip (mirror) image vertically.
		A_FLIP_VERT            = (1 << 7),
	};



};

#endif // ALIGNMENT_H_INCLUDED

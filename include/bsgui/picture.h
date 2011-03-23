#ifndef PICTURE_H_INCLUDED
#define PICTURE_H_INCLUDED

#include "bsgui/controls.h"
#include <BSGFX.h>

namespace BSGUI
{
	class Picture: public Control
	{
		public:
		BSGFX::Texture *image;

		Picture(Control *parent, Theme &theme, int x1, int y1, int x2, int y2, BSGFX::Texture *image);
		virtual ~Picture();

		virtual void Render();

		virtual void Place(int x1, int y1, int x2, int y2);

		virtual void SetImage(BSGFX::Texture *image);

		float scaleX;
		float scaleY;

		Color color;




	};





}


#endif // PICTURE_H_INCLUDED

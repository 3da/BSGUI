#ifndef PICTURE_H_INCLUDED
#define PICTURE_H_INCLUDED

#include "bsgui/controls.h"

namespace BSGUI
{
	class Picture: public Control
	{
		public:
		class Image *image;

		Picture(Control *parent, Theme &theme, int x1, int y1, int x2, int y2, Image *image);
		virtual ~Picture();

		virtual void Render();

		virtual void Place(int x1, int y1, int x2, int y2);

		virtual void SetImage(Image *image);

		float scaleX;
		float scaleY;

		Color color;




	};





}


#endif // PICTURE_H_INCLUDED

#include "bsgui/picture.h"

namespace BSGUI
{
	Picture::Picture(Control *parent, Theme &t, int x1, int y1, int x2, int y2, Image *img)
	:Control(parent, t)
	{
		image = img;
		Place(x1, y1, x2, y2);
		color.Set(1,1,1,1);
	}

	Picture::~Picture()
	{

	}

	void Picture::SetImage(Image *img)
	{
		image = img;
		Place(x1, y1, x2, y2);
	}


	void Picture::Place(int x1, int y1, int x2, int y2)
	{
		Control::Place(x1, y1, x2, y2);
		int width = x2-x1;
		int height = y2-y1;

		scaleX = width/image->GetWidth();
		scaleY = height/image->GetHeight();
	}

	void Picture::Render()
	{
		int x1, y1, x2, y2;
		GetBounds(x1, y1, x2, y2);
		color.Use();
		image->Draw(IF_ALIGN_LEFT, x1, y1, 0, scaleX, scaleY);
	}







}

#include "bsgui/picture.h"

namespace BSGUI
{
	Picture::Picture(Control *parent, int x1, int y1, int x2, int y2, BSGFX::Texture *img)
	:Control(parent)
	{
		image = img;
		Place(x1, y1, x2, y2);
		color.Set(1,1,1,1);
	}

	Picture::~Picture()
	{

	}

	void Picture::SetImage(BSGFX::Texture *img)
	{
		image = img;
		Place(x1, y1, x2, y2);
	}


	void Picture::Place(int x1, int y1, int x2, int y2)
	{
		Control::Place(x1, y1, x2, y2);
		int width = x2-x1;
		int height = y2-y1;

		//scaleX = width/image->GetWidth();
		//scaleY = height/image->GetHeight();
	}

	void Picture::Render()
	{
		int x1, y1, x2, y2;
		GetBounds(x1, y1, x2, y2);
		color.Use();
		BSGFX::Drawing::Sprite(x1, y1, image, scaleX, scaleY);
		//image->Draw(IF_ALIGN_LEFT, x1, y1, 0, scaleX, scaleY);
	}







}

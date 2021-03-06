/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/actions.h"

#include "bsgui/buttons.h"
#include <Texture.h>
#include "bsgui/draw.h"


namespace BSGUI
{

Button::Button(Control *parent, int x1, int y1, int x2, int y2, const MyString caption,
	Image *bitmap)
	: Control(parent)
{
	Place(x1, y1, x2, y2);
	this->caption = caption;
	pushed = highlighted = false;
	this->bitmap = bitmap;
	autoDeleteBitmap = true;
	actionPressed = 0;
}

Button::~Button()
{
	if (autoDeleteBitmap && bitmap)
		delete bitmap;
}


void Button::SetBitmap(Image *newBitmap, bool autoDelete)
{
	if (autoDeleteBitmap && bitmap)
		delete bitmap;
	bitmap = newBitmap;
	autoDeleteBitmap = autoDelete;
}

void Button::Render()
{
	int x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);


	if (pushed)
		theme.colorFrameHovered.Use();
	else if (Focused())
		theme.colorFrameFocused.Use();
	else
		theme.colorFrameNormal.Use();
	theme.DrawFrame(x1, y1, x2, y2);
	/*if (bitmap)
	{
		x1 -= (Draw::StringWidth(caption.c_str()) + bitmap->GetWidth() + 5)/2;
		x2 = bitmap->GetWidth() + 5;
	}
	else
	{
		x1 -= Draw::StringWidth(caption.c_str())/2;
		x2 = 0;
	}*/
	float textX, textY;
	textX = x1+(x2-x1)/2;
	textY = y1+(y2-y1)/2;
	if (Focused())
		theme.colorTextFocused.Use();
	else
		theme.colorTextUnfocused.Use();
	theme.DrawString(textX, textY, caption, true);
	//Draw::DrawString(x1 + x2, (y2-y1 + 1)/2 + y1 -
	//	Draw::StringHeight()/2, caption.c_str());
	//if (bitmap)
	//	bitmap->Draw(x1, (y2 - y1 + 1)/2 + y1 - bitmap->GetHeight()/2);
}

bool Button::OnMouseDown(int x, int y, int b)
{
	if (b == 1)
	{
		pushed = true;
	}
	return Control::OnMouseDown(x, y, b);
}

bool Button::OnMouseUp(int x, int y, int b)
{
	if (!Focused())
		return false;
	if (b == 1 && pushed)
	{
		ClearMouseTracking();
		if (mouseOverControl)
			RunAction(actionPressed);
	}
	pushed = false;
	return true;
}

}

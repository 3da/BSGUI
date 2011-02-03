/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/windows.h"
#include "bsgui/screen.h"
#include "bsgui/draw.h"

#include "bsgui/image.h"

#include <GL\gl.h>

namespace BSGUI
{

Window::Window(Screen *screen, Theme &t, const MyString caption)
	: Control(screen, t)
{
	moving = resizing = resizeable = false;
	minWidth = 50;
	minHeight = 25;
	Place(10, 10, 200, 160);
	this->caption = caption;
	icon = 0;
	movable = true;
}

Window::~Window()
{
	if (icon)
		delete icon;
}

void Window::Render()
{
	int	x1, y1, x2, y2;
	int	clen;
	GetBounds(x1, y1, x2, y2);


	theme.colorTitle.Use();
	theme.FillRect(x1, y1, x2, y1+theme.height);
	if (Focused())
		theme.colorFrameFocused.Use();
	else
		theme.colorFrameNormal.Use();
	theme.FillRect(x1, y1+theme.height, x2, y2);
	theme.colorLines.Use();
	theme.DrawRect(x1, y1, x2, y2);

	theme.colorTextUnfocused.Use();
	theme.DrawString((x2-x1)/2 + x1, y1 + theme.height/2, caption.c_str(), true);

	Control::Render();

	if (resizeable)
	{
		theme.DrawLine(x2 - 10, y2, x2, y2 - 10);
	}

	if (icon)
	{
		//icon->Draw(x1 + 1, y1 + (titleY2-y1)/2 - icon->GetHeight()/2);
	}
}

bool Window::OnMouseDown(int x, int y, int b)
{
	int	x1, y1, x2, y2, titleY2;
	GetBounds(x1, y1, x2, y2);

	if (b == 1)
		Focus();
	if (resizeable && x >= x2-10 && y >= y2-10)
	{
		resizing = true;
		mX = x;
		mY = y;
	}
	else if (movable)
	{
		moving = true;
		mX = x-x1;
		mY = y-y1;
	}
	SetMouseTracking();
	return true;
}

bool Window::OnMouseMoved(int x, int y)
{
	if (moving)
		Move(x-mX, y-mY);
	if (resizing)
	{
		Resize((x2-x1+1)+x-mX, (y2-y1+1)+y-mY);
		mX = x;
		mY = y;
	}
	return true;
}

bool Window::OnMouseUp(int x, int y, int b)
{
 	//if (!(moving || resizing))
 	//	return;
	moving = resizing = false;
	ClearMouseTracking();
	return true;
}

void Window::Translate(int &x, int &y)
{
	Control::Translate(x, y);
	y += theme.height;
}


}

/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_WINDOWS_H_INCLUDED__
#define __BSGUI_WINDOWS_H_INCLUDED__

#include <string>

#include "bsgui/controls.h"

namespace BSGUI
{

struct Window : public Control
{
	MyString caption;
	bool moving;
	bool resizing;
	bool resizeable;

	bool movable;
	int mX, mY;
	class Image *icon;

	Window(Screen *screen, Theme &t, const MyString caption);
	virtual ~Window();

	virtual void Render();

	virtual bool OnMouseDown(int x, int y, int b);
	virtual bool OnMouseMoved(int x, int y);
	virtual bool OnMouseUp(int x, int y, int b);

	virtual void Translate(int &x, int &y);

};

}

#endif

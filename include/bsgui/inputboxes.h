/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_INPUTBOXES_H_INCLUDED__
#define __BSGUI_INPUTBOXES_H_INCLUDED__

#include "bsgui/controls.h"

namespace BSGUI
{

struct Inputbox : public Control
{
	int tScroll;
	int cursor;
	int cursorX;

	MyString text;

	CallbackActionFunc actionPressed;


	Inputbox(Control *parent, Theme &t, int x1, int y1, int x2, int y2);
	virtual ~Inputbox();

	void SetText(const MyString text);

	virtual void Render();

	virtual bool OnKeyDown(int key, unsigned wchar_t code);
	virtual bool OnKeyUp(int key, unsigned wchar_t code);

	protected:


};

}

#endif

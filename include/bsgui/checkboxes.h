/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_CHECKBOXES_H_INCLUDED__
#define __BSGUI_CHECKBOXES_H_INCLUDED__

#include <string>

#include "buttons.h"
#include "MyString.h"

namespace BSGUI
{

struct Checkbox : public Button
{

	Checkbox(Control *parent, Theme&, int x1, int y1, int x2, int y2, const MyString caption);
	virtual ~Checkbox();

	CallbackActionFunc actionModified;

	virtual void Render();

	virtual bool OnMouseDown(int x, int y, int b);
	virtual bool OnMouseUp(int x, int y, int b);
	virtual void OnMouseEnter();
	virtual void OnMouseExit();

	MyString caption;

	bool checked;
	bool hover;

};


}

#endif

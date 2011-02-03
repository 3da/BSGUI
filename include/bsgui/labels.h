/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_LABELS_H_INCLUDED__
#define __BSGUI_LABELS_H_INCLUDED__

#include <string>

#include "bsgui/controls.h"

namespace BSGUI
{

struct Label : public Control
{
	MyString text;

	Label(Control *parent, Theme &t, int x, int y, const MyString text);
	virtual ~Label();

	void SetText(const MyString text);
	void SetTheme(Theme &t);

	virtual void Render();
};

}

#endif


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
	std::string text;

	Label(Control *parent, Theme &t, int x, int y, char *text);
	virtual ~Label();

	void SetText(const char *text);

	virtual void Render();
};

}

#endif


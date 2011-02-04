/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_SCREEN_H_INCLUDED__
#define __BSGUI_SCREEN_H_INCLUDED__

#include "bsgui/controls.h"

namespace BSGUI
{

struct Screen : public Control
{
	int			width;
	int			height;
	int     		mouseX;
	int     		mouseY;

	Screen(Theme &t);
	virtual ~Screen();

	virtual Screen *Activate();

	static Screen *screen;
};

}

#endif

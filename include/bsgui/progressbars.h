/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_PROGRESSBARS_H_INCLUDED__
#define __BSGUI_PROGRESSBARS_H_INCLUDED__

#include "bsgui/controls.h"

namespace BSGUI
{

struct ProgressBar : public Control
{
	float max;
	float value;

	ProgressBar(Control *parent, int x1, int y1, int x2, int y2);
	virtual ~ProgressBar();

	virtual void Render();
};

}

#endif

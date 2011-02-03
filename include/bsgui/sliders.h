/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_SLIDERS_H_INCLUDED__
#define __BSGUI_SLIDERS_H_INCLUDED__

#include "bsgui/controls.h"

namespace BSGUI
{

struct Slider : public Control
{
	float max;
	float value;

	CallbackActionFunc actionModified;

	Slider(Control *parent, Theme &t, int x1, int y1, int x2, int y2);
	virtual ~Slider();

	virtual void Render();


	virtual bool OnMouseDown(int x, int y, int b);
	virtual bool OnMouseMoved(int x, int y);
	virtual bool OnMouseUp(int x, int y, int b);

protected:
	bool sliding;
};


}

#endif

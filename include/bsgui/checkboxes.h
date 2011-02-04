/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_CHECKBOXES_H_INCLUDED__
#define __BSGUI_CHECKBOXES_H_INCLUDED__

#include <string>

#include "bsgui/controls.h"

namespace BSGUI
{

struct Checkbox : public Control
{


	Checkbox(Control *parent, Theme&, int x1, int y1, int x2, int y2,char *caption);
	virtual ~Checkbox();


	virtual void Render();

	virtual bool OnMouseDown(int x, int y, int b);
	virtual bool OnMouseUp(int x, int y, int b);
	virtual void OnMouseEnter();
	virtual void OnMouseExit();

	std::string caption;

	bool checked;
	bool hover;
	bool pushed;

};


}

#endif

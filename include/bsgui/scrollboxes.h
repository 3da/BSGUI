/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_SCROLLBOXES_H_INCLUDED__
#define __BSGUI_SCROLLBOXES_H_INCLUDED__

#include "bsgui/controls.h"

namespace BSGUI
{

struct Scrollbox : public Control
{
	float   clientWidth;
	float   clientHeight;
	float   hScrollMax;
	float   vScrollMax;
	float   hScroll;
	float   vScroll;
	int	scrolling;      // 0=no, 1=vertical, 2=horizontal
	CallbackActionFunc actionScrolled;

	Scrollbox(Control *parent, Theme &t, int x1, int y1, int x2, int y2);
	virtual ~Scrollbox();

	virtual void SetScrollingBounds(float hMax, float vMax);

	virtual void Render();
	virtual void Place(int x1, int y1, int x2, int y2);
	virtual void Layout();

	virtual void OnScroll(float hd, float vd);

	virtual bool OnMouseDown(int x, int y, int b);
	virtual bool OnMouseMoved(int x, int y);
	virtual bool OnMouseUp(int x, int y, int b);
	virtual void GetBoundsChildren(int &x1, int &y1, int &x2, int &y2);

};

}

#endif


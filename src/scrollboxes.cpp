/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/scrollboxes.h"
#include "bsgui/draw.h"
#include "bsgui/actions.h"

namespace BSGUI
{

Scrollbox::Scrollbox(Control *parent, Theme &t, int x1, int y1, int x2, int y2)
	: Control(parent, t)
{
	Place(x1, y1, x2, y2);
	hScroll = 0.0f;
	vScroll = 0.0f;
	scrolling = 0;
	xShift=yShift=0;
	scrolled = NULL;
	SetScrollingBounds(1, 1);
}

Scrollbox::~Scrollbox()
{
	FreeAction(scrolled);
}

void Scrollbox::SetScrollingBounds(float hMax, float vMax)
{
	float   oldHScroll = hScroll, oldVScroll = vScroll;
	if (hMax < 1.0f)
	        hMax = 1.0f;
	if (vMax < 1.0f)
	        vMax = 1.0f;
	hScrollMax = hMax;
	vScrollMax = vMax;
	clientWidth = hMax - (x2 - x1 - theme.scrollSize);
	clientHeight = vMax - (y2 - y1 - theme.scrollSize);
	if (clientWidth > 0.0f && hScroll > clientWidth)
	        hScroll = clientWidth;
	else
		if (clientWidth <= 0.0f)
			hScroll = 0;
	if (clientHeight > 0.0f && vScroll > clientHeight)
	        vScroll = clientHeight;
	else
		if (clientHeight <= 0.0f)
			vScroll = 0;
	if (oldHScroll != hScroll || oldVScroll != vScroll)
	{
		OnScroll(hScroll - oldHScroll, vScroll - oldVScroll);
		RunAction(scrolled);
	}
}

void Scrollbox::Render()
{
	int	x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);
	theme.colorFrameHovered.Use();
	theme.FillRect(x1, y1, x2 - theme.scrollSize, y2 - theme.scrollSize);
	//x2+=theme.scrollSize;
	//y2+=theme.scrollSize;
	DrawOwnFrame();

	theme.colorLines.Use();
 	theme.DrawLine(x2 - theme.scrollSize, y1, x2 - theme.scrollSize, y2);
 	theme.DrawLine(x1, y2 - theme.scrollSize, x2, y2 - theme.scrollSize);

 	int	x = x1, y = y1 + 1;

	//if (Focused())
		theme.colorFrameFocused.Use();
	if (clientHeight > 0.0f)
	{
		if (vScroll != 0.0f)
			y = (int)((y2 - y1 - theme.scrollSize)*vScroll/clientHeight + y1 + 1);
	 	theme.FillRect(x2 - theme.scrollSize, y, x2, y + theme.scrollSize-1);
	}

	if (clientWidth > 0.0f)
	{
		if (hScroll != 0.0f)
			x = (int)((x2 - x1 - theme.scrollSize)*hScroll/clientWidth + x1 + 1);
	 	theme.FillRect(x, y2 - theme.scrollSize, x + theme.scrollSize-1, y2);
	}



 	Control::Render();
}

void Scrollbox::GetBoundsChildren(int &x1, int &y1, int &x2, int &y2)
{
	Control::GetBounds(x1,y1,x2,y2);
	x2-=theme.scrollSize+1;
	y2-=theme.scrollSize+3;
	x1++;
	y1++;
}

void Scrollbox::Place(int x1, int y1, int x2, int y2)
{
	Control::Place(x1, y1, x2, y2);
	clientWidth = hScrollMax - (x2 - x1 - theme.scrollSize);
	clientHeight = vScrollMax - (y2 - y1 - theme.scrollSize);
}

bool Scrollbox::OnMouseDown(int x, int y, int b)
{
	int   x1, y1, x2, y2;
	if (b != 1)
		return false;
	Focus();
	GetBounds(x1, y1, x2, y2);

	// vertical scrolling ?
	if (clientHeight > 0.0f && x >= x2 - theme.scrollSize && y <= y2 - theme.scrollSize)
	{
		scrolling = 1;
		SetMouseTracking();
		OnMouseMoved(x, y);
		return true;
	}

	// horizontal scrolling ?
	if (clientWidth > 0.0f && y >= y2 - theme.scrollSize && x <= x2 - theme.scrollSize)
	{
		scrolling = 2;
		SetMouseTracking();
		OnMouseMoved(x, y);
		return true;
	}
	return false;
}

void Scrollbox::Layout()
{
	float   hMax = 1.0f, vMax = 1.0f;
	std::vector<Control*>::iterator i;
	for (i=childs.begin(); i!=childs.end(); i++)
	{
		if ((*i)->x2 > hMax)
			hMax = (*i)->x2;
		if ((*i)->y2 > vMax)
			vMax = (*i)->y2;
	}
	SetScrollingBounds(hMax, vMax);
}

void Scrollbox::OnScroll(float hd, float vd)
{
	std::vector<Control*>::iterator i;
	for (i=childs.begin(); i!=childs.end(); i++)
	{
		(*i)->xShift = (int)hScroll;
		(*i)->yShift = (int)vScroll;
	}
}

bool Scrollbox::OnMouseMoved(int x, int y)
{
	int	x1, y1, x2, y2;
	float	oldHScroll, oldVScroll;
	if (!scrolling)
		return true;
	GetBounds(x1, y1, x2, y2);

	oldHScroll = hScroll;
	oldVScroll = vScroll;

	if (scrolling == 1)
	{
		vScroll = (float)(y - y1 - 6)*clientHeight/(float)(y2 - y1 -theme.scrollSize);
		if (vScroll > clientHeight)
			vScroll = clientHeight;
		else
		if (vScroll < 0.0f)
		        vScroll = 0.0f;
	}
	else
	{
		hScroll = (float)(x - x1 - 6)*clientWidth/(float)(x2 - x1 - theme.scrollSize);
		if (hScroll > clientWidth)
			hScroll = clientWidth;
		else
		if (hScroll < 0.0f)
		        hScroll = 0.0f;
	}

	if (oldHScroll != hScroll || oldVScroll != vScroll)
	{
		OnScroll(hScroll - oldHScroll, vScroll - oldVScroll);
		RunAction(scrolled);
	}
	return true;
}

bool Scrollbox::OnMouseUp(int x, int y, int b)
{
	if (!scrolling)
	        return true;
	scrolling = 0;
	ClearMouseTracking();
	return true;
}


}

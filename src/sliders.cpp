/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/sliders.h"
#include "bsgui/draw.h"
#include "bsgui/actions.h"

namespace BSGUI
{

Slider::Slider(Control *parent, Theme &t, int x1, int y1, int x2, int y2)
	: Control(parent, t)
{
	sliding = false;
	Place(x1, y1, x2, y2);
	max = 5;
	value = 0;
}

Slider::~Slider()
{
}

void Slider::Render()
{
	int			x1, y1, x2, y2, width, height, sx;
	GetBounds(x1, y1, x2, y2);
	width = x2 - x1 + 1;
	height = y2 - y1 + 1;

	sx = (int)(x1 + (width-9)*value/max + 4);

	theme.colorLines.Use();
 	theme.DrawLine(x1 + 4, y1 + height/2, x2 - 4, y1 + height/2);
	if (Focused())
		theme.colorFrameFocused.Use();
	else
		theme.colorFrameNormal.Use();
	theme.DrawFrame(sx - 4, y1, sx + 4, y2);
}

bool Slider::OnMouseDown(int x, int y, int b)
{
	if (b != 1)
		return false;
	Focus();
	sliding = true;
	SetMouseTracking();
	OnMouseMoved(x, y);
	return true;
}

bool Slider::OnMouseMoved(int x, int y)
{
	int	x1, y1, x2, y2, width, height;
	float   oldValue = value;

	if (!sliding)
		return true;

	GetBounds(x1, y1, x2, y2);
	width = x2 - x1 + 1;
	height = y2 - y1 + 1;

	if (x < x1+3)
		x = x1+3;
	else
		if (x > x2-4)
			x = x2-4;

 	value = max*(float)(x-3-x1)/(float)(width-8);
 	if (value < 0.0f)
 		value = 0.0f;
 	else
 		if (value > max)
 			value = max;

	if (value != oldValue)
		RunAction(modified);
	return true;
}

bool Slider::OnMouseUp(int x, int y, int b)
{
	if (b != 1 || !sliding)
		return true;
	sliding = false;
	ClearMouseTracking();
	return true;
}

}

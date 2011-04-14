/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/progressbars.h"

#include "bsgui/draw.h"

namespace BSGUI
{

ProgressBar::ProgressBar(Control *parent, int x1, int y1, int x2, int y2)
	: Control(parent)
{
	max = 100;
	value = 0;
	Place(x1, y1, x2, y2);
}

ProgressBar::~ProgressBar()
{
}


void ProgressBar::Render()
{
	int	x1, y1, x2, y2, w;
	GetBounds(x1, y1, x2, y2);

	theme.DrawRect(x1, y1, x2, y2);

	if (value < 0.0f)
		value = 0.0f;
	if (value > max)
		value = max;

	if (max == 0.0f || value == 0.0f)
		return;

	w = (int)((float)(x2-x1+1)*value/max);

	theme.FillRect(x1+1, y1, x1 + w + 1, y2 );
}

}

/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/labels.h"

#include "bsgui/draw.h"

namespace BSGUI
{

Label::Label(Control *parent, Theme &t, int x, int y, const ch_t *text)
	: Control(parent, t)
{
	Move(x, y);
	this->text = text;
	Resize(theme.StringWidth(text), theme.height);

}

Label::~Label()
{
}

void Label::SetText(const ch_t *t)
{
	text = t;
	Resize(theme.StringWidth(t), theme.height);

}


void Label::Render()
{
	int	x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);

	if (Focused())
		theme.colorTextFocused.Use();
	else
		theme.colorTextUnfocused.Use();
	theme.DrawString(x1, y1+theme.height/2, text.c_str(), false);
}

}

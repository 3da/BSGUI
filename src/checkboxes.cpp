/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/checkboxes.h"
#include "bsgui/draw.h"
#include "bsgui/actions.h"

namespace BSGUI
{

Checkbox::Checkbox(Control *parent, int x1, int y1, int x2, int y2, const MyString caption)
:Button(parent, x1, y1, x2, y2, caption, 0)
{
	Place(x1, y1, x2, y2);
	hover = pushed = false;
	checked = false;
	this->caption = caption;
	actionModified = 0;
}

Checkbox::~Checkbox()
{
}


void Checkbox::Render()
{
	int x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);

	theme.colorFrameNormal.Use();
	theme.DrawFrame(x1, y1 + 4, x1 + 12, y2 - 4);

	if (checked)
	{
		theme.colorLines.Use();
		theme.DrawLine(x1, y1 + 4, x1 + 12, y2 - 4);
		theme.DrawLine(x1 + 12, y1 + 4, x1, y2 - 3);
	}

	//theme.colorLines.Use();
	//theme.DrawRect(x1 + 14, y1 + 4, x2, y2 - 4);

	if (Focused())
		theme.colorTextFocused.Use();
	else
		theme.colorTextUnfocused.Use();
	theme.DrawString(x1 + 16, (y2-y1)/2 + y1, caption, false);
}


bool Checkbox::OnMouseDown(int x, int y, int b)
{
	return Button::OnMouseDown(x, y, b);
}

bool Checkbox::OnMouseUp(int x, int y, int b)
{
	if (!Focused())
		return false;;
	if (b == 1 && pushed)
	{
		ClearMouseTracking();
		if (hover)
		{
			checked=!checked;
			RunAction(actionModified);
 		}
	}
	pushed = false;
	return true;
}

void Checkbox::OnMouseEnter()
{
	Control::OnMouseEnter();
	hover = true;
}

void Checkbox::OnMouseExit()
{
	Control::OnMouseExit();
	hover = false;
}

}

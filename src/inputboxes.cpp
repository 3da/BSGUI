/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>
#include "bsgui/inputboxes.h"

#include "bsgui/draw.h"

namespace BSGUI
{

Inputbox::Inputbox(Control *parent, Theme &t, int x1, int y1, int x2, int y2)
	: Control(parent, t)
{

	Place(x1, y1, x2, y2);
	tScroll = 0;
	cursor = cursorX = 0;
	actionPressed = 0;
}

Inputbox::~Inputbox()
{
}


void Inputbox::Render()
{
	int			x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);
	if (Focused())
		theme.colorFrameFocused.Use();
	else
		theme.colorFrameNormal.Use();
	theme.FillRect(x1, y1 + 1, x2 - 1, y2);

	if (Focused())
		theme.colorTextFocused.Use();
	else
		theme.colorTextUnfocused.Use();
	theme.DrawString(x1 + 5 - tScroll, y1 + (y2 - y1)/2, text.c_str(), false);

	theme.colorLines.Use();
	if (HasKeyboardFocus())
	{

		theme.DrawLine(x1 + 5 + cursorX - tScroll, y1 + 4,
			x1 + 5 + cursorX - tScroll, y2 - 4);
	}

	theme.DrawRect(x1, y1, x2, y2);
}

bool Inputbox::OnKeyDown(int key, unsigned wchar_t ascii)
{
	int w, h;
	int x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);
	w = x2-x1;
	h = y2-y1;

	switch (ascii)
	{
		case 8:
			if (text.empty())
				return true;
			text.erase(text.length()-1);
			--cursor;
			cursorX = theme.StringWidth(text.c_str());
			if (cursorX - tScroll < 0)
			{
				tScroll = cursorX - (w/2);
				if (tScroll < 0)
				        tScroll = 0;
			}
			return true;
			break;
		case '\n':
		case '\r':
			RunAction(actionPressed);
			break;
		case 0:
			return false;
			break;
		default:
			if (ascii < 32)
			        break;
			text.push_back(ascii);
			cursor++;
			cursorX = theme.StringWidth(text.c_str());
			if (cursorX - tScroll > w - 10)
				tScroll = cursorX - w + (w/2);
			return true;
			break;
	}
}

bool Inputbox::OnKeyUp(int key, unsigned wchar_t ascii)
{
	return false;
}

void Inputbox::SetText(const MyString t)
{
	int     w, h;
	GetClientSize(w, h);
	text = t;
	cursor = 0;
	cursorX = theme.StringWidth(text.c_str());
	if (cursorX - tScroll < 0)
	{
		tScroll = cursorX - (w/2);
		if (tScroll < 0)
	        tScroll = 0;
	}
}

}

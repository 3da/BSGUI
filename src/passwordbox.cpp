

#include "bsgui/passwordbox.h"

#include "bsgui/draw.h"

namespace BSGUI
{

Passwordbox::Passwordbox(Control *parent, Theme &t, int x1, int y1, int x2, int y2)
	: Control(parent, t)
{

	Place(x1, y1, x2, y2);
	tScroll = 0;
	cursor = cursorX = 0;
}

Passwordbox::~Passwordbox()
{
}


void Passwordbox::Render()
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
	theme.DrawString(x1 + 5 - tScroll, y1 + (y2 - y1)/2, password.c_str(), false);

	theme.colorLines.Use();
	if (HasKeyboardFocus())
	{
		theme.DrawLine(x1 + 5 + cursorX - tScroll, y1 + 4,
			x1 + 5 + cursorX - tScroll, y2 - 4);
	}

	theme.DrawRect(x1, y1, x2, y2);
}

bool Passwordbox::OnKeyDown(int key, unsigned char ascii)
{
	int     w, h;
	GetClientSize(w, h);

	switch (ascii)
	{
		case 8:
			if (text.empty())
				return true;
			text.erase(text.length()-1);
			password.erase(password.length()-1);
			--cursor;
			cursorX = theme.StringWidth(password.c_str());
			if (cursorX - tScroll < 0)
			{
				tScroll = cursorX - (w/2);
				if (tScroll < 0)
				        tScroll = 0;
			}
			return true;
			break;
		case 0:
			return false;
			break;
		default:
			if (ascii < 32 || ascii > 127)
			        break;
			text.push_back(ascii);
			password.push_back('*');
			++cursor;
			cursorX = theme.StringWidth(password.c_str());
			if (cursorX - tScroll > w - 10)
				tScroll = cursorX - w + (w/2);
			return true;
			break;
	}
}

bool Passwordbox::OnKeyUp(int key, unsigned char ascii)
{
	return false;
}

const ch_t *Passwordbox::GetText()
{
	return text.c_str();
}

void Passwordbox::SetText(const ch_t *t)
{
	int     w, h;
	GetClientSize(w, h);
	text = t;
	password.clear();
	password.insert(0, text.length(), '*');
	cursor = 0;
	cursorX = theme.StringWidth(password.c_str());
	if (cursorX - tScroll < 0)
	{
		tScroll = cursorX - (w/2);
		if (tScroll < 0)
	        tScroll = 0;
	}
}

}

/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>
#include "bsgui/controls.h"
#include "bsgui/actions.h"
#include "bsgui/draw.h"
//#include "bsgui/menus.h"

#include <GL/GL.h>

#include <cstring>

namespace BSGUI
{

Control *Control::lastChildUnderMouse = 0;
Control *Control::trackControl = 0;
Control *Control::keyboardFocusControl = 0;

Control::Control(Control *parent, Theme &t)
{
	x1 = y1 = x2 = y2 = xShift = yShift = 0;
	minWidth = minHeight = 0;
	maxWidth = maxHeight = 0x7FFFFFFF;
	this->parent = childFocus = 0;
	if (parent)
		parent->AddChild(this);
	performLayout = true;
	mouseOverControl = false;
	popupMenu = 0;
	exclusiveChild = 0;
	actionClicked = 0;
	visible = true;
	theme = t;
}

Control::~Control()
{
	Invalidate();
	RemoveAllChildren();

	if (parent)
	{
		if (parent->exclusiveChild == this)
		{
			parent->exclusiveChild = 0;
			parent->childFocus = 0;
		}
		parent->RemoveChild(this);
	}
}


Control *Control::FindChild(MyString name)
{
	if (name.empty())
		return NULL;
	if (!wcscmp(name.c_str(), this->name.c_str()))
		return this;
	std::vector<Control*>::iterator i;
	for (i = childs.begin(); i!=childs.end(); i++)
	{
		Control	*r = (*i)->FindChild(name);
		if (r)
			return r;
	}
	return NULL;
}

void Control::AddChild(Control *child)
{
	if (!child)
		return;
	if (child->parent)
		child->parent->RemoveChild(child);
	child->parent = this;
	childs.push_back(child);

	performLayout = true;
}

void Control::RemoveChild(Control *child)
{
	if (!child || child->parent != this)
		return;
	child->parent = NULL;
	std::vector<Control*>::iterator i;
	for (i=childs.begin(); i!=childs.end(); i++)
	{
		if (child == (*i))
		{
			childs.erase(i);
			break;
		}

	}

	performLayout = true;
}

void Control::RemoveAllChildren()
{
	for (unsigned long i=0; i<childs.size(); i++)
	{
		delete childs[i];
	}
	childs.clear();
}

Control *Control::ChildAt(int x, int y)
{
	int	x1, y1, x2, y2;
	GetBoundsChildren(x1, y1, x2, y2);
	if (!(x >= x1 && y >= y1 && x <= x2 && y <= y2))
		return this;
	std::vector<Control*>::iterator i;
	for (i=childs.begin(); i!=childs.end(); i++)
	{
		(*i)->GetBounds(x1, y1, x2, y2);
		if (!(x >= x1 && y >= y1 && x <= x2 && y <= y2))
			continue;
		return (*i)->ChildAt(x, y);
	}
	return this;
}

Screen *Control::GetScreen()
{
	Control	*control = this;
	while (control->parent)
		control = control->parent;
	return (Screen*)control;
}

void Control::MakeExclusive()
{
	if (parent)
		parent->exclusiveChild = this;
}

void Control::Tick()
{
	if (performLayout)
	{
		Layout();
		performLayout = false;
	}
	std::vector<Control*>::iterator i;
	for (i=childs.begin(); i!=childs.end(); i++)
		(*i)->Tick();
}

void Control::Render()
{
	int	x1, y1, x2, y2;
	GetBoundsChildren(x1, y1, x2, y2);

	Draw::OpenClipArea(x1, y1, x2, y2);
	std::vector<Control*>::iterator i;
	for (i=childs.begin(); i!=childs.end(); i++)
	{
		if ((*i) == exclusiveChild || !(*i)->visible)
			continue;
		(*i)->OpenClip();
		(*i)->Render();
		(*i)->CloseClip();
	}

	if (exclusiveChild)
	{
		theme.colorFrameFocused.Use();
		theme.FillRect(x1, y1, x2, y2);

		exclusiveChild->OpenClip();
		exclusiveChild->Render();
		exclusiveChild->CloseClip();
	}

	Draw::CloseClipArea();
}

void Control::Focus()
{
	if (!parent)
		return;
	parent->Focus();

	if (parent->childFocus == this)
		return;

	std::vector<Control*>::iterator i;
	for (i = parent->childs.begin(); i != parent->childs.end(); i++)
	{
		if ((*i)==this)
		{
			parent->childs.erase(i);
			parent->childs.push_back(this);
			break;
		}
	}

	Control *oldFocus = parent->childFocus;

	parent->childFocus = this;

	if (oldFocus)
		oldFocus->OnFocusExit();
	OnFocusEnter();
}

bool Control::Focused()
{
	if (!parent)
		return true;
	return (parent->childFocus == this && parent->Focused());
}

void Control::Place(int x1, int y1, int x2, int y2)
{
	int	oldX1 = this->x1;
	int	oldY1 = this->y1;
	int	oldW = this->x2 - this->x1;
	int	oldH = this->y2 - this->y1;
	if (x2 - x1 + 1 < minWidth)
		x2 = x1 + minWidth - 1;
	if (y2 - y1 + 1 < minHeight)
		y2 = y1 + minHeight - 1;
	if (x2 - x1 + 1 > maxWidth)
		x2 = x1 + maxWidth - 1;
	if (y2 - y1 + 1 > maxHeight)
		y2 = y1 + maxHeight - 1;
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	if (x2 - x1 != oldW || y2 - y1 != oldH)
	{
		performLayout = true;
		RunAction(actionResized);
	}
	if (x1 != oldX1 || y1 != oldY1)
		RunAction(actionMoved);
}

void Control::Move(int x, int y)
{
	int	dx = x2 - x1, dy = y2 - y1;
	Place(x, y, x + dx, y + dy);
}

void Control::Resize(int width, int height)
{
	Place(x1, y1, x1 + width - 1, y1 + height - 1);
}

void Control::Center(bool horizontal, bool vertical)
{
	int	w, h, pw, ph;
	if (!parent)
		return;

	parent->GetClientSize(pw, ph);
	GetClientSize(w, h);

	if (horizontal)
		Move(pw/2 - w/2, y1);
	if (vertical)
		Move(x1, ph/2 - h/2);
}

void Control::Translate(int &x, int &y)
{
	if (parent)
		parent->Translate(x, y);
	x += x1;
	y += y1;
}

void Control::GetBounds(int &x1, int &y1, int &x2, int &y2)
{
	x1 = this->x1 - xShift;
	y1 = this->y1 - yShift;
	x2 = this->x2 - xShift;
	y2 = this->y2 - yShift;
	if (parent)
	{
		parent->Translate(x1, y1);
		parent->Translate(x2, y2);
	}
}



void Control::GetBoundsChildren(int &x1, int &y1, int &x2, int &y2)
{
	GetBounds(x1,y1,x2,y2);
}

void Control::SetTheme(Theme &t)
{
	theme = t;
	if (childs.empty())
		return;
	std::vector<Control*>::iterator i;
	for (i = childs.begin(); i != childs.end(); i++)
	{
		(*i)->SetTheme(t);
	}
}

void Control::GetClientSize(int &w, int &h)
{
	w = x2 - x1/* - theme.padLeft - theme.padRight*/;
	h = y2 - y1/* - theme.padTop - theme.padBottom*/;
}

void Control::OpenClip()
{
	int	x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);
	Draw::OpenClipArea(x1, y1, x2, y2);
}

void Control::CloseClip()
{
	Draw::CloseClipArea();
}

void Control::DrawOwnFrame()
{
	int	x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);
	theme.colorFrameNormal.Use();
	theme.DrawFrame(x1, y1, x2, y2);
}

void Control::DrawOwnFocus()
{
	int	x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);
	theme.colorFrameFocused.Use();
	theme.DrawRect(x1 + 5, y1 + 5, x2 - 5, y2 - 5);
}

void Control::OnMouseEnter()
{
	mouseOverControl = true;
}

bool Control::OnMouseMoved(int x, int y)
{
	return false;
}

void Control::OnMouseExit()
{
	mouseOverControl = false;
}

bool Control::OnMouseDown(int x, int y, int b)
{
	if (b == 1)
	{
		SetKeyboardFocus();
		Focus();
		RunAction(actionClicked);
		return true;
	}
	/*if (b == 3)
	{
		//if (popupMenu)
		//	popupMenu->Run();
	}*/
	return false;
}

bool Control::OnMouseUp(int x, int y, int b)
{
	return false;
}

bool Control::OnKeyDown(int key, unsigned wchar_t ascii)
{
	return false;
}

bool Control::OnKeyUp(int key, unsigned wchar_t ascii)
{
	return false;
}


void Control::SetMouseTracking()
{
	trackControl = this;
}

void Control::ClearMouseTracking()
{
	trackControl = 0;
}

void Control::Invalidate()
{
	if (lastChildUnderMouse == this)
		lastChildUnderMouse = NULL;
	if (keyboardFocusControl == this)
		keyboardFocusControl = NULL;
}

void Control::SetKeyboardFocus()
{
	keyboardFocusControl = this;
}

bool Control::HasKeyboardFocus()
{
	return (keyboardFocusControl == this) && (this->Focused());
}

}

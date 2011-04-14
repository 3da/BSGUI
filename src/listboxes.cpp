/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>
#include "bsgui/listboxes.h"
#include "bsgui/draw.h"
#include "bsgui/actions.h"

#include <cstring>

namespace BSGUI
{

ListboxItem::ListboxItem(Control *parent)
	: Control(parent)
{
	box = (Listbox*)parent;
	//minimumWidth = 16;
	itemHeight = theme.height + 5;
	data = NULL;
	actionSelected = 0;
}

ListboxItem::~ListboxItem()
{

}



void ListboxItem::Render()
{
	int			x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);

	if (Focused())
		theme.colorFrameFocused.Use();
	else
		theme.colorFrameNormal.Use();
	theme.FillRect(x1, y1, x2, y2);

	theme.colorLines.Use();
	theme.DrawRect(x1 + 2, y1 + 1, x2 - 1, y2 - 2);

	if (Focused())
		theme.colorTextFocused.Use();
	else
		theme.colorTextUnfocused.Use();
	theme.DrawString(x1 + 8, (y2-y1 + 1)/2 + y1, text, false);
}

void ListboxItem::Focus()
{
	Control::Focus();
	if (box->active != this)
	{
		box->active = this;
		RunAction(actionSelected);
		RunActionOf(box, box->actionModified);
	}
	//box->Focus();
}


Listbox::Listbox(Control *parent, int x1, int y1, int x2, int y2)
	: Scrollbox(parent, x1, y1, x2, y2)
{
	active = NULL;
	actionModified = 0;
}

Listbox::~Listbox()
{
}

void Listbox::Clear()
{
	for(unsigned long i=0; i<childs.size(); i++)
	{
		delete (ListboxItem*)childs[i];
	}
	childs.clear();
	active = 0;
	Layout();
}

void Listbox::AddItem(const MyString text)
{

	(new ListboxItem(this))->text = text;
	Layout();
}

void Listbox::AddItem(ListboxItem *item)
{
	AddChild(item);
}

ListboxItem *Listbox::FindItem(const MyString text)
{
	std::vector<Control*>::iterator i;
	for (i=childs.begin(); i!=childs.end(); i++)
	{
		ListboxItem *child = (ListboxItem*)(*i);
		if (!wcscmp(child->text.c_str(), text.c_str()))
		        return child;
	}
	return 0;
}

ListboxItem *Listbox::GetItem(unsigned int index)
{
	std::vector<Control*>::iterator i;
	for (i=childs.begin(); i!=childs.end(); i++)
	{
		ListboxItem *child = (ListboxItem*)(*i);
		if (child->index == index)
		        return child;
	}
	return 0;
}

void Listbox::RemoveItem(ListboxItem *item)
{
	if (!item)
	        return;
	unsigned int indexLast = item->index;
	unsigned int index = 0;

	if (item == active)
	{
		active = 0;
	}
	RemoveChild(item);
	delete item;
}

void Listbox::SelectItem(unsigned int index)
{
	SelectItem(GetItem(index));
}

void Listbox::SelectItem(ListboxItem *item)
{
	if (!item || item == active)
		return;
	active = item;
	RunActionOf(active, active->actionSelected);
	RunAction(actionModified);
}

void Listbox::Layout()
{
	int     	w, h, y = 0, index = 0;

	GetClientSize(w, h);
	w--;

	std::vector<Control*>::iterator i;
	/*for (i=childs.begin(); i!=childs.end(); i++)
	{
		ListboxItem *child = (ListboxItem*)(*i);
		if ((int)child->minimumWidth > w)
			w = child->minimumWidth;
	}*/

	for (i=childs.begin(); i!=childs.end(); i++)
	{
		ListboxItem *child = (ListboxItem*)(*i);
		child->index = index++;
		child->Place(0, y, w, y + child->itemHeight - 1);
		y += child->itemHeight;
	}

	//if (!active)
	//	active = (ListboxItem*)*(childs.begin());

	Scrollbox::Layout();
}

void Listbox::GetClientSize(int &w, int &h)
{
	Control::GetClientSize(w,h);
	w-=theme.scrollSize;
	h-=theme.scrollSize;
}

}

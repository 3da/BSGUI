/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_LISTBOXES_H_INCLUDED__
#define __BSGUI_LISTBOXES_H_INCLUDED__

#include "scrollboxes.h"

namespace BSGUI
{

struct ListboxItem : public Control
{
	struct Listbox *box;
	MyString text;
	unsigned int index;
	//unsigned int    minimumWidth;
	unsigned int    itemHeight;
	void            *data;

	CallbackActionFunc actionSelected;

	ListboxItem(Control *parent, Theme &t);
	virtual ~ListboxItem();

	virtual void Render();
	virtual void Focus();
	//virtual void GetBounds();
};

struct Listbox : public Scrollbox
{
	ListboxItem     *active;

	CallbackActionFunc actionModified;

	Listbox(Control *parent, Theme &t, int x1, int y1, int x2, int y2);
	virtual ~Listbox();

	virtual void Clear();
	virtual void AddItem(const MyString text);
	virtual void AddItem(ListboxItem *item);
	virtual ListboxItem *FindItem(const MyString text);
	virtual ListboxItem *GetItem(unsigned int index);
	virtual void RemoveItem(ListboxItem *item);
	virtual void SelectItem(unsigned int index);
	virtual void SelectItem(ListboxItem *item);

	virtual void GetClientSize(int &w, int &h);

	virtual void Layout();
};

}

#endif

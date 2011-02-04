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
	std::string text;
	unsigned int index;
	//unsigned int    minimumWidth;
	unsigned int    itemHeight;
	void            *data;

	ListboxItem(Control *parent, Theme &t);
	virtual ~ListboxItem();

	virtual void Render();
	virtual void Focus();
	//virtual void GetBounds();
};

struct Listbox : public Scrollbox
{
	ListboxItem     *active;

	Listbox(Control *parent, Theme &t, int x1, int y1, int x2, int y2);
	virtual ~Listbox();

	virtual void Clear();
	virtual void AddItem(const char *text);
	virtual void AddItem(ListboxItem *item);
	virtual ListboxItem *FindItem(const char *text);
	virtual ListboxItem *GetItem(unsigned int index);
	virtual void RemoveItem(ListboxItem *item);
	virtual void SelectItem(unsigned int index);
	virtual void SelectItem(ListboxItem *item);

	virtual void GetClientSize(int &w, int &h);

	virtual void Layout();
};

}

#endif

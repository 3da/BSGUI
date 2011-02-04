/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/menus.h"
#include "bsgui/images.h"
#include "bsgui/draw.h"
#include "bsgui/actions.h"

MenuItem::MenuItem(Control *owner, const char *caption, Action *clickedAction,
	Bitmap *bitmap)
	: Control(owner)
{
	this->bitmap = bitmap;
	clicked = clickedAction;
	subMenu = NULL;
	popup = NULL;
}

MenuItem::~MenuItem()
{
	if (subMenu)
		delete subMenu;
	if (bitmap)
		delete bitmap;
}

MenuItem *MenuItem::AddItem(const char *caption, Action *clicked, Bitmap *bitmap)
{
	if (!subMenu)
	{
	        subMenu = new PopupMenu;
	        subMenu->rootMenuControl = menuControl;
	}
	return subMenu->AddItem(caption, clicked, bitmap);
}

void MenuItem::Clear()
{
	if (subMenu)
		subMenu->Clear();
}

void MenuItem::Render()
{
	int     x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);

	if (caption[0] == '-' && !caption[1])
	{
		BSGUIDraw::Color(220, 234, 235);
		BSGUIDraw::DrawLine(x1, y1 + (y2 - y1 + 1)/2, x2 + 1,
			y1 + (y2 - y1 + 1)/2);
		return;
	}

	if (mouseOverControl || (subMenu && subMenu->menu->parent))
	{
		BSGUIDraw::Color(220, 234, 235);
		BSGUIDraw::FillRect(x1, y1, x2, y2);
		BSGUIDraw::Color(20, 34, 35);
	}
	else
	        BSGUIDraw::Color(220, 234, 235);

	BSGUIDraw::DrawString(x1 + ((MenuControl*)parent)->iconPad + 5, y1 + 1, caption.c_str());
	if (bitmap)
		bitmap->Draw(x1 + 5, (y2 - y1 + 1)/2 - bitmap->height/2 + y1);
}

void MenuItem::OnMouseEnter()
{
	Control::OnMouseEnter();
	for (Control *child = parent->firstChild;child;child = child->next)
	{
		if (((MenuItem*)child)->subMenu &&
			((MenuItem*)child)->subMenu->menu->parent)
		{
			((MenuItem*)child)->subMenu->Cancel();
			break;
		}
	}

	if (subMenu || popup)
	{
		int     x1, y1, x2, y2;
		GetBounds(x1, y1, x2, y2);
		BSGUI_RUNACTION(popup);
		subMenu->Run(x2 - 10, y1 + 5);
	}
}

void MenuItem::OnMouseUp(int x, int y, int b)
{
	if (b == 1)
		BSGUI_RUNACTION(clicked);
	if (b < 4)
		menuControl->popupMenu->Cancel();
}


MenuControl::MenuControl(Control *owner)
	: Control(owner)
{
	Control::SetPadding(2, 2, 2, 2);
	iconPad = 0;
}

MenuControl::~MenuControl()
{
}

void MenuControl::CalcSize()
{
	int     	w = 10, h = 3;

	for (Control *child=firstChild;child;child = child->next)
	{
		if (w < BSGUIDraw::StringWidth(((MenuItem*)child)->caption.c_str())+10)
		        w = BSGUIDraw::StringWidth(((MenuItem*)child)->caption.c_str())
				+ 10;
		if (((MenuItem*)child)->caption[0] == '-' &&
		        !((MenuItem*)child)->caption[1])
			h += 4;
		else
			h += BSGUIDraw::StringHeight() + 3;
	}

	Resize(w + (iconPad?(iconPad+5):0), h);
}

void MenuControl::Layout()
{
	int     	w, h, y = 0;

	iconPad = 0;
	GetClientSize(w, h);
	w--;
	for (Control *child=firstChild;child;child = child->next)
	{
		int     height;
		if (((MenuItem*)child)->bitmap &&
			iconPad < (int)((MenuItem*)child)->bitmap->width)
			iconPad = ((MenuItem*)child)->bitmap->width + 5;
		if (((MenuItem*)child)->caption[0] == '-' &&
		        !((MenuItem*)child)->caption[1])
			height = 4;
		else
			height = BSGUIDraw::StringHeight() + 3;
		child->Place(0, y, w, y + height - 1);
		y += height;
	}

	Control::Layout();
}

void MenuControl::Render()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	BSGUIDraw::Color(120, 134, 135, 192);
	BSGUIDraw::FillRect(x1, y1, x2, y2);
	BSGUIDraw::Color(220, 234, 235);
	BSGUIDraw::DrawRect(x1, y1, x2, y2);
	glDisable(GL_BLEND);

	Control::Render();
}

void MenuControl::OnFocusExit()
{
	Control::OnFocusExit();
	if (popupMenu->rootMenuControl)
		popupMenu->rootMenuControl->popupMenu->Cancel();
	else
	        popupMenu->Cancel();
}


PopupMenu::PopupMenu()
{
	rootMenuControl = NULL;
	menu = new MenuControl(NULL);
	menu->popupMenu = this;
}

PopupMenu::~PopupMenu()
{
	if (!menu->parent)
	        delete menu;
}

MenuItem *PopupMenu::AddItem(const char *caption, Action *clicked, Bitmap *bitmap)
{
	MenuItem *item = new MenuItem(menu, caption, clicked, bitmap);
	item->menuControl = rootMenuControl?rootMenuControl:menu;
	menu->CalcSize();
	menu->Layout();
	return item;
}

void PopupMenu::Clear()
{
	menu->RemoveAllChildren();
}

void PopupMenu::Run(int x, int y)
{
	if (menu->parent)
	        Cancel();
	if (x == -10000)
	        x = screen->mouseX;
	if (y == -10000)
	        y = screen->mouseY;
	menu->CalcSize();
	menu->Layout();
	screen->AddChild(menu);
	menu->Move(x, y);
	menu->Focus();
}

void PopupMenu::Cancel()
{
	for (Control *child = menu->firstChild;child;child = child->next)
	{
		if (((MenuItem*)child)->subMenu &&
			((MenuItem*)child)->subMenu->menu->parent)
		{
			((MenuItem*)child)->subMenu->Cancel();
			break;
		}
	}

	if (menu->parent)
	        menu->parent->RemoveChild(menu);
}

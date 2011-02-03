/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/messageboxes.h"
#include "bsgui/controls.h"
#include "bsgui/windows.h"
#include "bsgui/labels.h"
#include "bsgui/buttons.h"
#include "bsgui/actions.h"
#include "bsgui/screen.h"

namespace BSGUI
{

static void CloseMessageBox(Control *sender)
{
	delete sender->parent;
}

void MessageBox(Theme &t, const MyString msg, const MyString caption, bool exclusive)
{
	Window *w = new Window(Screen::screen, t, caption);
	Label *l = new Label(w, t, 20, 20, msg);
	Button *b = new Button(w, t, 0, 0, 100, 25, L"Ok");
	w->Resize((l->x2 - l->x1) + 40, 80);
	l->Center();
	b->Move(0, l->y2 + 15);
	b->Center(true, false);
	w->Resize(w->x2 - w->x1 + 1, w->y2 - w->y1 + 16 + b->y2 - b->y1);
	b->actionPressed = CloseMessageBox;
	if (exclusive)
		w->MakeExclusive();
	w->Center();
}

}

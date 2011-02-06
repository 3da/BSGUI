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

void MessageBox(Theme &t, ch_t *msg, ch_t *caption, bool exclusive)
{
	Window *w = new Window(Screen::screen, t, caption);
	Label *l = new Label(w, t, 20, 20, msg);
#ifdef UNICODE
	Button *b = new Button(w, t, 0, 0, 100, 25, L"Ok");
#else
	Button *b = new Button(w, t, 0, 0, 100, 25, "Ok");
#endif
	w->Resize((l->x2 - l->x1) + 40, 80);
	l->Center();
	b->Move(0, l->y2 + 15);
	b->Center();
	w->Resize(w->x2 - w->x1 + 1, w->y2 - w->y1 + 16 + b->y2 - b->y1);
	b->clicked = new CallbackAction(CloseMessageBox);
	if (exclusive)
		w->MakeExclusive();
	w->Center();
}

}
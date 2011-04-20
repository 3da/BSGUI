#include "bsgui/gfxsettings.h"

#include "bsgui/labels.h"
#include "bsgui/buttons.h"
#include "bsgui/listboxes.h"

#include <wchar.h>

namespace BSGUI
{

static void SaveAction(Control *sender)
{
	GfxSettings *gs = (GfxSettings*)sender->parent;
	gs->actionSave(gs);
	delete gs;
}

static void CancelAction(Control *sender)
{
	GfxSettings *gs = (GfxSettings*)sender->parent;
	gs->actionCancel(gs);
	delete gs;
}


GfxSettings::GfxSettings(Theme &t, Screen *screen)
: Window(screen, t, "Graphical settings")
{
	int	w, h;
	Label	*l;
	Button	*b;
	Resize(500, 350);
	Center();

	actionCancel = 0;
	actionSave = 0;

	GetClientSize(w,h);

	l = new Label(this, t, 5, 5, L"Select graphical mode:");
	lstModes = new Listbox(this, t, 5, l->y2 + 5, w-10, h-60);

	b = new Button(this, t, w-125, lstModes->y2 + 5, w-70, lstModes->y2 + 30, L"Save");
	b->actionPressed = SaveAction;
	b = new Button(this, t, w-65, lstModes->y2 + 5, w-10, lstModes->y2+30, L"Cancel");
	b->actionPressed = CancelAction;

	vecModes = BSGFX::Videomode::GetAvaible();
	for (unsigned int i = 0; i < vecModes.size(); i++)
	{
		wchar_t buf[128];
		swprintf(buf, L"%dx%d", vecModes[i].GetWidth(), vecModes[i].GetHeight());
		lstModes->AddItem(buf);
	}


	MakeExclusive();
}

BSGFX::Videomode GfxSettings::GetSelected()
{
	r
}

}



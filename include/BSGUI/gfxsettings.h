#ifndef GFXSETTINGS_H_INCLUDED
#define GFXSETTINGS_H_INCLUDED

#include "bsgui/windows.h"

namespace BSGUI
{

struct GfxSettings: public Window
{
	GfxSettings(Theme &t, Screen *screen);

	class Listbox *lstModes;
	BSGFX::Videomode GetSelected();

	CallbackActionFunc actionCancel;
	CallbackActionFunc actionSave;

	std::vector<BSGFX::Videomode> vecModes;
};


}

#endif // GFXSETTINGS_H_INCLUDED

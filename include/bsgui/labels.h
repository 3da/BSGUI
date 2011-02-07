/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_LABELS_H_INCLUDED__
#define __BSGUI_LABELS_H_INCLUDED__

#include <string>

#include "bsgui/controls.h"

namespace BSGUI
{

struct Label : public Control
{
	str_t text;

	Label(Control *parent, Theme &t, int x, int y, const ch_t *text);
	virtual ~Label();

	void SetText(const ch_t *text);
	void SetTheme(Theme &t);

	virtual void Render();
};

}

#endif


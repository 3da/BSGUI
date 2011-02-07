/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_MODELVIEWS_H_INCLUDED__
#define __BSGUI_MODELVIEWS_H_INCLUDED__

#include "bsgui/renderers.h"

namespace BSGUI
{

struct ModelView : public Renderer
{
	CallbackActionFunc renderView;

	ModelView(Control *parent, Theme &t, int x1, int y1, int x2, int y2);
	virtual ~ModelView();

	virtual void OffscreenRender();
	virtual void Render();
};

}

#endif

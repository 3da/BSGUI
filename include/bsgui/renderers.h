/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_RenderERS_H_INCLUDED__
#define __BSGUI_RenderERS_H_INCLUDED__

#include "bsgui/controls.h"

#include <GL/GL.h>

namespace BSGUI
{

struct Renderer : public Control
{
	GLuint			texture;
	void			*RendererListEntry;
	bool			enabled;

	Renderer(Control *parent, Theme &t);
	virtual ~Renderer();

	virtual void Place(int x1, int y1, int x2, int y2);
	virtual void OffscreenRender();
	virtual void DrawOffscreenRender();
};

}

#endif

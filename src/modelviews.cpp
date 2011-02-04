/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/modelviews.h"

#include "bsgui/actions.h"

namespace BSGUI
{

ModelView::ModelView(Control *parent, Theme &t, int x1, int y1, int x2, int y2)
	: Renderer(parent, t)
{
	Place(x1, y1, x2, y2);
	renderView = NULL;
}

ModelView::~ModelView()
{
	FreeAction(renderView);
}

void ModelView::OffscreenRender()
{
	RunAction(renderView);
}

void ModelView::Render()
{
	DrawOffscreenRender();
}

}

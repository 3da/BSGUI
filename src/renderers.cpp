/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>
#include "bsgui/renderers.h"
#include "bsgui/controls.h"
#include "bsgui/draw.h"
#include "bsgui/screen.h"

#include <cstdlib>

#include <GL/GL.h>

namespace BSGUI
{

struct RendererListItem
{
	struct Renderer		*r;
	RendererListItem	*prev;
	RendererListItem	*next;
};

RendererListItem	*Renderer = 0;
RendererListItem	*lastRenderer = 0;

static RendererListItem *AddRenderer(class Renderer *r)
{
	RendererListItem	*item = new RendererListItem;
	item->r = r;
	item->prev = lastRenderer;
	item->next = 0;
	if (Renderer)
		lastRenderer->next = item;
	else
		Renderer = item;
	lastRenderer = item;
	return item;
}

static void removeRenderer(RendererListItem *item)
{
	if (item->prev)
		item->prev->next = item->next;
	else
		Renderer = item->next;
	if (item->next)
		item->next->prev = item->prev;
	else
		lastRenderer = item->prev;
	delete item;
}

Renderer::Renderer(Control *parent)
	: Control(parent)
{
	glGenTextures(1, &texture);
	RendererListEntry = AddRenderer(this);

	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);

	void	*buffer = malloc((x2 - x1 + 1)*(y2 - y1 + 1));
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, x2 - x1 + 1,
		y2 - y1 + 1, 0, GL_RED, GL_BYTE, buffer);
	free(buffer);

	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
	glDisable(GL_TEXTURE_RECTANGLE_ARB);

	enabled = true;
}

Renderer::~Renderer()
{
	removeRenderer((RendererListItem*)RendererListEntry);
	glDeleteTextures(1, &texture);
}

void Renderer::Place(int x1, int y1, int x2, int y2)
{
	Control::Place(x1, y1, x2, y2);

	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);

	void	*buffer = malloc((x2 - x1 + 1)*(y2 - y1 + 1));
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, x2 - x1 + 1,
		y2 - y1 + 1, 0, GL_RED, GL_BYTE, buffer);
	free(buffer);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
}

void Renderer::OffscreenRender()
{
}

void Renderer::DrawOffscreenRender()
{
	if (!enabled)
		return;

	int	x1, y1, x2, y2;
	GetBounds(x1, y1, x2, y2);

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex2f(x1 + 1, y1);
	glTexCoord2f(x2 - x1, 0);	glVertex2f(x2 + 1, y1);
	glTexCoord2f(x2 - x1, y2 - y1);	glVertex2f(x2 + 1, y2);
	glTexCoord2f(0, y2 - y1);	glVertex2f(x1 + 1, y2);
	glEnd();
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
}

void RenderOffscreenBSGUIControls()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	for (RendererListItem *item = Renderer;item;item = item->next)
	{
		if (!item->r->enabled)
			continue;
		glViewport(0, 0, item->r->x2 - item->r->x1 + 1,
			item->r->y2 - item->r->y1 + 1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		item->r->OffscreenRender();
		glEnable(GL_TEXTURE_RECTANGLE_ARB);
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, item->r->texture);
		glCopyTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 0, 0, 0, 0,
			item->r->x2 - item->r->x1 + 1,
			item->r->y2 - item->r->y1 + 1);
		glDisable(GL_TEXTURE_RECTANGLE_ARB);
	}
	glViewport(0, 0, Screen::screen->width, Screen::screen->height);
	glBindTexture(GL_TEXTURE_2D, 0);
}

}

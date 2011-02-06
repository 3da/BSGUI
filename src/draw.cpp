/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include <GL/GL.h>
#include "bsgui/draw.h"
#include "bsgui/screen.h"
#include "bsgui/font.h"

#include <SDL/SDL.h>

namespace BSGUI
{

struct CharInfo
{
	float	u1;
	float	v1;
	float	u2;
	float	v2;
	float	w;
};

struct ClipArea
{
	int		x1;
	int		y1;
	int		x2;
	int		y2;
	ClipArea	*next;
};

static int	orthoEnableCount = 0;

static Image *cursorImage;
static ClipArea	*carea = 0;
static unsigned long cursorFlags = IF_ALIGN_TOP_LEFT;

void Draw::EnableOrtho()
{
	orthoEnableCount++;
	if (orthoEnableCount != 1)
		return;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

  	glLoadIdentity();
	glOrtho(0, Screen::screen->width, Screen::screen->height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void Draw::DisableOrtho()
{
	orthoEnableCount--;
	if (orthoEnableCount != 0)
		return;
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
 	glMatrixMode(GL_MODELVIEW);
 	glPopMatrix();
}


bool Draw::SetCursorImage(Image *image)
{
	cursorImage = image;
	return true;
}

void Draw::DrawCursor(int x, int y)
{
	Color(1,1,1,1).Use();
	cursorImage->Draw(IDM_NORMAL, cursorFlags, x, y);
}

void Draw::SetCursorAlign(unsigned long flags)
{
	cursorFlags	= flags;
}


void Draw::SetClipArea(int x1, int y1, int x2, int y2)
{
	glScissor(x1, Screen::screen->height - y2 - 2, x2 - x1+1, y2 - y1+2);
}

void Draw::ResetClipping()
{
	ClipArea	*ca;
	glScissor(0, 0, Screen::screen->width, Screen::screen->height);
	while (carea)
	{
		ca = carea->next;
		delete carea;
		carea = ca;
	}
}

void Draw::OpenClipArea(int x1, int y1, int x2, int y2)
{
	ClipArea	*ca = new ClipArea;
	if (carea)
	{
		if (x1 < carea->x1)
			x1 = carea->x1;
		if (y1 < carea->y1)
			y1 = carea->y1;
		if (x2 > carea->x2)
			x2 = carea->x2;
		if (y2 > carea->y2)
			y2 = carea->y2;
	}
	ca->x1 = x1;
	ca->y1 = y1;
	ca->x2 = x2;
	ca->y2 = y2;
	ca->next = carea;
	carea = ca;
	SetClipArea(x1, y1, x2, y2);
}

void Draw::CloseClipArea()
{
	ClipArea	*ca;
	if (!carea)
	{
		glScissor(0, 0, Screen::screen->width, Screen::screen->height);
		return;
	}
	ca = carea->next;
	delete carea;
	carea = ca;
	if (carea)
		SetClipArea(ca->x1, ca->y1, ca->x2, ca->y2);
	else
		glScissor(0, 0, Screen::screen->width, Screen::screen->height);
}

}

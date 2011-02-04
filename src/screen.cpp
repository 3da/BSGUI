/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/screen.h"

#include <GL/GL.h>

namespace BSGUI
{

Screen *Screen::screen = 0;

Screen::Screen(Theme &t)
	: Control(NULL, t)
{
	GLint	vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	x2 = vp[2] - 1;
	y2 = vp[3] - 1;
 	width = vp[2];
 	height = vp[3];
}

Screen::~Screen()
{
}

Screen *Screen::Activate()
{
	Screen	*prevScreen = screen;
	screen = this;
	GLint	vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	x2 = vp[2] - 1;
	y2 = vp[3] - 1;
 	width = vp[2];
 	height = vp[3];
	return prevScreen;
}

}

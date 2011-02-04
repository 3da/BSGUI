/*
** Bad Sector's OpenGL GUI
*/

#include <bsgui/bsgui.h>

#include "bsgui/draw.h"
#include "bsgui/screen.h"
#include "bsgui/controls.h"

#include <GL/GL.h>
#include <SDL/SDL.h>

namespace BSGUI
{

using namespace Draw;

static void BeginRender()
{
    // Based on SDL_GL_Enter2DMode from SDL examples.
    SDL_Surface *screen = SDL_GetVideoSurface();

    glPushAttrib(GL_COLOR_BUFFER_BIT);
    glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_SCISSOR_TEST);

    // This allows alpha blending of 2D textures with the scene.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, screen->w, screen->h);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0, (GLdouble)screen->w, (GLdouble)screen->h, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // useful mostly only if all 2D operations use integer coordinates:
    // TODO - supply a hint for beginRender if scaling is used or not?
    //glTranslatef(0.375f, 0.375f, 0);

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor4f(1, 1, 1, 1);
}


static void EndRender()
{
    // Based on SDL_GL_Leave2DMode from SDL examples.
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib(); // GL_ENABLE_BIT
    glPopAttrib(); // GL_COLOR_BUFFER_BIT
}


void Render()
{
	/*bool	scissorEnabled = glIsEnabled(GL_SCISSOR_TEST) == GL_TRUE;
	bool	depthEnabled = glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
	bool	cullingEnabled = glIsEnabled(GL_CULL_FACE) == GL_TRUE;
	bool	textureEnabled = glIsEnabled(GL_TEXTURE_2D) == GL_TRUE;
	bool	lightingEnabled = glIsEnabled(GL_LIGHTING) == GL_TRUE;
	if (!scissorEnabled)
		glEnable(GL_SCISSOR_TEST);
	if (depthEnabled)
		glDisable(GL_DEPTH_TEST);
	if (cullingEnabled)
		glDisable(GL_CULL_FACE);
	if (!textureEnabled)
		glEnable(GL_TEXTURE_2D);
	if (lightingEnabled)
		glDisable(GL_LIGHTING);*/
	BeginRender();
	//glLoadIdentity();
	//EnableOrtho();

	Screen::screen->Render();
	Draw::ResetClipping();

	int	x, y;
	SDL_GetMouseState(&x, &y);
	Screen::screen->mouseX = x;
	Screen::screen->mouseY = y;
	Draw::DrawCursor(x - 1, y - 1);

	//DisableOrtho();
	EndRender();
	/*if (lightingEnabled)
		glEnable(GL_LIGHTING);
	if (!textureEnabled)
		glDisable(GL_TEXTURE_2D);
	if (cullingEnabled)
		glEnable(GL_CULL_FACE);
	if (depthEnabled)
		glEnable(GL_DEPTH_TEST);
	if (!scissorEnabled)
		glDisable(GL_SCISSOR_TEST);*/
}

void Tick()
{
	Screen::screen->Tick();
}

bool HandleSDLEvent(SDL_Event *ev)
{
	Control	*child;

	if (!Screen::screen)
		return false;

	switch (ev->type)
	{
		case SDL_MOUSEBUTTONDOWN:
			if (Control::trackControl)
			{
				Control::trackControl->OnMouseDown(ev->button.x, ev->button.y, ev->button.button);
				return true;
			}
			if (Screen::screen->exclusiveChild)
			{
				int	x1, y1, x2, y2;
				Screen::screen->exclusiveChild->GetBounds(x1, y1, x2, y2);
				if (!(ev->button.x >= x1 && ev->button.x <= x2 && ev->button.y >= y1 && ev->button.y <= y2))
					return false;
				child = Screen::screen->exclusiveChild->ChildAt(ev->button.x, ev->button.y);
			}
			else
				child = Screen::screen->ChildAt(ev->button.x, ev->button.y);
			if (child->visible)
   				return child->OnMouseDown(ev->button.x, ev->button.y, ev->button.button);
			return false;
		case SDL_MOUSEBUTTONUP:
			if (Control::trackControl)
			{
				Control::trackControl->OnMouseUp(ev->button.x, ev->button.y, ev->button.button);
				return true;
			}
			if (Screen::screen->exclusiveChild)
				child = Screen::screen->exclusiveChild->ChildAt(ev->button.x, ev->button.y);
			else
				child = Screen::screen->ChildAt(ev->button.x, ev->button.y);
			if (child->visible)
   				return child->OnMouseUp(ev->button.x, ev->button.y, ev->button.button);
			return false;
		case SDL_MOUSEMOTION:
			if (Control::trackControl)
			{
				Control::trackControl->OnMouseMoved(ev->button.x, ev->button.y);
				return true;
			}
			if (Screen::screen->exclusiveChild)
				child = Screen::screen->exclusiveChild->ChildAt(ev->motion.x, ev->motion.y);
			else
				child = Screen::screen->ChildAt(ev->motion.x, ev->motion.y);
			if (Control::lastChildUnderMouse != child)
			{
				if (Control::lastChildUnderMouse)
					Control::lastChildUnderMouse->OnMouseExit();
				Control::lastChildUnderMouse = child;
				child->OnMouseEnter();
			}
			if (child->visible)
   				return child->OnMouseMoved(ev->motion.x, ev->motion.y);
			return false;
		case SDL_KEYDOWN:
			if (!(Control::keyboardFocusControl && Control::keyboardFocusControl->Focused()))
				return false;
			return Control::keyboardFocusControl->OnKeyDown(ev->key.keysym.sym, ev->key.keysym.unicode&0x7F);
		case SDL_KEYUP:
			if (!(Control::keyboardFocusControl && Control::keyboardFocusControl->Focused()))
				return false;
			return Control::keyboardFocusControl->OnKeyUp(ev->key.keysym.sym, ev->key.keysym.unicode&0x7F);
	}
	return false;
}




}


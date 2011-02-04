/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>

#include "bsgui/bsgui.h"

static bool	Running;
Action		*terminateBSGUIApplicationAction = 0;

static void TerminateBSGUI(Control *sender)
{
	Running = false;
}

static void InitOpenGL(char *caption, int width, int height, bool fullscreen)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_SetVideoMode(width, height, 32, SDL_OPENGL|
		(fullscreen?SDL_FULLSCREEN:0));
	SDL_WM_SetCaption(caption, caption);
	SDL_ShowCursor(false);

	SDL_EnableUNICODE(true);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
		SDL_DEFAULT_REPEAT_INTERVAL);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0f, (float)width/(float)height, 1.0f, 1024.0f);
	glMatrixMode(GL_MODELVIEW);
}

static void Render()
{
	RenderOffscreenBSGUIControls();

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

 	RenderBSGUI();

  	SDL_GL_SwapBuffers();
}

bool initBSGUIApplication(char *caption, int width, int height, bool fullscreen)
{
	InitOpenGL(caption, width, height, fullscreen);
	if (!InitBSGUI())
	{
		SDL_Quit();
		return false;
	}
	BSGUIDraw::LoadFontData("data/fontdata.bmp", "data/fontsize.dat");
	BSGUIDraw::LoadCursorImage("data/cursor.bmp", "data/cursorAlpha.bmp");
	terminateBSGUIApplicationAction = new CallbackAction(TerminateBSGUI);
	terminateBSGUIApplicationAction->autoDelete = false;
	return true;
}

void RunBSGUIApplication(void (*idleFunc)())
{
	SDL_Event	event;

	Running = true;
	while (Running)
	{
		if (idleFunc)
			idleFunc();
		TickBSGUI();
		while (SDL_PollEvent(&event))
		{
			if (!HandleSDLEvent(&event))
				switch (event.type)
				{
					case SDL_QUIT:
						Running = false;
					break;
				}
		}
		Render();
	}
}

void TerminateBSGUIApplication(void)
{
	Running = false;
}

void ShutdownBSGUIApplication()
{
	delete terminateBSGUIApplicationAction;
	ShutdownBSGUI();
	SDL_Quit();
}

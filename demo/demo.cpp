/*
** Bad Sector's OpenGL GUI **DEMO**
*/

#include <bsgui/bsgui.h>
#include <math.h>
//#undef main

#undef MessageBox

using namespace BSGUI;

bool	running = true;
float	rotation = 0.0f, size = 1.0f, backgnd = 0.2f, progress = 0.0f;
int	steps = 5;
Slider	*stepCount, *sizeSlider, *backSlider;
Window	*win;
Screen	*screen1, *screen2;
Label	*fps;

BMFont *font;

Theme theme;

void stepsModifiedAction(Control *sender)
{
	ProgressBar	*pbar2 = (ProgressBar*)Screen::screen->FindChild("pbar2");
	steps = (int)stepCount->value + 3;
	pbar2->value = steps - 3;
}

void terminateAppAction(Control *sender)
{
	running = false;
}

void closeWindowAction(Control *sender)
{
	delete sender->parent;
}

void newWindowAction(Control *sender)
{
	Window	*w = new Window(Screen::screen, theme, "A simple window");
	w->Place(10, 10, 300, 100);
	w->Move(rand()%600+30, rand()%500+30);

	Button	*button = new Button(w, theme, 10, 20, 280, 55, "Close me");
	button->clicked = new CallbackAction(closeWindowAction);
}

void addWideItemAction(Control *sender)
{
	Listbox *lbox = (Listbox*)Screen::screen->FindChild("listbox");
	lbox->AddItem("A very wide item to test horizontal scrolling");
	//lbox->Clear();
}

void removeItemAction(Control *sender)
{
	Listbox *lbox = (Listbox*)Screen::screen->FindChild("listbox");
	lbox->RemoveItem(lbox->active);
}

void addTheAboveTextAction(Control *sender)
{
	Listbox *lbox = (Listbox*)Screen::screen->FindChild("listbox");
	lbox->AddItem(((Passwordbox*)Screen::screen->FindChild("inputbox"))->text.c_str());
	((Passwordbox*)Screen::screen->FindChild("inputbox"))->SetText("");
	Screen::screen->FindChild("inputbox")->Focus();
}

void activateScreen1(Control *sender)
{
	screen1->Activate();
}

void activateScreen2(Control *sender)
{
	screen2->Activate();
}

void renderObject()
{
	glDisable(GL_CULL_FACE);
	glBegin(GL_TRIANGLES);
	float	a = 0.0f, step;
	step = (360.0f/(float)steps)*3.14f/180.0f;
	for (int i=0;i<steps;i++,a+=step)
	{
		glColor3f(i/(float)steps, i/((float)steps+1.0f),
  			i/((float)steps+2.0f));
		glVertex3f(0.0f, size, 0.0f);
		glVertex3f(sinf(a)*size, 0.0f, cosf(a)*size);
		glVertex3f(sinf(a+step)*size, 0.0f, cosf(a+step)*size);
	}
	a = 0.0f;
	for (int i=0;i<steps;i++,a+=step)
	{
		glColor3f(i/(float)steps, i/((float)steps+1.0f),
  			i/((float)steps+2.0f));
		glVertex3f(0.0f, -size, 0.0f);
		glVertex3f(sinf(a)*size, 0.0f, cosf(a)*size);
		glVertex3f(sinf(a+step)*size, 0.0f, cosf(a+step)*size);
	}
	glEnd();
	glEnable(GL_CULL_FACE);
}

void renderObjectAction(Control *sender)
{
	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(rotation, 0.0f, 1.0f, 0.0f);
	renderObject();
}

void fileSelectedAction(Control *sender)
{
	const char *fname = ((FileBrowser*)sender)->GetFilename();
	printf("File selected: %s\n", fname);
}

void selectFileAction(Control *sender)
{
	FileBrowser	*browser = new FileBrowser(theme, Screen::screen, "Select File");
	browser->selected = new CallbackAction(fileSelectedAction);
}

void aboutBoxAction(Control *sender)
{
	MessageBox(theme, "BSGUI (Bad Sector's OpenGL GUI) version 0.2 Demo", "About");
}

void imageWinResizedAction(Control *sender)
{
	//Image	*img = (Image*)*sender->childs.begin();
	//int	w, h;
	//sender->GetClientSize(w, h);
	//img->minWidth = 10;
	//img->minHeight = 10;
	//img->Place(img->x1, img->y1, w-img->x1 - 1, h-img->y1 - 1);
}

void enableFiltering(Control *sender)
{
	Image	*img = (Image*)Screen::screen->FindChild("cenda");
	//img->bitmap->SetFiltering(true);
}

void disableFiltering(Control *sender)
{
	Image	*img = (Image*)Screen::screen->FindChild("cenda");
	//img->bitmap->SetFiltering(false);
}

void cendaClicked(Control *sender)
{
	selectFileAction(sender);
}

void render()
{
	//RenderOffscreenBSGUIControls();

	glClearColor((146.0f/255.0f)*backgnd, (139.0f/255.0f)*backgnd,
		(125.0f/255.0f)*backgnd, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(rotation, 1.0f, 0.0f, 0.0f);
	glRotatef(rotation/0.6f, 0.0f, 1.0f, 0.0f);

	renderObject();

 	Render();

  	SDL_GL_SwapBuffers();

	progress += 0.008;
}

void initOpenGL()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);
	SDL_WM_SetCaption("Bad Sector's OpenGL GUI Demo", "demo");
	SDL_ShowCursor(false);

	SDL_EnableUNICODE(true);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
		SDL_DEFAULT_REPEAT_INTERVAL);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0f, 800.0f/600.0f, 1.0f, 1024.0f);
	glMatrixMode(GL_MODELVIEW);
}

void createUI()
{
	IMG_Init(IMG_INIT_PNG);
	font = new BMFont("droid30.fnt");
	font->Init();
	theme.font = font;
	theme.fontSize = 0.5;
	theme.height = 20;
	theme.scrollSize = 10;
	//Draw::LoadFontData("fontdata.bmp", "fontsize.dat");
	//Draw::LoadCursorImage("cursor.bmp", "cursorAlpha.bmp");
	Draw::SetCursorImage(ImageOpenGL::LoadImage("SoS2.png"));

	win = new Window(Screen::screen, theme, "A fluffy window");
	win->Center();

	screen1 = new Screen(theme);
	screen1->Activate();

 	new Label(win, theme, 5, 0, "A Label");

 	(new Button(win, theme, 10, 25, 180, 50, "Create window"))->clicked =
		new CallbackAction(newWindowAction);
 	new Button(win, theme, 10, 55, 180, 80, "Another button");
 	new Button(win, theme, 10, 85, 180, 110, "Yet another button");

	Window	*win2 = new Window(Screen::screen, theme, "Another fluffy window");
	win2->Center();
	win2->Move(win2->x1 + 200, win2->y1 - 70);
	win2->Resize(win2->x2-win2->x1+1, win2->y2-win2->y1+270);
	win2->movable = false;

	(new Checkbox(win2, theme, 10, 0, 180, 20, "Checker"))->checked = true;
	new Checkbox(win2, theme, 10, 25, 180, 45, "Checker 2");
	new Checkbox(win2, theme, 10, 50, 180, 70, "Checker 3");

	Listbox *lbox = new Listbox(win2, theme, 10, 80, 175, 200);
	lbox->AddItem("A string item");
	for (int i=0;i<20;i++)
	{
		char    buff[100];
		sprintf(buff, "Item %i of 20", i+1);
		lbox->AddItem(buff);
	}
	lbox->name = "listbox";
	Button *b1 = (new Button(win2, theme, 10, 210, 175, 235, "Add wide item"));//
	b1->clicked = new CallbackAction(addWideItemAction);
	b1->visible = true;
	(new Button(win2, theme, 10, 240, 175, 265, "Remove active item"))->clicked =
	        new CallbackAction(removeItemAction);
	(new Passwordbox(win2, theme, 10, 270, 175, 295))->name = "inputbox";
	(new Button(win2, theme, 10, 300, 175, 325, "Add the above text"))->clicked =
	        new CallbackAction(addTheAboveTextAction);

	ProgressBar	*pbar = new ProgressBar(win2, theme, 10, 330, 175, 355);
	pbar->value = 10;
	ProgressBar	*pbar2 = new ProgressBar(win2, theme, 10, 360, 175, 385);
	pbar2->name = "pbar2";
	pbar2->max = 62;

	Window	*win3 = new Window(Screen::screen, theme, "A window");
	win3->Center();
	win3->Move(win3->x1 - 200, win3->y1 - 70);
	win3->Resize(190, 400);

 	new Label(win3, theme, 5, 25, "Object resolution:");
	stepCount = new Slider(win3, theme, 10, 50, 180, 65);
	stepCount->max = 61;
	stepCount->value = 3;
	stepCount->modified = new CallbackAction(stepsModifiedAction);

 	new Label(win3, theme, 5, 80, "Object size:");
	sizeSlider = new Slider(win3, theme, 10, 105, 180, 120);
	sizeSlider->max = 5;
	sizeSlider->value = 30;

 	new Label(win3, theme, 5, 175, "Background intensity:");
	backSlider = new Slider(win3, theme, 10, 200, 180, 215);
	backSlider->max = 1;
	backSlider->value = 45;

 	Scrollbox	*sbox = new Scrollbox(win3, theme, 10, 240, 175, 350);
 	new Button(sbox, theme, 10, 10, 200, 35, "Clipped button");
 	new Button(sbox, theme, 10, 40, 200, 65, "Another clipped button");
 	new Checkbox(sbox, theme, 10, 80, 200, 100, "Clipped checkbox");
 	new Label(sbox, theme, 10, 120, "Clipped label");


	//Window	*win4 = new Window(Screen::screen, "ModelView control");
	//win4->Center();
	//win4->Move(win4->x1, win4->y1 + 160);

	//ModelView *mview = new ModelView(win4, 10, 10, 175, 110);
	//mview->renderView = new CallbackAction(renderObjectAction);

	Window *win5 = new Window(Screen::screen, theme, "Image control");
	win5->Center();
	win5->Move(win5->x1, win5->y1 - 160);
	//Image *cenda = ImageOpenGL::LoadImage("cenda.bmp");
	//cenda->Center();
	//cenda->name = "cenda";
	//cenda->clicked = new CallbackAction(cendaClicked);
	//win5->resizeable = true;
	//win5->resized = new CallbackAction(imageWinResizedAction);
	//win5->icon = new Bitmap(INTERNALBMP_WINICON);
	win5->movable = false;

	(new Button(Screen::screen, theme, 750, 5, 795, 30, "Quit"))->clicked = new CallbackAction(terminateAppAction);




	screen2 = new Screen(theme);
	//Window	*nwin = new Window(screen2, "A window in screen 2");
	//nwin->Center();

	BSGUI::Window *winConnecting = new BSGUI::Window(screen2, theme, "Connecting");
	winConnecting->Resize(200,150);
	winConnecting->Center();
	winConnecting->movable = false;

	new BSGUI::Label(winConnecting, theme, 10, 10, "Connecting");



	(new BSGUI::Button(winConnecting, theme, 30, 80, 170, 110, "Quit"));

 	(new Button(Screen::screen, theme, 5, 5, 105, 30, "Screen 1"))->clicked =
 		new CallbackAction(activateScreen1);
 	(new Button(Screen::screen, theme, 110, 5, 210, 30, "Screen 2"))->clicked =
 		new CallbackAction(activateScreen2);
 	(new Button(screen2, theme, 5, 5, 105, 30, "Screen 1"))->clicked =
 		new CallbackAction(activateScreen1);
 	(new Button(screen2, theme, 110, 5, 210, 30, "Screen 2"))->clicked =
 		new CallbackAction(activateScreen2);

 	fps = new Label(screen1, theme, 5, 40, "?? fps ");
}

void runMe()
{
	SDL_Event	event;
	uint		last = SDL_GetTicks();
	float		frames = 0, tenths = 0;

	while (running)
	{
		frames++;
		if (SDL_GetTicks() - last > 10)
		{
			char	buff[128];
			rotation += 1;
			last = SDL_GetTicks();
			tenths++;
			float seconds = tenths/100.0f;
			sprintf(buff, "%i fps for %i seconds and %i frames",
				(int)(frames/seconds), (int)seconds,
				(int)frames);
			fps->SetText(buff);
		}
		Tick();
		while (SDL_PollEvent(&event))
		{
			if (!HandleSDLEvent(&event))
				switch (event.type)
				{
					case SDL_QUIT:
						running = false;
					break;
				}
		}
		render();
	}
}

int main(int argc, char **argv)
{
	initOpenGL();

	createUI();

	runMe();

	SDL_Quit();

	return 0;
}

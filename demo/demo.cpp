/*
** Bad Sector's OpenGL GUI **DEMO**
*/

#include <bsgui/bsgui.h>
#include <math.h>
//#undef main



using namespace BSGUI;

bool	running = true;
float	rotation = 0.0f, size = 1.0f, backgnd = 0.2f, progress = 0.0f;
int	steps = 5;
Slider	*stepCount, *sizeSlider, *backSlider;
Window	*win;
Screen	*screen1, *screen2;
Label	*fps;

BMFont *font;

Theme themeDefault;
Theme themeRed;

void SetThemeRed(Control *sender)
{
	sender->parent->SetTheme(themeRed);
}

void SetThemeDefault(Control *sender)
{
	sender->parent->SetTheme(themeDefault);
}

void StepsModifiedAction(Control *sender)
{
	ProgressBar	*pbar2 = (ProgressBar*)Screen::screen->FindChild("pbar2");
	steps = (int)stepCount->value + 3;
	pbar2->value = steps - 3;
}

void TerminateAppAction(Control *sender)
{
	running = false;
}


void AddWideItemAction(Control *sender)
{
	Listbox *lbox = (Listbox*)Screen::screen->FindChild("listbox");
	lbox->AddItem("A very wide item to test horizontal scrolling");
	//lbox->Clear();
}

void RemoveItemAction(Control *sender)
{
	Listbox *lbox = (Listbox*)Screen::screen->FindChild("listbox");
	lbox->RemoveItem(lbox->active);
}

void AddTheAboveTextAction(Control *sender)
{
	Listbox *lbox = (Listbox*)Screen::screen->FindChild("listbox");
	lbox->AddItem(((Inputbox*)Screen::screen->FindChild("inputbox"))->text.c_str());
	((Inputbox*)Screen::screen->FindChild("inputbox"))->SetText("");
	Screen::screen->FindChild("inputbox")->Focus();
}

void ActivateScreen1(Control *sender)
{
	screen1->Activate();
}

void ActivateScreen2(Control *sender)
{
	screen2->Activate();
}

void RenderObject()
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

void RenderObjectAction(Control *sender)
{
	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(rotation, 0.0f, 1.0f, 0.0f);
	RenderObject();
}

void FileSelectedAction(Control *sender)
{
	//const char *fname = ((FileBrowser*)sender)->GetFilename().GetAscii();
	//printf("File selected: %s\n", fname);
}

void SelectFileAction(Control *sender)
{
	FileBrowser	*browser = new FileBrowser(themeDefault, Screen::screen, "Select File");
	browser->actionSelected = FileSelectedAction;
}

void AboutBoxAction(Control *sender)
{
	MessageBox(sender->theme, "BSGUI demo", "About");
}

void ImageWinResizedAction(Control *sender)
{
	Picture *img = (Picture*)sender->FindChild("bob");
	int	w, h;
	sender->GetClientSize(w, h);
	img->Place(10, 10, w - 10, h - 30);
}

void EnableFiltering(Control *sender)
{
	Image	*img = (Image*)Screen::screen->FindChild("cenda");
	//img->bitmap->SetFiltering(true);
}

void DisableFiltering(Control *sender)
{
	Image	*img = (Image*)Screen::screen->FindChild("cenda");
	//img->bitmap->SetFiltering(false);
}

void CendaClicked(Control *sender)
{
	SelectFileAction(sender);
}

void Render()
{
	//RenderOffscreenBSGUIControls();

	glClearColor((146.0f/255.0f)*backgnd, (139.0f/255.0f)*backgnd,
		(125.0f/255.0f)*backgnd, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -5.0f);
	glRotatef(rotation, 1.0f, 0.0f, 0.0f);
	glRotatef(rotation/0.6f, 0.0f, 1.0f, 0.0f);

	RenderObject();

 	BSGUI::Render();

  	SDL_GL_SwapBuffers();

	progress += 0.008;
}

void InitOpenGL()
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

void CreateUI()
{
	IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
	font = new BMFont("font.fnt");
	font->Init();
	themeDefault.font = font;
	themeDefault.fontSize = 0.5;
	themeDefault.height = 20;
	themeDefault.scrollSize = 10;
	themeRed.font = font;
	themeRed.fontSize = 0.7;
	themeRed.height = 35;
	themeRed.scrollSize = 15;
	themeRed.colorFrameFocused.Set(0.7, 0.3, 0.2, 0.5);
	themeRed.colorFrameHovered.Set(0.8, 0.2, 0.3, 0.5);
	themeRed.colorFrameNormal.Set(0.65, 0.3, 0.3, 0.5);
	Draw::SetCursorImage(ImageOpenGL::LoadImage("cursor.png"));
	Draw::SetCursorAlign(IF_ALIGN_CENTER);

	screen1 = new Screen(themeDefault);


	win = new Window(screen1, themeDefault, "Unmovable window");
	win->Center();
	win->movable = false;
 	new Label(win, themeDefault, 5, 0, "A Label");
 	(new Button(win, themeDefault, 10, 25, 180, 50, "About"))->actionPressed = AboutBoxAction;
 	(new Button(win, themeDefault, 10, 55, 180, 80, "Use red theme"))->actionPressed = SetThemeRed;
 	(new Button(win, themeDefault, 10, 85, 180, 110, "Use default theme"))->actionPressed = SetThemeDefault;

	Window *win11 = new Window(screen1, themeRed, "Unicode test window");
	win11->movable = false;
	win11->Move(0, 400);
	win11->Center(true, false);
 	new Label(win11, themeRed, 5, 0, L"Русский язык");
 	new Label(win11, themeRed, 5, 50, L"Қазақ тілі");



	Window	*win2 = new Window(screen1, themeDefault, "Fluffy window");
	win2->Center();
	win2->Move(win2->x1 + 200, win2->y1 - 70);
	win2->Resize(win2->x2-win2->x1+1, win2->y2-win2->y1+270);
	win2->movable = true;

	(new Checkbox(win2, themeDefault, 10, 0, 180, 20, "Checker"))->checked = true;
	new Checkbox(win2, themeDefault, 10, 25, 180, 45, "Checker 2");
	new Checkbox(win2, themeDefault, 10, 50, 180, 70, "Checker 3");

	Listbox *lbox = new Listbox(win2, themeDefault, 10, 80, 175, 200);
	lbox->AddItem("A string item");
	for (int i=0;i<20;i++)
	{
		char    buff[100];
		sprintf(buff, "Item %i of 20", i+1);
		lbox->AddItem(buff);
	}
	lbox->name = "listbox";
	Button *b1 = (new Button(win2, themeDefault, 10, 210, 175, 235, "Add wide item"));//
	b1->actionPressed = AddWideItemAction;
	b1->visible = true;
	(new Button(win2, themeDefault, 10, 240, 175, 265, "Remove active item"))->actionPressed = RemoveItemAction;
	Inputbox *inpBox11 = new Inputbox(win2, themeDefault, 10, 270, 175, 295);
	inpBox11->name = "inputbox";
	inpBox11->actionPressed = AddTheAboveTextAction;
	(new Button(win2, themeDefault, 10, 300, 175, 325, "Add the above text"))->actionPressed = AddTheAboveTextAction;

	ProgressBar	*pbar = new ProgressBar(win2, themeDefault, 10, 330, 175, 355);
	pbar->value = 10;
	ProgressBar	*pbar2 = new ProgressBar(win2, themeDefault, 10, 360, 175, 385);
	pbar2->name = "pbar2";
	pbar2->max = 62;

	Window	*win3 = new Window(screen1, themeDefault, "A window");
	win3->Center();
	win3->Move(win3->x1 - 200, win3->y1 - 70);
	win3->Resize(190, 400);

 	new Label(win3, themeDefault, 5, 25, "Object resolution:");
	stepCount = new Slider(win3, themeDefault, 10, 50, 180, 65);
	stepCount->max = 61;
	stepCount->value = 3;
	stepCount->actionModified = StepsModifiedAction;

 	new Label(win3, themeDefault, 5, 80, "Object size:");
	sizeSlider = new Slider(win3, themeDefault, 10, 105, 180, 120);
	sizeSlider->max = 5;
	sizeSlider->value = 30;

 	new Label(win3, themeDefault, 5, 175, "Background intensity:");
	backSlider = new Slider(win3, themeDefault, 10, 200, 180, 215);
	backSlider->max = 1;
	backSlider->value = 45;

 	Scrollbox	*sbox = new Scrollbox(win3, themeDefault, 10, 240, 175, 350);
 	new Button(sbox, themeDefault, 10, 10, 200, 35, "Clipped button");
 	new Button(sbox, themeDefault, 10, 40, 200, 65, "Another clipped button");
 	new Checkbox(sbox, themeDefault, 10, 80, 200, 100, "Clipped checkbox");
 	new Label(sbox, themeDefault, 10, 120, "Clipped label");


	//Window	*win4 = new Window(Screen::screen, "ModelView control");
	//win4->Center();
	//win4->Move(win4->x1, win4->y1 + 160);

	//ModelView *mview = new ModelView(win4, 10, 10, 175, 110);
	//mview->renderView = new CallbackAction(renderObjectAction);

	Window *win5 = new Window(screen1, themeDefault, "Resizeable window");
	win5->Center();
	win5->Move(win5->x1, win5->y1 - 160);
	Image *img = ImageOpenGL::LoadImage("spongebob.jpg");
	img->GetHeight();
	int width, height;
	win5->GetClientSize(width, height);
	win5->resizeable = true;
	win5->actionResized = ImageWinResizedAction;
	Picture *bob = new Picture(win5, themeDefault, 10,10, width-10, height-30, img);
	bob->name = "bob";

	(new Button(Screen::screen, themeDefault, 750, 5, 795, 30, "Quit"))->actionPressed = TerminateAppAction;




	screen2 = new Screen(themeDefault);
	//Window	*nwin = new Window(screen2, "A window in screen 2");
	//nwin->Center();

	BSGUI::Window *winConnecting = new BSGUI::Window(screen2, themeDefault, "Connecting");
	winConnecting->Resize(200,150);
	winConnecting->Center();
	winConnecting->movable = false;

	new BSGUI::Label(winConnecting, themeDefault, 10, 10, "Connecting");



	(new BSGUI::Button(winConnecting, themeDefault, 30, 80, 170, 110, "Quit"));

 	(new Button(Screen::screen, themeDefault, 5, 5, 105, 30, "Screen 1"))->actionPressed = ActivateScreen1;
 	(new Button(Screen::screen, themeDefault, 110, 5, 210, 30, "Screen 2"))->actionPressed = ActivateScreen2;
 	(new Button(screen2, themeDefault, 5, 5, 105, 30, "Screen 1"))->actionPressed = ActivateScreen1;
 	(new Button(screen2, themeDefault, 110, 5, 210, 30, "Screen 2"))->actionPressed = ActivateScreen2;

 	fps = new Label(screen1, themeDefault, 5, 40, "?? fps ");

 	screen1->Activate();
}

void RunMe()
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
		::Render();
	}
}

int main(int argc, char **argv)
{
	InitOpenGL();

	CreateUI();

	RunMe();

	SDL_Quit();

	return 0;
}

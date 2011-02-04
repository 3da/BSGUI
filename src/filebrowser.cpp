/*
** Bad Sector's OpenGL GUI
*/

#include <dirent.h>
//#include <bsgui/bsgui.h>
#include "bsgui/filebrowser.h"
#include "bsgui/inputboxes.h"
#include "bsgui/actions.h"
#include "bsgui/listboxes.h"
#include "bsgui/labels.h"
#include "bsgui/buttons.h"
#include "bsgui/controls.h"

#include <cstring>
#include <cstdlib>
#include <cstdio>

namespace BSGUI
{

static void CancelFileBrowserAction(Control *sender)
{
	delete sender->parent;
}

static void CloseFileBrowserAction(Control *sender)
{
	FileBrowser	*browser = (FileBrowser*)sender->parent;
	if (browser->filename->text[0] == '/')
	{
		if (!strcmp(browser->filename->text.c_str(), "/."))
		{
			browser->ReloadFiles();
			return;
		}
		std::string newPath = browser->path;
		newPath+='/';
		newPath+=browser->filename->text;
		browser->path = newPath;
		browser->ReloadFiles();
		return;
	}
	Control::RunActionOf(browser, browser->selected);
	delete browser;
}

static void SelectFileBrowserAction(Control *sender)
{
	FileBrowser	*browser = (FileBrowser*)sender->parent;
	browser->filename->text = browser->files->active->text;
}

FileBrowser::FileBrowser(Theme &t, Screen *screen, char *caption, char *path)
	: Window(screen, t, caption)
{
	int	w, h;
	Label	*l;
	Button	*b;
	Resize(500, 300);
	Center();
	if (path)
		this->path = strdup(path);
	else
		this->path = strdup(".");

	GetClientSize(w,h);

	l = new Label(this, t, 5, 5, "Files and directories:");
	files = new Listbox(this, t, 5, l->y2 + 5, w-10, h-40);
	files->modified = new CallbackAction(SelectFileBrowserAction);
	l = new Label(this, t, 5, files->y2+9, "Filename:");
	filename = new Inputbox(this, t, l->x2 + 5, files->y2 + 5, w-130, files->y2 + 30);
	b = new Button(this, t, w-125, files->y2 + 5, w-70, files->y2 + 30, "Ok");
	b->clicked = new CallbackAction(CloseFileBrowserAction);
	b = new Button(this, t, w-65, files->y2 + 5, w-10, files->y2+30, "Cancel");
	b->clicked = new CallbackAction(CancelFileBrowserAction);

	ReloadFiles();

	MakeExclusive();
}

FileBrowser::~FileBrowser()
{
}

const char *FileBrowser::GetFilename()
{
	std::string	fname;
	fname = path;
	fname+=filename->text;
	return fname.c_str();
}

void FileBrowser::ReloadFiles()
{
	struct dirent	*dent;
	DIR		*dir;
	files->RemoveAllChildren();

	dir = opendir(path.c_str());
	while ((dent = readdir(dir)))
	{
		DIR	*dir2 = opendir(dent->d_name);
		if (dir2)
		{
			char	*buff = (char*)malloc(strlen(dent->d_name)+2);
			closedir(dir2);
			sprintf(buff, "/%s", dent->d_name);
			files->AddItem(buff);
			free(buff);
		}
		else
			files->AddItem(dent->d_name);
	}
	closedir(dir);
	files->Layout();

	files->SelectItem((unsigned int)0);

	if (files->active)
		filename->text = files->active->text;
	else
		filename->text = "";
}

}

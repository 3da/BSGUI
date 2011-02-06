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
#ifdef UNICODE
		if (!wcscmp(browser->filename->text.c_str(), L"/."))
#else
		if (!strcmp(browser->filename->text.c_str(), "/."))
#endif
		{
			browser->ReloadFiles();
			return;
		}
		str_t newPath = browser->path;
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

FileBrowser::FileBrowser(Theme &t, Screen *screen, const ch_t *caption, const ch_t *path)
	: Window(screen, t, caption)
{
	int	w, h;
	Label	*l;
	Button	*b;
	Resize(500, 300);
	Center();
	if (path)
		this->path = path;
	else
#ifdef UNICODE
		this->path = L".";
#else
		this->path = ".";
#endif

	GetClientSize(w,h);

#ifdef UNICODE
	l = new Label(this, t, 5, 5, L"Files and directories:");
#else
	l = new Label(this, t, 5, 5, "Files and directories:");
#endif
	files = new Listbox(this, t, 5, l->y2 + 5, w-10, h-40);
	files->modified = new CallbackAction(SelectFileBrowserAction);
#ifdef UNICODE
	l = new Label(this, t, 5, files->y2+9, L"Filename:");
#else
	l = new Label(this, t, 5, files->y2+9, "Filename:");
#endif
	filename = new Inputbox(this, t, l->x2 + 5, files->y2 + 5, w-130, files->y2 + 30);
#ifdef UNICODE
	b = new Button(this, t, w-125, files->y2 + 5, w-70, files->y2 + 30, L"Ok");
#else
	b = new Button(this, t, w-125, files->y2 + 5, w-70, files->y2 + 30, "Ok");
#endif
	b->clicked = new CallbackAction(CloseFileBrowserAction);
#ifdef UNICODE
	b = new Button(this, t, w-65, files->y2 + 5, w-10, files->y2+30, L"Cancel");
#else
	b = new Button(this, t, w-65, files->y2 + 5, w-10, files->y2+30, "Cancel");
#endif
	b->clicked = new CallbackAction(CancelFileBrowserAction);

	ReloadFiles();

	MakeExclusive();
}

FileBrowser::~FileBrowser()
{
}

const ch_t *FileBrowser::GetFilename()
{
	str_t fname;
	fname = path;
	fname+=filename->text;
	return fname.c_str();
}

void FileBrowser::ReloadFiles()
{
	struct dirent	*dent;
	DIR		*dir;
	files->RemoveAllChildren();

#ifdef UNICODE
	char p[256];
	wcstombs(p, path.c_str(), 256);
#else
	const char *p = path.c_str();
#endif
	dir = opendir(p);
	while ((dent = readdir(dir)))
	{
		DIR	*dir2 = opendir(dent->d_name);
		if (dir2)
		{
			ch_t	*buff = (ch_t*)malloc(strlen(dent->d_name)+2);
			closedir(dir2);
#ifdef UNICODE
			swprintf(buff, L"/%s", dent->d_name);
#else
			sprintf(buff, "/%s", dent->d_name);
#endif
			files->AddItem(buff);
			free(buff);
		}
		else
		{
#ifdef UNICODE
			wchar_t d_name[256];
			mbstowcs(d_name, dent->d_name, 256);
			files->AddItem(d_name);
#else
			files->AddItem(dent->d_name);
#endif
		}
	}
	closedir(dir);
	files->Layout();

	files->SelectItem((unsigned int)0);

	if (files->active)
		filename->text = files->active->text;
	else
#ifdef UNICODE
		filename->text = L"";
#else
		filename->text = "";
#endif
}

}

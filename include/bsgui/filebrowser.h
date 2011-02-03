/*
** Slashy2D Editor
*/

#ifndef __BSGUI_FILEBROWSER_H_INCLUDED__
#define __BSGUI_FILEBROWSER_H_INCLUDED__

#include "bsgui/windows.h"

namespace BSGUI
{

struct FileBrowser : public Window
{
	class Listbox *files;
	class Inputbox *filename;
	MyString path;

	CallbackActionFunc actionSelected;

	FileBrowser(Theme &t, Screen *screen, const MyString caption, const MyString path=MyString());
	virtual ~FileBrowser();

	virtual const MyString GetFilename();
	virtual void ReloadFiles();
};

}

#endif

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
	std::string path;

	FileBrowser(Theme &t, Screen *screen, char *caption, char *path=NULL);
	virtual ~FileBrowser();

	virtual const char *GetFilename();
	virtual void ReloadFiles();
};

}

#endif

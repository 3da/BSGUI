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
	str_t path;

	FileBrowser(Theme &t, Screen *screen, const ch_t *caption, const ch_t *path=NULL);
	virtual ~FileBrowser();

	virtual const ch_t *GetFilename();
	virtual void ReloadFiles();
};

}

#endif

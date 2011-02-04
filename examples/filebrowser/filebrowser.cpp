/*
** BSGUI file browser demo
*/

#include <bsgui/bsgui.h>

void error(char *msg)
{
	fprintf(stderr, "error: %s\n", msg);
	exit(1);
}

void fileSelected(Control *sender)
{
	char	*fileName = ((FileBrowser*)sender)->getFilename();
	printf("File selected: %s\n", fileName);
	free(fileName);
}

void openFile(Control *sender)
{
	FileBrowser	*browser = new FileBrowser("Open file");
	browser->selected = new CallbackAction(fileSelected);
}

void createUI()
{
	PopupMenu	*menu = new PopupMenu;
	MenuItem	*file = menu->addItem("File", NULL);
	file->addItem("New", NULL, new Bitmap(INTERNALBMP_NEW));
	file->addItem("Open", new CallbackAction(openFile),
		new Bitmap(INTERNALBMP_OPEN));
	file->addItem("Save", NULL, new Bitmap(INTERNALBMP_SAVE));
	file->addItem("Save as", NULL, new Bitmap(INTERNALBMP_SAVEAS));
	menu->addItem("-", NULL);
	menu->addItem("Quit", terminateBSGUIApplicationAction,
		new Bitmap(INTERNALBMP_QUIT));
	
	screen->popupMenu = menu;
	
	messageBox("Select File->Open to show the browser", "Message", false);
}

int main()
{
	if (!initBSGUIApplication())
		error("Cannot initialize BSGUI");

	createUI();
	runBSGUIApplication();
		
	shutdownBSGUIApplication();
}

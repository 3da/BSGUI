/*
** BSGUI menus demo
*/

#include <bsgui/bsgui.h>

void error(char *msg)
{
	fprintf(stderr, "error: %s\n", msg);
	exit(1);
}

void item1Action(Control *sender)
{
	printf("action 1 clicked\n");
}

void item2Action(Control *sender)
{
	printf("action 2 clicked\n");
}

void item3Action(Control *sender)
{
	printf("action 3 clicked\n");
}

void createUI()
{
	PopupMenu	*menu = new PopupMenu;
	MenuItem	*file = menu->addItem("File", NULL);
	file->addItem("New", NULL, new Bitmap(INTERNALBMP_NEW));
	file->addItem("Open", NULL, new Bitmap(INTERNALBMP_OPEN));
	file->addItem("Save", NULL, new Bitmap(INTERNALBMP_SAVE));
	file->addItem("Save as", NULL, new Bitmap(INTERNALBMP_SAVEAS));
	MenuItem	*items = menu->addItem("Items", NULL);
	items->addItem("Item 1", new CallbackAction(item1Action));
	items->addItem("Item 2", new CallbackAction(item2Action));
	items->addItem("Item 3", new CallbackAction(item3Action));
	items->addItem("Item that does nothing", NULL);
	menu->addItem("-", NULL);
	menu->addItem("Quit", terminateBSGUIApplicationAction,
		new Bitmap(INTERNALBMP_QUIT));
	
	screen->popupMenu = menu;
	
	messageBox("Right click on the desktop to show the menu", "Message",
		false);
}

int main()
{
	if (!initBSGUIApplication())
		error("Cannot initialize BSGUI");

	createUI();
	runBSGUIApplication();
		
	shutdownBSGUIApplication();
}

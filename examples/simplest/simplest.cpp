/*
** Simplest BSGUI usage
*/

#include <bsgui/bsgui.h>

void error(char *msg)
{
	fprintf(stderr, "error: %s\n", msg);
	exit(1);
}

int main()
{
	if (!initBSGUIApplication())
		error("Cannot initialize BSGUI");
	
	runBSGUIApplication();
		
	shutdownBSGUIApplication();
}

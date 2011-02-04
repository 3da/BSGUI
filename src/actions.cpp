/*
** Bad Sector's OpenGL GUI
*/

//#include <bsgui/bsgui.h>
#include "bsgui/actions.h"

namespace BSGUI
{

Action::Action()
{
	autoDelete = true;
}

Action::~Action()
{
}


CallbackAction::CallbackAction(CallbackActionFunc cbFunc)
{
	func = cbFunc;
}

void CallbackAction::Run(Control *sender)
{
	if (func)
		func(sender);
}

}

/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_MESSAGEBOXES_H_INCLUDED__
#define __BSGUI_MESSAGEBOXES_H_INCLUDED__

#include "bsgui/theme.h"

namespace BSGUI
{

void MessageBox(Theme &t, char *msg, char *caption="Message", bool exclusive=true);

}

#endif

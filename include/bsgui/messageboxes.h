/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_MESSAGEBOXES_H_INCLUDED__
#define __BSGUI_MESSAGEBOXES_H_INCLUDED__

#include "bsgui/theme.h"

#include "bsgui/mystring.h"


#undef MessageBox

namespace BSGUI
{

void MessageBox(
				Theme &t,
				const MyString msg,
				const MyString caption=L"Message",
				bool exclusive=true
				);

}

#endif

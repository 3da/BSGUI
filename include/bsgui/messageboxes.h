/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_MESSAGEBOXES_H_INCLUDED__
#define __BSGUI_MESSAGEBOXES_H_INCLUDED__

#include "bsgui/theme.h"

namespace BSGUI
{

void MessageBox(
				Theme &t,
				const ch_t *msg,
#ifdef UNICODE
				const ch_t *caption=L"Message",
#else
				const ch_t *caption="Message",
#endif
				bool exclusive=true
				);

}

#endif

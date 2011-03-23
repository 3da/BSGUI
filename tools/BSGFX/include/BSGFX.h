#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "Drawing.h"
#include "Screen.h"
#include "Videomode.h"
#include "Texture.h"
#include "Alignment.h"
#include "Font.h"

namespace BSGFX
{
	enum IMAGE_TYPES
	{
		IT_NONE = 0,
		IT_JPG = (1<<0),
		IT_PNG = (1<<1),
		IT_TIF = (1<<2),

	};

	bool Initialize(unsigned long useImageTypes);





}

#endif // MAIN_H_INCLUDED

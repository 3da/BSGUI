#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "BSGFX/Drawing.h"
#include "BSGFX/Screen.h"
#include "BSGFX/Videomode.h"
#include "BSGFX/Texture.h"
#include "BSGFX/Alignment.h"
#include "BSGFX/Font.h"

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

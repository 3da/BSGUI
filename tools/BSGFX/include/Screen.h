#ifndef SCREEN_H_INCLUDED
#define SCREEN_H_INCLUDED

#include "Videomode.h"

#include <string>

namespace BSGFX
{

class Screen
{
public:
	static bool Initialize(unsigned int width, unsigned int height,
							unsigned int depth,
							bool fullscreen,
							const char *caption);

	static const Videomode GetVideomode() {return videomode;}

protected:
	static bool SetupOpengl(unsigned int width, unsigned int height);

	static std::string caption;

	static Videomode videomode;

};


}


#endif // SCREEN_H_INCLUDED

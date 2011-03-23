#ifndef VIDEOMODE_H_INCLUDED
#define VIDEOMODE_H_INCLUDED



namespace BSGFX
{
class Videomode
{
public:
	Videomode(unsigned int w, unsigned int h, unsigned int d, bool fs)
	{
		width = w;
		height = h;
		depth = d;
		fullscreen = fs;
	}
	Videomode()
	{
		width = 0;
		height = 0;
		depth = 0;
		fullscreen = false;
	}

	bool Initialize(const char *caption);

	unsigned int GetWidth() const {return width;}
	unsigned int GetHeight() const {return height;}
	unsigned int GetDepth() const {return depth;}
	bool IsFullscreen() const {return fullscreen;}


protected:
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	bool fullscreen;


};




}

#endif // VIDEOMODE_H_INCLUDED

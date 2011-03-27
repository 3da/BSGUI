#ifndef BSGFX_H_INCLUDED
#define BSGFX_H_INCLUDED

#include "Texture.h"
#include "Alignment.h"


namespace BSGFX
{
class Drawing
{
public:

	static void Line(int x1, int y1, int x2, int y2, float width = 1.0);
	static void Rect(int x, int y, int width, int height, unsigned long flags = A_TOP_LEFT, float rotation = 0, const Texture *texture = 0, const Rectangle *sourceRect = 0);
	static void Sprite(int x, int y, Texture *texture, float scaleX = 1, float scaleY = 1, float rotation = 0, unsigned long flags = A_TOP_LEFT, const Rectangle *sourceRect = 0);
	static void Sprite();
	static void CircleFilled(int x, int y, int radius/*, Texture *texture = 0*/);
	static void Circle(int x, int y, int radius, float lineWidth);

	static void Flush();
	static void Color(float r, float g, float b, float a = 1.0);
	static void ClearColor(float r, float g, float b);

	static void InitializeCircle(unsigned int segments);

private:
	static unsigned long circleList[2];

	static Texture *spriteLogo;



};

}

#endif // BSGFX_H_INCLUDED

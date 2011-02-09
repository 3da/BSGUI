/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_DRAW_H_INCLUDED__
#define __BSGUI_DRAW_H_INCLUDED__

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB	0x84F5
#endif

namespace BSGUI
{
class Image;
class Font;
namespace Draw
{
	bool SetCursorImage(Image *image);
	void DrawCursor(int x, int y);
	void SetCursorAlign(unsigned long flags);

	//bool SetBaseFont(Font *font);

	void SetClipArea(int x1, int y1, int x2, int y2);

	void ResetClipping();
	void OpenClipArea(int x1, int y1, int x2, int y2);
	void CloseClipArea();
}

}

#endif

#ifndef THEME_H_INCLUDED
#define THEME_H_INCLUDED

#include "bsgui/image.h"
#include "bsgui/mystring.h"

namespace BSGUI
{

struct Theme
{

	Theme();

	void DrawString(int x, int y, const MyString str,  bool center);
	int StringWidth(const MyString str);

	void DrawRect(int x1, int y1, int x2, int y2);
	void FillRect(int x1, int y1, int x2, int y2);
	void DrawLine(int x1, int y1, int x2, int y2);
	void DrawFrame(int x1, int y1, int x2, int y2);


	class Font *font;
	float fontSize;

	//int padLeft;
	//int padTop;
	//int padRight;
	//int padBottom;
	int height;
	int scrollSize;
	Color colorTextFocused;
	Color colorTextUnfocused;
	Color colorFrameFocused;
	Color colorFrameHovered;
	Color colorFrameNormal;
	Color colorLines;
	Color colorTitleFocused;
	Color colorTitleUnfocused;
	Color colorTitleText;



};

}

#endif // THEME_H_INCLUDED

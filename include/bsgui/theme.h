#ifndef THEME_H_INCLUDED
#define THEME_H_INCLUDED

#include <BSGFX.h>

#include "bsgui/mystring.h"

namespace BSGUI
{
struct Color
{
	Color()
	{
		Set(1, 0.5, 0 , 1);
	}

	Color(float r, float g, float b, float a = 1)
	{
		Set(r, g, b, a);
	}

	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float c[4];
	};

	void Use()
	{
		BSGFX::Drawing::Color(r, g, b, a);
	}

	void Set(float r_, float g_, float b_, float a_ = 1)
	{
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

};

struct Theme
{

	Theme();

	void DrawString(int x, int y, const MyString str,  bool center);
	int StringWidth(const MyString str);

	void DrawRect(int x1, int y1, int x2, int y2);
	void FillRect(int x1, int y1, int x2, int y2);
	void DrawLine(int x1, int y1, int x2, int y2);
	void DrawFrame(int x1, int y1, int x2, int y2);


	BSGFX::Font *font;
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

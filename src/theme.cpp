#include "bsgui/theme.h"

#include "bsgui/font.h"

namespace BSGUI
{

Theme::Theme()
{
	font=0;
	fontSize=1.0;
	//padLeft=0;
	//padTop=0;
	//padRight=0;
	//padBottom=0;
	height=15;
	scrollSize=10;
	colorFrameNormal.Set(0.45, 0.45, 0.45, 0.5);
	colorTextFocused.Set(0.75, 0.85, 0.60, 0.7);
	colorTextUnfocused.Set(0.75, 0.75, 0.75, 0.5);
	colorFrameFocused.Set(0.6, 0.6, 0.6, 0.5);
	colorFrameHovered.Set(0.3, 0.3, 0.3, 0.5);
	colorLines.Set(1.0,1.0,1.0, 0.5);
	colorTitleUnfocused.Set(0.7,0.8,0.8, 0.7);
	colorTitleFocused.Set(0.7, 0.8, 0.9, 0.7);
	colorTitleText.Set(0.9,0.9,0.9, 0.5);
}


void Theme::DrawString(int x, int y, const MyString str, bool center)
{
	if (font)
		font->DrawString(x,y,str.c_str(),(center)?IF_ALIGN_CENTER:IF_ALIGN_VERT_CENTER,fontSize);
}

int Theme::StringWidth(const MyString str)
{
	if (font)
		return font->GetStringWidth(str.c_str(), fontSize);
	else
		return 0;
}

void Theme::DrawRect(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINE_STRIP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glVertex2f(x1, y1);
	glEnd();
}

void Theme::FillRect(int x1, int y1, int x2, int y2)
{
	glBegin(GL_QUADS);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
}


void Theme::DrawLine(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void Theme::DrawFrame(int x1, int y1, int x2, int y2)
{
	FillRect(x1, y1, x2, y2);
	colorLines.Use();
	DrawRect(x1, y1, x2, y2);
}


}




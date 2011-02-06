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
	colorFrameNormal=Color(0.45, 0.45, 0.45, 0.5);
	colorTextFocused=Color(0.75, 0.85, 0.60, 0.7);
	colorTextUnfocused=Color(0.75, 0.75, 0.75, 0.5);
	colorFrameFocused=Color(0.6, 0.6, 0.6, 0.5);
	colorFrameHovered=Color(0.3, 0.3, 0.3, 0.5);
	colorLines=Color(1.0,1.0,1.0, 0.5);
	colorTitle=Color(0.6,0.5,0.6, 0.7);
	colorTitleText=Color(0.9,0.9,0.9, 0.5);
}


void Theme::DrawString(int x, int y, const ch_t *str, bool center)
{
	if (font)
		font->DrawString(x,y,str,(center)?IF_ALIGN_CENTER:IF_ALIGN_VERT_CENTER,fontSize);
}

int Theme::StringWidth(const ch_t *str)
{
	if (font)
		return font->GetStringWidth(str, fontSize);
	else
		return 0;
}

void Theme::DrawRect(int x1, int y1, int x2, int y2)
{
	//if (focused)
	//	colorFrameFocused.Use();
	//colorLines.Use();
	//glBindTexture(GL_TEXTURE_2D, 0);
	y1++;
	y2++;
	glBegin(GL_LINE_STRIP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glVertex2f(x1, y1);
	glEnd();
	glBegin(GL_POINTS);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
}

void Theme::FillRect(int x1, int y1, int x2, int y2)
{
	//colorFrameNormal.Use();
	//glBindTexture(GL_TEXTURE_2D, 0);
	x1++;
	x2++;
	glBegin(GL_QUADS);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
}


void Theme::DrawLine(int x1, int y1, int x2, int y2)
{
	//glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void Theme::DrawFrame(int x1, int y1, int x2, int y2)
{
	//glBindTexture(GL_TEXTURE_2D, 0);
	//colorFrameNormal.Use();
	FillRect(x1, y1, x2, y2);
	colorLines.Use();
	DrawRect(x1, y1, x2, y2);

}


}




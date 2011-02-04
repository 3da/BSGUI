/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_IMAGES_H_INCLUDED__
#define __BSGUI_IMAGES_H_INCLUDED__

#define INTERNALBMP_NEW		((char*)-1)
#define INTERNALBMP_OPEN	((char*)-2)
#define INTERNALBMP_SAVE	((char*)-3)
#define INTERNALBMP_SAVEAS	((char*)-4)
#define INTERNALBMP_QUIT	((char*)-5)
#define INTERNALBMP_WINICON	((char*)-6)

#include "bsgui/controls.h"

#include "GL\GL.h"

namespace BSGUI
{

struct Bitmap
{
	GLuint	name;
	GLuint	width;
	GLuint	height;
	GLuint	target;
	bool	filtering;

	Bitmap(char *fname);
	~Bitmap();

	void Bind();
	void Unbind();
	void Draw(int x, int y);
	void Draw(int x1, int y1, int x2, int y2);
	void SetFiltering(bool enabled);
};

struct Image : public Control
{
	Bitmap	*bitmap;
	bool	autoDeleteBitmap;

	Image(Control *parent, int x, int y, char *fname);
	virtual ~Image();

	virtual void SetBitmap(Bitmap *newBitmap, bool resize=true, bool autoDelete=true);
	virtual void SetBitmap(char *fname, bool resize=true);

	virtual void Render();
};

}

#endif

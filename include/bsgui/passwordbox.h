#ifndef PASSWORDBOX_H_INCLUDED
#define PASSWORDBOX_H_INCLUDED


#include "bsgui/controls.h"

namespace BSGUI
{

struct Passwordbox : public Control
{
	int tScroll;
	int cursor;
	int cursorX;

	std::string text;
	std::string password;


	Passwordbox(Control *parent, Theme &t, int x1, int y1, int x2, int y2);
	virtual ~Passwordbox();

	void SetText(const char *text);
	const char *GetText();


	virtual void Render();

	virtual bool OnKeyDown(int key, unsigned char ascii);
	virtual bool OnKeyUp(int key, unsigned char ascii);

	protected:


};

}


#endif // PASSWORDBOX_H_INCLUDED

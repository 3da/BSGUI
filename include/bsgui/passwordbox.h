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

	str_t text;
	str_t password;


	Passwordbox(Control *parent, Theme &t, int x1, int y1, int x2, int y2);
	virtual ~Passwordbox();

	void SetText(const ch_t *text);
	const ch_t *GetText();


	virtual void Render();

	virtual bool OnKeyDown(int key, unsigned char code);
	virtual bool OnKeyUp(int key, unsigned char code);

	protected:


};

}


#endif // PASSWORDBOX_H_INCLUDED

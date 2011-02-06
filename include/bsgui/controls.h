/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_CONTROLS_H_INCLUDED__
#define __BSGUI_CONTROLS_H_INCLUDED__

#include "config.h"
#include <vector>
#include "bsgui/actions.h"
#include "bsgui/theme.h"

namespace BSGUI
{

struct Control
{
	Control *parent;
	std::vector<Control*> childs;
	Control *childFocus;

	Control *exclusiveChild;

	std::string name;
	int xShift;
	int yShift;
	int x1;
	int y1;
	int x2;
	int y2;

	int minWidth;
	int minHeight;
	int maxWidth;
	int maxHeight;
	bool performLayout;
	bool mouseOverControl;

	bool visible;

	Theme theme;

	struct Action *clicked;
	struct Action *modified;
	struct Action *selected;
	struct Action *moved;
	struct Action *resized;

	struct PopupMenu *popupMenu;

	Control(Control *parent, Theme &theme);
	virtual ~Control();

	virtual Control *FindChild(char *name);

	virtual void AddChild(Control *child);
	virtual void RemoveChild(Control *child);
	virtual void RemoveAllChildren();
	virtual void Layout() {};

	virtual Control *ChildAt(int x, int y);

	struct Screen *GetScreen();

	virtual void MakeExclusive();

	virtual void Tick();
	virtual void Render();
	virtual void Focus();
	virtual bool Focused();
	virtual void Place(int x1, int y1, int x2, int y2);
	virtual void Move(int x, int y);
	virtual void Resize(int width, int height);
	virtual void Center(bool horizontal = true, bool vertical = true);
	virtual void Translate(int &x, int &y);
	virtual void GetBounds(int &x1, int &y1, int &x2, int &y2);
	//virtual void GetChildrenBounds(int &x1, int &y1, int &x2, int &y2);
	virtual void GetBoundsChildren(int &x1, int &y1, int &x2, int &y2);
	//virtual void GetBoundsChildren(int &x1, int &y1, int &x2, int &y2);
	virtual void SetTheme(Theme &t);
	virtual void GetClientSize(int &w, int &h);

	// drawing helpers
	virtual void OpenClip();
	virtual void CloseClip();
	virtual void DrawOwnFrame();
	virtual void DrawOwnFocus();
	// mouse events
	virtual void OnMouseEnter();
	virtual bool OnMouseMoved(int x, int y);
	virtual void OnMouseExit();
	virtual bool OnMouseDown(int x, int y, int b);
	virtual bool OnMouseUp(int x, int y, int b);
	// keyboard events
	virtual bool OnKeyDown(int key, unsigned ch_t code);
	virtual bool OnKeyUp(int key, unsigned ch_t code);
	// focus events
	virtual void OnFocusEnter() {};
	virtual void OnFocusExit() {};

	void SetMouseTracking();
	static void ClearMouseTracking();
	void Invalidate();
	void SetKeyboardFocus();
	bool HasKeyboardFocus();

	static Control *lastChildUnderMouse;
	static Control *trackControl;
	static Control *keyboardFocusControl;

	inline void FreeAction(Action *a)
	{
		if (a && a->autoDelete)
			delete a;
	}

	inline void RunAction(Action *a)
	{
		if (a)
			a->Run(this);
	}

	inline static void RunActionOf(Control *c, Action *a)
	{
		if (a)
			a->Run(c);
	}


};

typedef Control Widget;

}

#endif

/*
** Bad Sector's OpenGL GUI
*/

#ifndef __BSGUI_ACTIONS_H_INCLUDED__
#define __BSGUI_ACTIONS_H_INCLUDED__

namespace BSGUI
{

/*struct Action
{
	bool autoDelete;

	Action();
	virtual ~Action();

	virtual void Run(struct Control *sender){}
};*/

typedef void (*CallbackActionFunc)(struct Control *sender);

/*struct CallbackAction : public Action
{
	CallbackActionFunc      func;

	CallbackAction(CallbackActionFunc cbFunc);

	virtual void Run(struct Control *sender);
};*/

}

#endif

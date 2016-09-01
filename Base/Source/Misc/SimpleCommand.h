/****************************************************************************/
/*!
\file SimpleCommand.h
\author Lee Sek Heng
\par email: 150629Z@.mymail.nyp.edu.sg
\brief
Class to define Commands that the player will be using.
*/
/****************************************************************************/
#ifndef SIMPLE_COMMAND_H
#define SIMPLE_COMMAND_H

struct SimpleCommand
{
	enum COMMAND_TYPE
	{
		FORWARD_COMMAND = 0,
		BACK_COMMAND,
		RIGHT_COMMAND,
		LEFT_COMMAND,
		JUMP_COMMAND,
        INTERACT_COMMAND,
        PAUSE_MENU_COMMAND,
		TOTAL_COMMANDS,
	};
	static unsigned char m_allTheKeys[TOTAL_COMMANDS];
	SimpleCommand();
	~SimpleCommand();
	COMMAND_TYPE whatCommand;
};

#endif
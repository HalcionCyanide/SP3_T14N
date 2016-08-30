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
		TOTAL_COMMANDS,
	};
	static unsigned char m_allTheKeys[TOTAL_COMMANDS];
	SimpleCommand();
	~SimpleCommand();
	COMMAND_TYPE whatCommand;
};

#endif
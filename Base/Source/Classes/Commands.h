#pragma once

#include <vector>

struct Command
{
    static std::vector<Command*> All_Command_Type;
	Command();
	virtual ~Command();
    unsigned char theKey;
};

struct ForwardCommand : public Command
{
	ForwardCommand();
	virtual ~ForwardCommand();
};

struct BackCommand : public Command
{
	BackCommand();
	virtual ~BackCommand();
};

struct RightCommand : public Command
{
	RightCommand();
	virtual ~RightCommand();
};

struct LeftCommand : public Command
{
	LeftCommand();
	virtual ~LeftCommand();
};
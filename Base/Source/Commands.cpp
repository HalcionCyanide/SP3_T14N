#include "Commands.h"

std::vector<Command*> Command::All_Command_Type;

//unsigned char ForwardCommand::theKey = 'W';
//unsigned char BackCommand::theKey = 'S';
//unsigned char RightCommand::theKey = 'A';
//unsigned char LeftCommand::theKey = 'D';

Command::Command()
{
    theKey = NULL;
}

Command::~Command()
{
    
}

ForwardCommand::ForwardCommand()
{
    theKey = 'W';
    All_Command_Type.push_back(this);
}

ForwardCommand::~ForwardCommand()
{
}

BackCommand::BackCommand()
{
    theKey = 'S';
    All_Command_Type.push_back(this);
}

BackCommand::~BackCommand()
{
}

RightCommand::RightCommand()
{
    theKey = 'D';
    All_Command_Type.push_back(this);
}

RightCommand::~RightCommand()
{
}

LeftCommand::LeftCommand()
{
}

LeftCommand::~LeftCommand()
{
}
#include "SimpleCommand.h"

unsigned char SimpleCommand::m_allTheKeys[SimpleCommand::TOTAL_COMMANDS] = { 'W', 'S', 'A', 'D'};

SimpleCommand::SimpleCommand()
{
    whatCommand = TOTAL_COMMANDS;
}

SimpleCommand::~SimpleCommand()
{

}
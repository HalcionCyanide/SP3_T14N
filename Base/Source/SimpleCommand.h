#pragma once

struct SimpleCommand
{
    enum COMMAND_TYPE
    {
        FORWARD_COMMAND,
        BACK_COMMAND,
        RIGHT_COMMAND,
        LEFT_COMMAND,
        TOTAL_COMMANDS,
    };
    static unsigned char m_allTheKeys[TOTAL_COMMANDS];
    SimpleCommand();
    ~SimpleCommand();
    COMMAND_TYPE whatCommand;
};
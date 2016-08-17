#pragma once

struct Command
{
    Command() {};
    virtual ~Command() {};
};

struct ForwardCommand : public Command
{
    virtual ~ForwardCommand();
    static unsigned char theKey;
};
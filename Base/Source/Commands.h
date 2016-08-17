#pragma once

struct Command
{
    Command() {};
    virtual ~Command() {};
};

struct ForwardCommand : public Command
{
    ForwardCommand();
    virtual ~ForwardCommand();
    static unsigned char theKey;
};

struct BackCommand : public Command
{
    BackCommand();
    virtual ~BackCommand();
    static unsigned char theKey;
};

struct RightCommand : public Command
{
    RightCommand();
    virtual ~RightCommand();
    static unsigned char theKey;
};

struct LeftCommand : public Command
{
    LeftCommand();
    virtual ~LeftCommand();
    static unsigned char theKey;
};
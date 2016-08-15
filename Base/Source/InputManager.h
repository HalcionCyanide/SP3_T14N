#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

class InputManager
{
public:
	InputManager();
	void HandleUserInput();
	bool GetKeyValue(char);

private:
	bool cIM_Keys[256];
};

#endif
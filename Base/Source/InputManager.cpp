#include "InputManager.h"
#include "Application.h"

// Test Build 1 
// - Ryan

InputManager::InputManager()
{
    for (int num = 0; num < 256; ++num)
    {
        cIM_Keys[num] = false;
    }
}

void InputManager::HandleUserInput()
{
	for (unsigned i = 0; i < 256; ++i)
	{
		// Check Keypress
		if (Application::IsKeyPressed(i) && cIM_Keys[i] == false)
		{
			cIM_Keys[i] = true;
		}
		else if (!Application::IsKeyPressed(i) && cIM_Keys[i] == true)
		{
			cIM_Keys[i] = false;
		}
	}
}

bool InputManager::GetKeyValue(char c)
{
	return cIM_Keys[c];
}
#include "InputManager.h"
#include "Application.h"

// Test Build 2 
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

Vector3 InputManager::GetMousePosition()
{
	return MousePosition;
}

void InputManager::SetMousePosition(Vector3 v3)
{
	MousePosition = v3;
}

void InputManager::SetScreenSize(float x, float y)
{
	cIM_ScreenWidth = x;
	cIM_ScreenHeight = y;
}

void InputManager::UpdateMouse()
{
	//New Version for use with my camera version
	POINT mousePosition;
	GetCursorPos(&mousePosition);

	if (!cIM_inMouseMode)
	{
		int moveX = (int)cIM_ScreenWidth / 2;
		int moveY = (int)cIM_ScreenHeight / 2;

		//Lock the cursor's position to the center of the screen.
		SetCursorPos(moveX, moveY);

		//Calculate the difference between the cursor coordinates between frames
		cIM_CameraYaw = static_cast<float>(mousePosition.x - moveX);
		cIM_CameraPitch = static_cast<float>(mousePosition.y - moveY);
	}
	
	float ScreenX = mousePosition.x * Scene_System::accessing().GetUIWidth() / cIM_ScreenWidth;
	float ScreenY = (cIM_ScreenHeight - mousePosition.y) * Scene_System::accessing().GetUIHeight() / cIM_ScreenHeight;
	SetMousePosition(Vector3(ScreenX, ScreenY, 0));
}

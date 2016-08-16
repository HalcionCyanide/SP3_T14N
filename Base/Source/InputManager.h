#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include "Vector3.h"
#include "Scene_System.h"

class Scene_System;

class InputManager
{
public:
	InputManager();
	void HandleUserInput();
	bool GetKeyValue(char);
	Vector3 GetMousePosition();
	void SetMousePosition(Vector3);
	void UpdateMouse();

	void SetScreenSize(float, float);

	float cIM_ScreenWidth, cIM_ScreenHeight;
	float cIM_CameraYaw, cIM_CameraPitch;
	bool cIM_inMouseMode = false;

private:
	Vector3 MousePosition;
	bool cIM_Keys[256];
};

#endif
/****************************************************************************/
/*!
\file InputManager.h
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines the Input Manager that handles all forms of input
*/
/****************************************************************************/


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
	void SetMouseToScreenCenter();
	void UpdateMouse();

	void SetScreenSize(float, float);

	float cIM_ScreenWidth, cIM_ScreenHeight;
	float cIM_CameraYaw = 0, cIM_CameraPitch = 0;
	bool cIM_inMouseMode = false;

private:
	Vector3 MousePosition;
	bool cIM_Keys[256];
};

#endif
/****************************************************************************/
/*!
\file UI_System.h
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines the UI system that holds layers of UI elements
*/
/****************************************************************************/

#ifndef _UI_SYSTEM_H
#define _UI_SYSTEM_H

#include "GenericSystem.h"
#include "Vector3.h"
#include "..\\Classes\\UI_Layer.h"

#include <vector>

class UI_System : public GenericSystem
{
public:
	UI_System();
	virtual ~UI_System();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	std::vector<UI_Layer*> cUIS_LayerContainer;

};

#endif // _UI_SYSTEM_H
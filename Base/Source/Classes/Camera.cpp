#include "Camera.h"
#include "..\\Mains\\Application.h"
#include "Mtx44.h"

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(double dt)
{
	static const float CAMERA_SPEED = 20.f;
	/*if(Application::IsKeyPressed(VK_LEFT) || Application::IsKeyPressed('A'))
	{
	position.x -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_RIGHT) || Application::IsKeyPressed('D'))
	{
	position.x += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_UP) || Application::IsKeyPressed('W'))
	{
	position.y += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::IsKeyPressed(VK_DOWN) || Application::IsKeyPressed('S'))
	{
<<<<<<< HEAD:Base/Source/Camera.cpp
		position.y -= (float)(CAMERA_SPEED * 0.2 * dt);
=======
	position.y -= (float)(CAMERA_SPEED * 0.2 * dt);
>>>>>>> ab5a85f05485912636525a3765b25538f6987043:Base/Source/Classes/Camera.cpp
	}*/
}
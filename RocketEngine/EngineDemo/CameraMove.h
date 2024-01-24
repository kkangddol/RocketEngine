#pragma once
#include "..\\RocketEngine\\RocketAPI.h"

namespace Rocket
{
	class Camera;
}


class CameraMove : public Rocket::Component
{	
public:
	CameraMove(Rocket::GameObject* owner);

public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void OnMouseMove();

	// Enter ������ ���ؼ� ������.
private:
	void DrawAimLine();

public:
	float moveSpeed;

private:
	Rocket::Camera* _camera;
};

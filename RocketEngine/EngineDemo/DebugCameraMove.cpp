#include "DebugCameraMove.h"

DebugCameraMove::DebugCameraMove(RocketEngine::GameObject* owner)
	: Component(owner),
	moveSpeed(2.0f)
{
}

void DebugCameraMove::Start()
{
	_camera = gameObject->GetComponent<RocketEngine::Camera>();
}

void DebugCameraMove::Update()
{
	//float deltaTime = rocket.GetDeltaTime();
	float deltaTime = RocketEngine::GetDeltaTime();
	moveSpeed = 5.0f;

	if (RocketEngine::GetKey(VK_SHIFT))
	{
		moveSpeed *= 2.0f;
	}

	if (RocketEngine::GetKey('W'))
	{
		_camera->Walk(moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey('S'))
	{
		_camera->Walk(-moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey('A'))
	{
		_camera->Strafe(-moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey('D'))
	{
		_camera->Strafe(moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey('Q'))
	{
		_camera->WorldUpDown(-moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey('E'))
	{
		_camera->WorldUpDown(moveSpeed * deltaTime);
	}

	OnMouseMove();
}

void DebugCameraMove::OnMouseMove()
{
	if (!RocketEngine::GetKey(VK_RBUTTON))
	{
		return;
	}

	RocketEngine::Vector2 mouseDelta = RocketEngine::GetMouseDelta();
	mouseDelta = mouseDelta * RocketEngine::GetDeltaTime();
	_camera->Pitch(mouseDelta.y);
	_camera->RotateY(mouseDelta.x);
}
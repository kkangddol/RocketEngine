#include "CameraMove.h"


CameraMove::CameraMove(RocketEngine::GameObject* owner)
	: Component(owner),
	moveSpeed(2.0f)
{
}

void CameraMove::Start()
{
	_camera = gameObject->GetComponent<RocketEngine::Camera>();
}

void CameraMove::Update()
{
	/// ī�޶� ������ �ʿ䰡 ��� �� �ּ�ó�� �ߴ�
	/*
	//float deltaTime = rocket.GetDeltaTime();
	float deltaTime = RocketEngine::GetDeltaTime();
	moveSpeed = 5.0f;

	if (RocketEngine::GetKey(VK_SHIFT))
	{
		moveSpeed *= 2.0f;
	}

	if (RocketEngine::GetKey(VK_UP))
	{
		_camera->Walk(moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey(VK_DOWN))
	{
		_camera->Walk(-moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey(VK_LEFT))
	{
		_camera->Strafe(-moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey(VK_RIGHT))
	{
		_camera->Strafe(moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey(VK_NEXT))
	{
		_camera->WorldUpDown(-moveSpeed * deltaTime);
	}

	if (RocketEngine::GetKey(VK_PRIOR))
	{
		_camera->WorldUpDown(moveSpeed * deltaTime);
	}
	// 
	// 	if (RocketEngine::GetKeyDown(VK_LBUTTON))
	// 	{
	// 		RocketEngine::Raycast* ray = gameObject->GetComponent<RocketEngine::Raycast>();
	// 		ray->UpdateRayInfo();
	// 		ray->PerformRayCast();
	// 	}

	//OnMouseMove();

	DrawAimLine();
	*/
}

void CameraMove::OnMouseMove()
{
	//// Player�� �ü� ����� ī�޶��� ������ ������ ��, acos�� �־� ������ ���Ѵ�.
	//RocketEngine::Vector3 playerForward = Vector3Normalize(_camera->gameObject->transform.GetParent()->GetParent()->GetForward());
	//RocketEngine::Vector3 cameraForward = Vector3Normalize(_camera->gameObject->transform.GetForward());
	////RocketEngine::Vector4 temp = Vector4MultiplyMatrix(RocketEngine::Vector4(cameraForward.x, cameraForward.y, cameraForward.z, 1.0f), 
	////	_camera->gameObject->transform.GetParent()->GetWorldRotationMatrix());
	////cameraForward.x = temp.x;
	////cameraForward.y = temp.y;
	////cameraForward.z = temp.z;
	//float pitchAngle = acos(playerForward * cameraForward);

	//// 30��(radian���� pie/6 = 0.5236...) �̳� ������ ���� pitch ����. �ƴϸ� 30���� ����.
	///// �θ��� rotation�� ������� �ϰų�, ���콺�� ȭ�� ������ ���� ���� �߰����� ��ġ�� �ʿ��� ��츦 �����غ� �� �ִ�.

	/// Try.0
	//RocketEngine::Vector2 mouseDelta = RocketEngine::GetMouseDelta();
	Vector2 mouseDelta = RocketEngine::MouseDeltaSmooth();
	_camera->Pitch(mouseDelta.y * RocketEngine::GetDeltaTime() * 0.05f);

	/// Tryseok
	Vector3 euler = _camera->gameObject->transform.GetLocalEuler();
	if (89.0f < euler.x)
	{
		_camera->gameObject->transform.SetLocalRotationEuler(89.0f, 0.0f, 0.0f);
	}
	else if (euler.x < -89.0f)
	{
		_camera->gameObject->transform.SetLocalRotationEuler(-89.0f, 0.0f, 0.0f);
	}

	/// Try.2
// 	if (pitchAngle > 0.5236f)
// 	{
// 		if (cameraForward.y > 0)
// 		{
// 			_camera->gameObject->transform.SetLocalRotation(-0.5235f, 1.0f, 0.0f, 0.0f);
// 		}
// 		else
// 		{
// 			_camera->gameObject->transform.SetLocalRotation(0.5235f, 1.0f, 0.0f, 0.0f);
// 		}
// 	}
// 	else
// 	{
// 		RocketEngine::Vector2 mouseDelta = RocketEngine::GetMouseDelta();
// 		_camera->Pitch(mouseDelta.y * 0.5f);
//	}

	/// Try.4
	/// ���� ���̸� �������� ��¦ ƨ��� ȸ�������ش�.
	//if (pitchAngle > 0.5236f)
	//{
	//	isPitchAbleAngle = false;
	//}

	//if (isPitchAbleAngle)
	//{
	//	RocketEngine::Vector2 mouseDelta = RocketEngine::GetMouseDelta();
	//	mouseDelta = mouseDelta * RocketEngine::GetDeltaTime();
	//	_camera->Pitch(mouseDelta.y * 0.1f);
	//}
	//else
	//{
	//	float angleX = cameraForward.y;
	//	_camera->gameObject->transform.Rotate(angleX, 0.0f, 0.0f);

	//	isPitchAbleAngle = true;
	//}
}

void CameraMove::DrawAimLine()
{
	if (RocketEngine::GetKey(VK_RETURN))
	{
		Vector3 pos = gameObject->transform.GetPosition() + Vector3(0.7f, -0.4f, 0.0f);

		Matrix tempTM = gameObject->transform.GetWorldTM();
		Vector3 temp =  Vector3::Transform(pos , tempTM);
		pos.x = temp.x;
		pos.y = temp.y;
		pos.z = temp.z;

		Vector3 target = pos + gameObject->transform.GetForward() * 50.0f;
		RocketEngine::DrawDebugLine(pos, target);
	}
}

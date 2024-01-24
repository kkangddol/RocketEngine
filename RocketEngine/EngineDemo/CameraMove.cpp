#include "CameraMove.h"


CameraMove::CameraMove(Rocket::GameObject* owner)
	: Component(owner),
	moveSpeed(2.0f)
{
}

void CameraMove::Start()
{
	_camera = gameObject->GetComponent<Rocket::Camera>();
}

void CameraMove::Update()
{
	/// 카메라 움직일 필요가 없어서 다 주석처리 했다
	/*
	//float deltaTime = rocket.GetDeltaTime();
	float deltaTime = Rocket::GetDeltaTime();
	moveSpeed = 5.0f;

	if (Rocket::GetKey(VK_SHIFT))
	{
		moveSpeed *= 2.0f;
	}

	if (Rocket::GetKey(VK_UP))
	{
		_camera->Walk(moveSpeed * deltaTime);
	}

	if (Rocket::GetKey(VK_DOWN))
	{
		_camera->Walk(-moveSpeed * deltaTime);
	}

	if (Rocket::GetKey(VK_LEFT))
	{
		_camera->Strafe(-moveSpeed * deltaTime);
	}

	if (Rocket::GetKey(VK_RIGHT))
	{
		_camera->Strafe(moveSpeed * deltaTime);
	}

	if (Rocket::GetKey(VK_NEXT))
	{
		_camera->WorldUpDown(-moveSpeed * deltaTime);
	}

	if (Rocket::GetKey(VK_PRIOR))
	{
		_camera->WorldUpDown(moveSpeed * deltaTime);
	}
	// 
	// 	if (Rocket::GetKeyDown(VK_LBUTTON))
	// 	{
	// 		Rocket::Raycast* ray = gameObject->GetComponent<Rocket::Raycast>();
	// 		ray->UpdateRayInfo();
	// 		ray->PerformRayCast();
	// 	}

	//OnMouseMove();

	DrawAimLine();
	*/
}

void CameraMove::OnMouseMove()
{
	//// Player의 시선 방향과 카메라의 방향을 내적한 후, acos에 넣어 각도를 구한다.
	//Rocket::Vector3 playerForward = Vector3Normalize(_camera->gameObject->transform.GetParent()->GetParent()->GetForward());
	//Rocket::Vector3 cameraForward = Vector3Normalize(_camera->gameObject->transform.GetForward());
	////Rocket::Vector4 temp = Vector4MultiplyMatrix(Rocket::Vector4(cameraForward.x, cameraForward.y, cameraForward.z, 1.0f), 
	////	_camera->gameObject->transform.GetParent()->GetWorldRotationMatrix());
	////cameraForward.x = temp.x;
	////cameraForward.y = temp.y;
	////cameraForward.z = temp.z;
	//float pitchAngle = acos(playerForward * cameraForward);

	//// 30도(radian으로 pie/6 = 0.5236...) 이내 범위일 때만 pitch 수행. 아니면 30도에 고정.
	///// 부모의 rotation을 곱해줘야 하거나, 마우스가 화면 밖으로 갔을 때의 추가적인 조치가 필요한 경우를 생각해볼 수 있다.

	/// Try.0
	//Rocket::Vector2 mouseDelta = Rocket::GetMouseDelta();
	Vector2 mouseDelta = Rocket::MouseDeltaSmooth();
	_camera->Pitch(mouseDelta.y * Rocket::GetDeltaTime() * 0.05f);

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
// 		Rocket::Vector2 mouseDelta = Rocket::GetMouseDelta();
// 		_camera->Pitch(mouseDelta.y * 0.5f);
//	}

	/// Try.4
	/// 범위 밖이면 안쪽으로 살짝 튕기듯 회전시켜준다.
	//if (pitchAngle > 0.5236f)
	//{
	//	isPitchAbleAngle = false;
	//}

	//if (isPitchAbleAngle)
	//{
	//	Rocket::Vector2 mouseDelta = Rocket::GetMouseDelta();
	//	mouseDelta = mouseDelta * Rocket::GetDeltaTime();
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
	if (Rocket::GetKey(VK_RETURN))
	{
		Vector3 pos = gameObject->transform.GetPosition() + Vector3(0.7f, -0.4f, 0.0f);

		Matrix tempTM = gameObject->transform.GetWorldTM();
		Vector3 temp =  Vector3::Transform(pos , tempTM);
		pos.x = temp.x;
		pos.y = temp.y;
		pos.z = temp.z;

		Vector3 target = pos + gameObject->transform.GetForward() * 50.0f;
		Rocket::DrawDebugLine(pos, target);
	}
}

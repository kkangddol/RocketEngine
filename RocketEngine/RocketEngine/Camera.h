#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include "MathHeader.h"
#include "../RocketGraphicsInterface/ICamera.h"

namespace Rocket::Core
{
	class GraphicsSystem;
}

namespace Rocket
{
	class GameObject;

	class ROCKET_API Camera final : public Component
	{
		friend class Rocket::Core::GraphicsSystem;
	public:
		Camera(GameObject* owner);

	protected:
		virtual void Start() override;
		virtual void SetRenderData() override;

	public:
		Core::ICamera& GetCamera();
		
	private:
		Core::ICamera* _camera;

	public:
		float GetNearZ() const;
		float GetFarZ() const;
		float GetAspect() const;			// ī�޶� ���� screen.width/screen.height
		float GetFovX() const;				// FovX���� 60�й����� ��ȯ
		float GetRadianFovX() const;		// FovX���� ȣ�������� ��ȯ
		float GetFovY() const;				// FovY���� 60�й����� ��ȯ
		float GetRadianFovY() const;		// FovY���� ȣ�������� ��ȯ

		float GetNearWindowWidth() const;
		float GetNearWindowHeight() const;
		float GetFarWindowWidth() const;
		float GetFarWindowHeight() const;

		Matrix GetViewMatrix() const;	// ī�޶��� ������ǥ'��'�� ��ȯ
		Matrix GetProjMatrix() const;	// ī�޶��� ��������� ��ȯ

		void SetNearZ(float nearZ);
		void SetFarZ(float farZ);
		void SetAspect(float aspect);
		void SetFovY(float fovY);

	public:
		void Strafe(float delta);
		void Walk(float delta);
		void WorldUpDown(float delta);

		void Yaw(float angle);
		void Pitch(float angle);
		void Roll(float angle);
		void RotateY(float angle);

		void ShakeCameraUpdateOnHit(float deltaTime);
		void ShakeCameraUpdateOnShoot(float deltaTime);
		void SetIsCameraShakeOnHit(bool cameraShake);
		void SetIsCameraShakeOnShoot(bool cameraShake);

	private:
		float _nearZ;				// frustum�� ����� �������� �Ÿ�
		float _farZ;				// frustum�� �� �������� �Ÿ�
		float _aspect;				// ���� / ���� ����
		float _fovY;				// fov������ 60�й����� ��������
		float _nearWindowHeight;	// frustum�� ����� ����� ����
		float _farWindowHeight;		// frustum�� �� ����� ����

		Matrix _viewMatrix;		// ī�޶��� ������ǥ'��'
		Matrix _projMatrix;		// ī�޶��� ���� ���

		bool _isShakeCameraOnHit;
		bool _isShakeCameraOnShoot;
		float _shakeDurationOnHit;
		float _shakeDurationOnShoot;
		float _distX;
		float _distY;
		float _distYOnShoot;
	};
}

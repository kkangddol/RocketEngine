#pragma once

#include <DirectXMath.h>
#include <cmath>

/// <summary>
/// ī�޶� Ŭ����
/// ������ �ڵ� �����Ͽ� ���� �ʿ��� �� �����͸� ����
/// 
/// 23.04.20 ������ �����
/// </summary>
class Camera
{
public:
	Camera();
	~Camera();

public:
	DirectX::XMFLOAT3 GetPosition() const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const DirectX::XMFLOAT3& pos);

	DirectX::XMFLOAT3 GetLook() const;
	DirectX::XMFLOAT3 GetRight() const;
	DirectX::XMFLOAT3 GetUp() const;

	float GetNearZ() const;
	float GetFarZ() const;
	float GetAspect() const;		// ī�޶� ���� screen.width/screen.height
	float GetFovX() const;			// FovX���� 60�й����� ��ȯ
	float GetRadianFovX() const;	// FovX���� ȣ�������� ��ȯ
	float GetFovY() const;			// FovY���� 60�й����� ��ȯ
	float GetRadianFovY() const;	// FovY���� ȣ�������� ��ȯ

	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	void SetFrustum(float fovY, float aspect, float nearZ, float farZ);

	void LookAt(const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	void UpdateViewMatrix();

	DirectX::XMMATRIX GetViewMatrix() const;				// ī�޶��� ������ǥ'��'�� ��ȯ
	DirectX::XMMATRIX GetProjectionMatrix() const;		// ī�޶��� ��������� ��ȯ
	DirectX::XMMATRIX GetViewProjectionMatrix() const;	// ���� �� ����� ������ ���ؼ� ��ȯ(view�� ������� �����ְ���?)

	void Strafe(float delta);
	void Walk(float delta);
	void WorldUpDown(float delta);

	void Yaw(float angle);
	void Pitch(float angle);
	void Roll(float angle);
	void RotateY(float angle);


private:
	DirectX::XMFLOAT3 position_;				// ī�޶��� ��ġ (���� (���� ����))
	DirectX::XMFLOAT3 look_;					// ī�޶��� ���� (����)
	DirectX::XMFLOAT3 right_;				// ī�޶��� ������ (����)
	DirectX::XMFLOAT3 up_;					// ī�޶��� �� (����)

	float nearZ_;					// frustum�� ����� �������� �Ÿ�
	float farZ_;					// frustum�� �� �������� �Ÿ�
	float aspect_;					// ���� / ���� ����
	float fovY_;					// fov������ 60�й����� ��������
	float nearWindowHeight_;		// frustum�� ����� ����� ����
	float farWindowHeight_;			// frustum�� �� ����� ����

	DirectX::XMFLOAT4X4 viewMatrix_;			// ī�޶��� ������ǥ'��'
	DirectX::XMFLOAT4X4 projectionMatrix_;	// ī�޶��� ���� ���
};


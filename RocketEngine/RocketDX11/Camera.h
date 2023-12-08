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
	void SetRotation(float w, float x, float y, float z);

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

	void UpdateViewMatrix();

	DirectX::XMMATRIX GetViewMatrix() const;				// ī�޶��� ������ǥ'��'�� ��ȯ
	DirectX::XMMATRIX GetProjectionMatrix() const;		// ī�޶��� ��������� ��ȯ
	DirectX::XMMATRIX GetViewProjectionMatrix() const;	// ���� �� ����� ������ ���ؼ� ��ȯ(view�� ������� �����ְ���?)

	DirectX::XMVECTOR GetForward() const;
	DirectX::XMVECTOR GetUp() const;
	DirectX::XMVECTOR GetRight() const;

private:
	DirectX::XMFLOAT3 _position;				// ��ġ (����)
	DirectX::XMFLOAT4 _rotation;				// ���ʹϾ� (����)

	float _nearZ;					// frustum�� ����� �������� �Ÿ�
	float _farZ;					// frustum�� �� �������� �Ÿ�
	float _aspect;					// ���� / ���� ����
	float _fovY;					// fov������ 60�й����� ��������
	float _nearWindowHeight;		// frustum�� ����� ����� ����
	float _farWindowHeight;			// frustum�� �� ����� ����

	DirectX::XMFLOAT4X4 _viewMatrix;			// ī�޶��� ������ǥ'��'
	DirectX::XMFLOAT4X4 _projectionMatrix;	// ī�޶��� ���� ���
};


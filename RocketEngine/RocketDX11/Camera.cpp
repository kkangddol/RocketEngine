#include "Camera.h"

using namespace DirectX;

Camera::Camera()
	: position_(0.0f, 2.0f, -10.0f),
	look_(0.0f, 0.0f, 1.0f), right_(1.0f, 0.0f, 0.0f), up_(0.0f, 1.0f, 0.0f),
	nearZ_(0.01f), farZ_(50.0f), aspect_(1.0f), fovY_(90.0f),
	nearWindowHeight_(), farWindowHeight_(),
	viewMatrix_(), projectionMatrix_()
{
	SetFrustum(fovY_, aspect_, nearZ_, farZ_);
}

Camera::~Camera()
{

}

DirectX::XMFLOAT3 Camera::GetPosition() const
{
	return position_;
}

void Camera::SetPosition(float x, float y, float z)
{
	position_ = { x,y,z };
}

void Camera::SetPosition(const XMFLOAT3& pos)
{
	position_ = pos;
}

DirectX::XMFLOAT3 Camera::GetLook() const
{
	return look_;
}

DirectX::XMFLOAT3 Camera::GetRight() const
{
	return right_;
}

DirectX::XMFLOAT3 Camera::GetUp() const
{
	return up_;
}

float Camera::GetNearZ() const
{
	return nearZ_;
}

float Camera::GetFarZ() const
{
	return farZ_;
}

float Camera::GetAspect() const
{
	return aspect_;
}

float Camera::GetFovX() const
{
	return XMConvertToDegrees(GetRadianFovX());
}

float Camera::GetRadianFovX() const
{
	float halfWidth = GetNearWindowWidth() / 2;
	return 2.0f * atan(halfWidth / nearZ_);
}

float Camera::GetFovY() const
{
	return fovY_;
}

float Camera::GetRadianFovY() const
{
	return XMConvertToRadians(fovY_);
}

float Camera::GetNearWindowWidth() const
{
	return aspect_ * nearWindowHeight_;
}

float Camera::GetNearWindowHeight() const
{
	return nearWindowHeight_;
}

float Camera::GetFarWindowWidth() const
{
	return aspect_ * farWindowHeight_;
}

float Camera::GetFarWindowHeight() const
{
	return farWindowHeight_;
}

/// ī�޶��� ������ �����Ѵ�.
/// ���� ���� ���� ���� ����鵵 �缳�����ش�.
/// ������ XMMatrixPerspectiveFovLH �Լ��� �̿��ؼ� ��������� �����.
/// �� �κ��� ���� ���� ��������� ������ �ʹ�.
/// 
/// 23.04.20 ������ �����
void Camera::SetFrustum(float fovY, float aspect, float nearZ, float farZ)
{
	fovY_ = fovY;
	aspect_ = aspect;
	nearZ_ = nearZ;
	farZ_ = farZ;

	nearWindowHeight_ = 2.0f * nearZ_ * std::tanf(XMConvertToRadians(fovY_/2));
	farWindowHeight_ = 2.0f * farZ_ * std::tanf(XMConvertToRadians(fovY_ / 2));

	XMMATRIX temp = XMMatrixPerspectiveFovLH(XMConvertToRadians(fovY_ / 2), aspect_, nearZ_, farZ_);
	XMStoreFloat4x4(&projectionMatrix_, temp);
}

/// <summary>
/// target�� up���͸� �ָ� �װ��� �������� ī�޶� �ٶ󺻴�.
/// 
/// 23.04.20 ������ �����
/// </summary>
/// <param name="target">�ٶ� Ÿ��</param>
/// <param name="up"> ī�޶� right vector�� ���Ҷ� ����� up ����</param>
void Camera::LookAt(const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR targetVector = XMLoadFloat3(&target);
	XMVECTOR lookVector = XMVector3Normalize(XMVectorSubtract(targetVector, XMLoadFloat3(&position_)));
	XMVECTOR rightVector = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&up), lookVector));
	XMVECTOR upVector = XMVector3Normalize(XMVector3Cross(lookVector, rightVector));

	XMStoreFloat3(&look_, lookVector);
	XMStoreFloat3(&right_, rightVector);
	XMStoreFloat3(&up_, upVector);
}

/// <summary>
/// ī�޶��� ��ġ�� �����ϸ鼭 �ٶ󺸴� �Լ�.
/// ���������� �����ε��� LookAt()�Լ��� �� ȣ���Ѵ�.
/// 
/// 23.04.20 ������ �����
/// </summary>
/// <param name="pos">ī�޶��� ��ġ</param>
/// <param name="target">�ٶ� Ÿ��</param>
/// <param name="up">ī�޶�� right vector�� ���Ҷ� ����� up ����</param>
void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	position_ = pos;
	LookAt(target, up);
}

/// ViewMatrix�� �������ش�.
/// ������ �ڵ忡���� �߸��� ���Ͱ� �Ǿ����� �� look�� �������� �ٽ� �����ؼ� �����ϴ� �� �ϴ�.
/// ���� �׳� viewMatrix�� ���������.
/// 
/// �׸��� position�� ������ ����Ѵ�! �ֳ��ϸ� ī�޶� �������� �ű�°Ŵϱ�!
/// 
/// 23.04.20 ������ �����
void Camera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&right_);
	XMVECTOR U = XMLoadFloat3(&up_);
	XMVECTOR L = XMLoadFloat3(&look_);
	XMVECTOR P = XMLoadFloat3(&position_);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&right_, R);
	XMStoreFloat3(&up_, U);
	XMStoreFloat3(&look_, L);

	viewMatrix_(0, 0) = right_.x;
	viewMatrix_(1, 0) = right_.y;
	viewMatrix_(2, 0) = right_.z;
	viewMatrix_(3, 0) = x;

	viewMatrix_(0, 1) = up_.x;
	viewMatrix_(1, 1) = up_.y;
	viewMatrix_(2, 1) = up_.z;
	viewMatrix_(3, 1) = y;

	viewMatrix_(0, 2) = look_.x;
	viewMatrix_(1, 2) = look_.y;
	viewMatrix_(2, 2) = look_.z;
	viewMatrix_(3, 2) = z;

	viewMatrix_(0, 3) = 0.0f;
	viewMatrix_(1, 3) = 0.0f;
	viewMatrix_(2, 3) = 0.0f;
	viewMatrix_(3, 3) = 1.0f;
// 
// 	viewMatrix_ =
// 	{
// 		right_.x,		up_.x,			look_.x,		0.0f,
// 		right_.y,		up_.y,			look_.y,		0.0f,
// 		right_.z,		up_.z,			look_.z,		0.0f,
// 		-position_.x,	-position_.y,	-position_.z,	1.0f
// 	};
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return XMLoadFloat4x4(&viewMatrix_);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{
	return XMLoadFloat4x4(&projectionMatrix_);
}

DirectX::XMMATRIX Camera::GetViewProjectionMatrix() const
{
	return XMMatrixMultiply(GetViewMatrix(), GetProjectionMatrix());
}

void Camera::Strafe(float delta)
{
	XMVECTOR s = XMVectorReplicate(delta);
	XMVECTOR r = XMLoadFloat3(&right_);
	XMVECTOR p = XMLoadFloat3(&position_);
	XMStoreFloat3(&position_, XMVectorMultiplyAdd(s, r, p));
}

void Camera::Walk(float delta)
{
	XMVECTOR s = XMVectorReplicate(delta);
	XMVECTOR l = XMLoadFloat3(&look_);
	XMVECTOR p = XMLoadFloat3(&position_);
	XMStoreFloat3(&position_, XMVectorMultiplyAdd(s, l, p));
}


/// ����������� ��, �Ʒ��� �����̴� �Լ�
/// ���� �������� Up�� ī�޶��� ���� ��ǥ���� ������� ���ؼ� �� �����.
///
/// 23.04.20 ������ �����
void Camera::WorldUpDown(float delta)
{
	XMVECTOR s = { 0.0f, delta, 0.0f, 0.0f };
	XMMATRIX viewMatrix = XMLoadFloat4x4(&viewMatrix_);
	XMVECTOR temp = XMVector4Transform(s, viewMatrix);

	position_.x += XMVectorGetX(temp);
	position_.y += XMVectorGetY(temp);
	position_.z += XMVectorGetZ(temp);
}

/// TRT �� �ؾ߰ڴ�!
///
/// 23.04.21 ������ �����
void Camera::Yaw(float angle)
{
	XMMATRIX translation =
	{
		1.0f,			0.0f,			0.0f,			0.0f,
		0.0f,			1.0f,			0.0f,			0.0f,
		0.0f,			0.0f,			1.0f,			0.0f,
		-position_.x,	-position_.y,	-position_.z,	1.0f
	};

	XMMATRIX rotation =
	{
		std::cosf(angle),	0.0f,	-std::sinf(angle),	0.0f,
		0.0f,					0.0f,	0.0f,					0.0f,
		std::sinf(angle),	0.0f,	std::cosf(angle),	0.0f,
		0.0f,					0.0f,	0.0f,					1.0f
	};

	XMMATRIX inverseTranslation =
	{
		1.0f,			0.0f,			0.0f,			0.0f,
		0.0f,			1.0f,			0.0f,			0.0f,
		0.0f,			0.0f,			1.0f,			0.0f,
		position_.x,	position_.y,	position_.z,	1.0f
	};

	XMMATRIX matrix = translation * rotation * inverseTranslation;

	XMStoreFloat3(&look_, XMVector3TransformNormal(XMLoadFloat3(&look_), rotation));
	XMStoreFloat3(&right_, XMVector3TransformNormal(XMLoadFloat3(&right_), rotation));
}

void Camera::Pitch(float angle)
{
	XMMATRIX matrix = XMMatrixRotationAxis(XMLoadFloat3(&right_), angle);

	XMStoreFloat3(&look_, XMVector3TransformNormal(XMLoadFloat3(&look_), matrix));
	XMStoreFloat3(&up_, XMVector3TransformNormal(XMLoadFloat3(&up_), matrix));
}

void Camera::Roll(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX L = XMMatrixRotationAxis(XMLoadFloat3(&look_), angle);

	XMStoreFloat3(&up_, XMVector3TransformNormal(XMLoadFloat3(&up_), L));
	XMStoreFloat3(&right_, XMVector3TransformNormal(XMLoadFloat3(&right_), L));
}

void Camera::RotateY(float angle)
{
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&right_, XMVector3TransformNormal(XMLoadFloat3(&right_), R));
	XMStoreFloat3(&up_, XMVector3TransformNormal(XMLoadFloat3(&up_), R));
	XMStoreFloat3(&look_, XMVector3TransformNormal(XMLoadFloat3(&look_), R));
}

/// ���� ���� Yaw Pitch Roll�ε� ���������ϴٰ� ����������... �ФФФ�
/// ���߿� �ð����� �ٽ� ������!
/// 
/// 23.04.21 ������ �����
/*
/// ī�޶��� ���� Y���� �������� ȸ���ϴ� �Լ�
/// Y���� �������� ȸ���ϰ� viewMatrix�� look,right,up�� ����
///
/// 23.04.20 ������
void Camera::Yaw(float angle)
{
	// ȸ������ �����մϴ�.
	XMVECTOR axis = XMLoadFloat3(&up_);

	// Quaternion�� �����մϴ�.
	XMVECTOR quat = XMQuaternionRotationAxis(axis, angle);

	// �� ����� ȸ����ŵ�ϴ�.
	XMMATRIX viewMatrix = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix_), XMMatrixRotationQuaternion(quat));

	// ȸ���� �� ����� �����մϴ�.
	//XMStoreFloat4x4(&viewMatrix_, viewMatrix);
	XMStoreFloat3(&look_, XMVector3TransformNormal(XMLoadFloat3(&look_), viewMatrix));
	XMStoreFloat3(&right_, XMVector3TransformNormal(XMLoadFloat3(&right_), viewMatrix));
	//XMStoreFloat3(&up_, XMVector3TransformNormal(XMLoadFloat3(&up_), viewMatrix));
}

/// ī�޶��� ���� X���� �������� ȸ���ϴ� �Լ�
/// X���� �������� ȸ���ϰ� viewMatrix�� look,right,up�� ����
///
/// 23.04.20 ������
void Camera::Pitch(float angle)
{
	// ȸ������ �����մϴ�.
	XMVECTOR axis = XMLoadFloat3(&right_);

	// Quaternion�� �����մϴ�.
	XMVECTOR quat = XMQuaternionRotationAxis(axis, angle);

	// �� ����� ȸ����ŵ�ϴ�.
	XMMATRIX viewMatrix = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix_), XMMatrixRotationQuaternion(quat));

	// ȸ���� �� ����� �����մϴ�.
	//XMStoreFloat4x4(&viewMatrix_, viewMatrix);
	XMStoreFloat3(&look_, XMVector3TransformNormal(XMLoadFloat3(&look_), viewMatrix));
	//XMStoreFloat3(&right_, XMVector3TransformNormal(XMLoadFloat3(&right_), viewMatrix));
	XMStoreFloat3(&up_, XMVector3TransformNormal(XMLoadFloat3(&up_), viewMatrix));
}

/// ī�޶��� ���� Z���� �������� ȸ���ϴ� �Լ�
/// Z���� �������� ȸ���ϰ� viewMatrix�� look,right,up�� ����
///
/// 23.04.20 ������
void Camera::Roll(float angle)
{
	// ȸ������ �����մϴ�.
	XMVECTOR axis = XMLoadFloat3(&look_);

	// Quaternion�� �����մϴ�.
	XMVECTOR quat = XMQuaternionRotationAxis(axis, angle);

	// �� ����� ȸ����ŵ�ϴ�.
	XMMATRIX viewMatrix = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix_), XMMatrixRotationQuaternion(quat));

	// ȸ���� �� ����� �����մϴ�.
	//XMStoreFloat4x4(&viewMatrix_, viewMatrix);
	//XMStoreFloat3(&look_, XMVector3TransformNormal(XMLoadFloat3(&look_), viewMatrix));
	XMStoreFloat3(&right_, XMVector3TransformNormal(XMLoadFloat3(&right_), viewMatrix));
	XMStoreFloat3(&up_, XMVector3TransformNormal(XMLoadFloat3(&up_), viewMatrix));
}
*/
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

/// 카메라의 세팅을 설정한다.
/// 들어온 값에 맞춰 여러 멤버들도 재설정해준다.
/// 지금은 XMMatrixPerspectiveFovLH 함수를 이용해서 투영행렬을 만든다.
/// 이 부분은 내가 직접 투영행렬을 만들어보고 싶다.
/// 
/// 23.04.20 강석원 인재원
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
/// target과 up벡터를 주면 그것을 기준으로 카메라가 바라본다.
/// 
/// 23.04.20 강석원 인재원
/// </summary>
/// <param name="target">바라볼 타겟</param>
/// <param name="up"> 카메라가 right vector를 구할때 사용할 up 벡터</param>
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
/// 카메라의 위치도 변경하면서 바라보는 함수.
/// 내부적으로 오버로딩된 LookAt()함수를 또 호출한다.
/// 
/// 23.04.20 강석원 인재원
/// </summary>
/// <param name="pos">카메라의 위치</param>
/// <param name="target">바라볼 타겟</param>
/// <param name="up">카메라게 right vector를 구할때 사용할 up 벡터</param>
void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	position_ = pos;
	LookAt(target, up);
}

/// ViewMatrix를 갱신해준다.
/// 교수님 코드에서는 잘못된 벡터가 되어있을 시 look을 기준으로 다시 점검해서 변경하는 듯 하다.
/// 나는 그냥 viewMatrix만 갱신해줬다.
/// 
/// 그리고 position은 음수로 줘야한다! 왜냐하면 카메라 기준으로 옮기는거니까!
/// 
/// 23.04.20 강석원 인재원
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


/// 월드기준으로 위, 아래로 움직이는 함수
/// 월드 기준으로 Up을 카메라의 로컬 좌표계의 역행렬을 곱해서 더 해줬다.
///
/// 23.04.20 강석원 인재원
void Camera::WorldUpDown(float delta)
{
	XMVECTOR s = { 0.0f, delta, 0.0f, 0.0f };
	XMMATRIX viewMatrix = XMLoadFloat4x4(&viewMatrix_);
	XMVECTOR temp = XMVector4Transform(s, viewMatrix);

	position_.x += XMVectorGetX(temp);
	position_.y += XMVectorGetY(temp);
	position_.z += XMVectorGetZ(temp);
}

/// TRT 를 해야겠다!
///
/// 23.04.21 강석원 인재원
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

/// 내가 만들어본 Yaw Pitch Roll인데 차원도약하다가 터져버린다... ㅠㅠㅠㅠ
/// 나중에 시간날때 다시 봐야함!
/// 
/// 23.04.21 강석원 인재원
/*
/// 카메라의 로컬 Y축을 기준으로 회전하는 함수
/// Y축을 기준으로 회전하고 viewMatrix와 look,right,up을 갱신
///
/// 23.04.20 강석원
void Camera::Yaw(float angle)
{
	// 회전축을 설정합니다.
	XMVECTOR axis = XMLoadFloat3(&up_);

	// Quaternion을 생성합니다.
	XMVECTOR quat = XMQuaternionRotationAxis(axis, angle);

	// 뷰 행렬을 회전시킵니다.
	XMMATRIX viewMatrix = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix_), XMMatrixRotationQuaternion(quat));

	// 회전된 뷰 행렬을 적용합니다.
	//XMStoreFloat4x4(&viewMatrix_, viewMatrix);
	XMStoreFloat3(&look_, XMVector3TransformNormal(XMLoadFloat3(&look_), viewMatrix));
	XMStoreFloat3(&right_, XMVector3TransformNormal(XMLoadFloat3(&right_), viewMatrix));
	//XMStoreFloat3(&up_, XMVector3TransformNormal(XMLoadFloat3(&up_), viewMatrix));
}

/// 카메라의 로컬 X축을 기준으로 회전하는 함수
/// X축을 기준으로 회전하고 viewMatrix와 look,right,up을 갱신
///
/// 23.04.20 강석원
void Camera::Pitch(float angle)
{
	// 회전축을 설정합니다.
	XMVECTOR axis = XMLoadFloat3(&right_);

	// Quaternion을 생성합니다.
	XMVECTOR quat = XMQuaternionRotationAxis(axis, angle);

	// 뷰 행렬을 회전시킵니다.
	XMMATRIX viewMatrix = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix_), XMMatrixRotationQuaternion(quat));

	// 회전된 뷰 행렬을 적용합니다.
	//XMStoreFloat4x4(&viewMatrix_, viewMatrix);
	XMStoreFloat3(&look_, XMVector3TransformNormal(XMLoadFloat3(&look_), viewMatrix));
	//XMStoreFloat3(&right_, XMVector3TransformNormal(XMLoadFloat3(&right_), viewMatrix));
	XMStoreFloat3(&up_, XMVector3TransformNormal(XMLoadFloat3(&up_), viewMatrix));
}

/// 카메라의 로컬 Z축을 기준으로 회전하는 함수
/// Z축을 기준으로 회전하고 viewMatrix와 look,right,up을 갱신
///
/// 23.04.20 강석원
void Camera::Roll(float angle)
{
	// 회전축을 설정합니다.
	XMVECTOR axis = XMLoadFloat3(&look_);

	// Quaternion을 생성합니다.
	XMVECTOR quat = XMQuaternionRotationAxis(axis, angle);

	// 뷰 행렬을 회전시킵니다.
	XMMATRIX viewMatrix = XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix_), XMMatrixRotationQuaternion(quat));

	// 회전된 뷰 행렬을 적용합니다.
	//XMStoreFloat4x4(&viewMatrix_, viewMatrix);
	//XMStoreFloat3(&look_, XMVector3TransformNormal(XMLoadFloat3(&look_), viewMatrix));
	XMStoreFloat3(&right_, XMVector3TransformNormal(XMLoadFloat3(&right_), viewMatrix));
	XMStoreFloat3(&up_, XMVector3TransformNormal(XMLoadFloat3(&up_), viewMatrix));
}
*/
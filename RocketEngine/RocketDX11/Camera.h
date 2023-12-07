#pragma once

#include <DirectXMath.h>
#include <cmath>

/// <summary>
/// 카메라 클래스
/// 교수님 코드 참고하여 내가 필요할 것 같은것만 제작
/// 
/// 23.04.20 강석원 인재원
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
	float GetAspect() const;		// 카메라 비율 screen.width/screen.height
	float GetFovX() const;			// FovX값을 60분법으로 반환
	float GetRadianFovX() const;	// FovX값을 호도법으로 반환
	float GetFovY() const;			// FovY값을 60분법으로 반환
	float GetRadianFovY() const;	// FovY값을 호도법으로 반환

	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	void SetFrustum(float fovY, float aspect, float nearZ, float farZ);

	void LookAt(const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);

	void UpdateViewMatrix();

	DirectX::XMMATRIX GetViewMatrix() const;				// 카메라의 로컬좌표'계'를 반환
	DirectX::XMMATRIX GetProjectionMatrix() const;		// 카메라의 투영행렬을 반환
	DirectX::XMMATRIX GetViewProjectionMatrix() const;	// 위의 두 행렬을 적절히 곱해서 반환(view의 역행렬을 곱해주겠지?)

	void Strafe(float delta);
	void Walk(float delta);
	void WorldUpDown(float delta);

	void Yaw(float angle);
	void Pitch(float angle);
	void Roll(float angle);
	void RotateY(float angle);


private:
	DirectX::XMFLOAT3 position_;				// 카메라의 위치 (로컬 (월드 기준))
	DirectX::XMFLOAT3 look_;					// 카메라의 정면 (로컬)
	DirectX::XMFLOAT3 right_;				// 카메라의 오른쪽 (로컬)
	DirectX::XMFLOAT3 up_;					// 카메라의 위 (로컬)

	float nearZ_;					// frustum의 가까운 평면까지의 거리
	float farZ_;					// frustum의 먼 평면까지의 거리
	float aspect_;					// 가로 / 세로 비율
	float fovY_;					// fov각도를 60분법으로 갖고있음
	float nearWindowHeight_;		// frustum의 가까운 평면의 높이
	float farWindowHeight_;			// frustum의 먼 평면의 높이

	DirectX::XMFLOAT4X4 viewMatrix_;			// 카메라의 로컬좌표'계'
	DirectX::XMFLOAT4X4 projectionMatrix_;	// 카메라의 투영 행렬
};


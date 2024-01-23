#include "Collider.h"

#include <physx/PxPhysics.h>
#include <physx/PxPhysicsAPI.h>

namespace RocketEngine
{

	Collider::Collider(GameObject* owner)
		: Component(owner),
		_positionOffset(0.0f,0.0f,0.0f),
		_rotationOffset(1.0f,0.0f,0.0f,0.0f),
		_scaleOffset(1.0f,1.0f,1.0f),
		_pxScene(nullptr)
	{

	}

	RocketEngine::Vector3 Collider::GetPositionOffset() const
	{
		return _positionOffset;
	}

	RocketEngine::Quaternion Collider::GetRotationOffset() const
	{
		return _rotationOffset;
	}

	RocketEngine::Vector3 Collider::GetScaleOffset() const
	{
		return _scaleOffset;
	}

	void Collider::SetPositionOffset(Vector3 position)
	{
		_positionOffset = position;
	}

	void Collider::SetRotationOffset(Quaternion rotation)
	{
		_rotationOffset = rotation;
	}

	void Collider::SetRotationOffset(float angleX, float angleY, float angleZ)
	{
		float radianX = RMConvertToRadians(angleX);
		float radianY = RMConvertToRadians(angleY);
		float radianZ = RMConvertToRadians(angleZ);

		// 회전 각도를 반으로 나누어 준비합니다.
		float half_radianX = radianX * 0.5f;
		float half_radianY = radianY * 0.5f;
		float half_radianZ = radianZ * 0.5f;

		// 각 축의 쿼터니언 요소를 계산합니다.
		float sin_half_radianX = sin(half_radianX);
		float sin_half_radianY = sin(half_radianY);
		float sin_half_radianZ = sin(half_radianZ);
		float cos_half_radianX = cos(half_radianX);
		float cos_half_radianY = cos(half_radianY);
		float cos_half_radianZ = cos(half_radianZ);

		_rotationOffset.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;
		_rotationOffset.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		_rotationOffset.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		_rotationOffset.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;
	}

	void Collider::SetScaleOffset(Vector3 scale)
	{
		_scaleOffset = scale;
	}

	RocketEngine::Matrix Collider::GetOffsetTranslateMatrix() const
	{
		Matrix translateMatrix =
		{
			1,					0,					0,					0,
			0,					1,					0,					0,
			0,					0,					1,					0,
			_positionOffset.x,	_positionOffset.y,	_positionOffset.z,	1
		};

		return translateMatrix;
	}

	RocketEngine::Matrix Collider::GetOffsetScaleMatrix() const
	{
		Matrix scaleMatrix =
		{
			_scaleOffset.x,		0,					0,					0,
			0,					_scaleOffset.y,		0,					0,
			0,					0,					_scaleOffset.z,		0,
			0,					0,					0,					1
		};

		return scaleMatrix;
	}

	RocketEngine::Matrix Collider::GetOffsetRotationMatrix() const
	{
		Matrix rotationMatrix =
		{
			1.0f - 2.0f * (_rotationOffset.y * _rotationOffset.y + _rotationOffset.z * _rotationOffset.z),
			2.0f * (_rotationOffset.x * _rotationOffset.y + _rotationOffset.z * _rotationOffset.w),
			2.0f * (_rotationOffset.x * _rotationOffset.z - _rotationOffset.y * _rotationOffset.w),
			0,

			2.0f * (_rotationOffset.x * _rotationOffset.y - _rotationOffset.z * _rotationOffset.w),
			1.0f - 2.0f * (_rotationOffset.x * _rotationOffset.x + _rotationOffset.z * _rotationOffset.z),
			2.0f * (_rotationOffset.y * _rotationOffset.z + _rotationOffset.x * _rotationOffset.w),
			0,

			2.0f * (_rotationOffset.x * _rotationOffset.z + _rotationOffset.y * _rotationOffset.w),
			2.0f * (_rotationOffset.y * _rotationOffset.z - _rotationOffset.x * _rotationOffset.w),
			1.0f - 2.0f * (_rotationOffset.x * _rotationOffset.x + _rotationOffset.y * _rotationOffset.y),
			0,

			0,
			0,
			0,
			1
		};

		return rotationMatrix;
	}

	Matrix Collider::GetOffsetTM() const
	{
		Matrix result = GetOffsetScaleMatrix() * GetOffsetRotationMatrix() * GetOffsetTranslateMatrix();
		
		return result;
	}

	void Collider::SetPxScene(physx::PxScene* pxScene)
	{
		_pxScene = pxScene;
	}

	physx::PxScene* Collider::GetPxScene()
	{
		return _pxScene;
	}

}
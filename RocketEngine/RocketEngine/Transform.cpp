#include "Transform.h"
#include "GameObject.h"

namespace RocketEngine
{
	Transform::Transform(GameObject* owner)
		: Component(owner),
		_position(0.0f, 0.0f, 0.0f),
		_rotation(1.0f, 0.0f, 0.0f, 0.0f),
		_scale(1.0f, 1.0f, 1.0f),
		_parent(),
		_children()
	{

	}

	RocketEngine::Vector3 Transform::GetPosition() const
	{
		Vector4 result = { _position.x, _position.y, _position.z ,1.0f };
		if (_parent)
		{
			result = Vector4MultiplyMatrix(result, _parent->GetWorldTM());
		}

		return { result.x, result.y, result.z };
	}

	RocketEngine::Quaternion Transform::GetRotation() const
	{
		Quaternion result = _rotation;
		if (_parent)
		{
			result = QuaternionMultiply(result, _parent->GetRotation());
		}

		return result;
	}

	RocketEngine::Vector3 Transform::GetEuler() const
	{
		Quaternion rot = GetRotation();

		// 쿼터니언의 요소 추출
		float w = rot.w;
		float x = rot.x;
		float y = rot.y;
		float z = rot.z;

		// ZYX 회전 순서로 오일러 각 계산
		float yaw = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
		float pitch = std::asin(2.0f * (w * y - z * x));
		float roll = std::atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));

		// 라디안을 도(degree)로 변환하여 반환
		const float toDegree = 180.0f / 3.14159265358979323846f;
		return { roll * toDegree, pitch * toDegree,  yaw * toDegree };
	}

	RocketEngine::Vector3 Transform::GetScale() const
	{
		Vector3 result = _scale;
		if (_parent)
		{
			result.x *= _parent->GetScale().x;
			result.y *= _parent->GetScale().y;
			result.z *= _parent->GetScale().z;
		}

		return result;
	}

	void Transform::SetPosition(const Vector3& position)
	{
		SetPosition(position.x, position.y, position.z);
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		Vector4 result = { x,y,z ,1.0f };
		if (_parent)
		{
			result = Vector4MultiplyMatrix(result, _parent->GetWorldTM().Inverse());
		}

		_position.x = result.x;
		_position.y = result.y;
		_position.z = result.z;
	}

	void Transform::SetRotation(const Quaternion& quaternion)
	{
		SetRotation(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
	}

	void Transform::SetRotation(float w, float x, float y, float z)
	{
		Quaternion result = { w,x,y,z };
		if (_parent)
		{
			result = QuaternionMultiply(result, _parent->GetRotation().conjugate());
		}

		_rotation = result;
	}

	void Transform::SetRotationEuler(const Vector3& euler)
	{
		SetRotationEuler(euler.x, euler.y, euler.z);
	}

	void Transform::SetRotationEuler(float angleX, float angleY, float angleZ)
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

		Quaternion rot;

		rot.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;
		rot.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		rot.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		rot.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;

		SetRotation(rot);
	}

	void Transform::SetScale(const Vector3& scale)
	{
		SetScale(scale.x, scale.y, scale.z);
	}

	void Transform::SetScale(float x, float y, float z)
	{
		Vector4 result = { x,y,z ,1.0f };
		if (_parent)
		{
			Matrix scaleInverseMatrix = _parent->GetWorldScaleMatrix();
			scaleInverseMatrix.m[0][0] = 1 / scaleInverseMatrix.m[0][0];
			scaleInverseMatrix.m[1][1] = 1 / scaleInverseMatrix.m[1][1];
			scaleInverseMatrix.m[2][2] = 1 / scaleInverseMatrix.m[2][2];
			scaleInverseMatrix.m[3][3] = 1 / scaleInverseMatrix.m[3][3];
			result = Vector4MultiplyMatrix(result, scaleInverseMatrix);
		}

		_scale.x = result.x;
		_scale.y = result.y;
		_scale.z = result.z;
	}

	RocketEngine::Vector3 Transform::GetLocalPosition() const
	{
		return _position;
	}

	RocketEngine::Quaternion Transform::GetLocalRotation() const
	{
		return _rotation;
	}

	RocketEngine::Vector3 Transform::GetLocalEuler() const
	{
		// 쿼터니언의 요소 추출
		float w = _rotation.w;
		float x = _rotation.x;
		float y = _rotation.y;
		float z = _rotation.z;

		// ZYX 회전 순서로 오일러 각 계산
		float yaw = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
		float pitch = std::asin(2.0f * (w * y - z * x));
		float roll = std::atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));

		// 라디안을 도(degree)로 변환하여 반환
		const float toDegree = 180.0f / 3.14159265358979323846f;
		return { roll * toDegree, pitch * toDegree,  yaw * toDegree };
	}

	RocketEngine::Vector3 Transform::GetLocalScale() const
	{
		return _scale;
	}

	RocketEngine::Vector3& Transform::GetLocalPositionRef()
	{
		return _position;
	}

	RocketEngine::Quaternion& Transform::GetLocalRotationRef()
	{
		return _rotation;
	}

	RocketEngine::Vector3& Transform::GetLocalScaleRef()
	{
		return _scale;
	}	

	void Transform::SetLocalPosition(const Vector3& position)
	{
		_position = position;
	}

	void Transform::SetLocalPosition(float x, float y, float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	void Transform::SetLocalRotation(const Quaternion& quaternion)
	{
		_rotation = quaternion;
	}

	void Transform::SetLocalRotation(float w, float x, float y, float z)
	{
		Quaternion temp = QuaternionNormalize({ w,x,y,z });

		_rotation.w = temp.w;
		_rotation.x = temp.x;
		_rotation.y = temp.y;
		_rotation.z = temp.z;
	}

	void Transform::SetLocalRotationEuler(const Vector3& euler)
	{
		SetLocalRotationEuler(euler.x, euler.y, euler.z);
	}

	void Transform::SetLocalRotationEuler(float angleX, float angleY, float angleZ)
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

		_rotation.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;
		_rotation.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		_rotation.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		_rotation.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;
	}

	void Transform::SetLocalRotationEulerXZConvert(float angleX, float angleY, float angleZ)
	{
		SetLocalRotationEuler(angleZ, angleY + 90.0f, angleX);
	}

	void Transform::SetLocalScale(const Vector3& scale)
	{
		_scale = scale;
	}

	void Transform::SetLocalScale(float x, float y, float z)
	{
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;
	}

	RocketEngine::Vector3 Transform::GetForward() const
	{
		Vector4 temp = Vector4MultiplyMatrix(Vector4(0.0f, 0.0f, 1.0f, 0.0f), GetWorldRotationMatrix());
		return Vector3(temp.x, temp.y, temp.z);
	}

	RocketEngine::Vector3 Transform::GetUp() const
	{
		Vector4 temp = Vector4MultiplyMatrix(Vector4(0.0f, 1.0f, 0.0f, 0.0f), GetWorldRotationMatrix());
		return Vector3(temp.x, temp.y, temp.z);
	}

	RocketEngine::Vector3 Transform::GetRight() const
	{
		Vector4 temp = Vector4MultiplyMatrix(Vector4(1.0f, 0.0f, 0.0f, 0.0f), GetWorldRotationMatrix());
		return Vector3(temp.x, temp.y, temp.z);
	}

	RocketEngine::Matrix Transform::GetLocalScaleMatrix() const
	{
		Matrix scaleMatrix =
		{
			_scale.x,	0,		0,		0,
			0,		_scale.y,	0,		0,
			0,		0,		_scale.z,	0,
			0,		0,		0,			1
		};

		return scaleMatrix;
	}

	RocketEngine::Matrix Transform::GetLocalRotationMatrix() const
	{
		Matrix rotationMatrix =
		{
			1.0f - 2.0f * (_rotation.y * _rotation.y + _rotation.z * _rotation.z),
			2.0f * (_rotation.x * _rotation.y + _rotation.z * _rotation.w),
			2.0f * (_rotation.x * _rotation.z - _rotation.y * _rotation.w),
			0,

			2.0f * (_rotation.x * _rotation.y - _rotation.z * _rotation.w),
			1.0f - 2.0f * (_rotation.x * _rotation.x + _rotation.z * _rotation.z),
			2.0f * (_rotation.y * _rotation.z + _rotation.x * _rotation.w),
			0,

			2.0f * (_rotation.x * _rotation.z + _rotation.y * _rotation.w),
			2.0f * (_rotation.y * _rotation.z - _rotation.x * _rotation.w),
			1.0f - 2.0f * (_rotation.x * _rotation.x + _rotation.y * _rotation.y),
			0,

			0,
			0,
			0,
			1
		};

		return rotationMatrix;
	}

	RocketEngine::Matrix Transform::GetLocalTranslateMatrix() const
	{
		Matrix translateMatrix =
		{
			1,				0,				0,				0,
			0,				1,				0,				0,
			0,				0,				1,				0,
			_position.x,	_position.y,	_position.z,	1
		};

		return translateMatrix;
	}

	RocketEngine::Matrix Transform::GetWorldScaleMatrix() const
	{
		Matrix result = GetLocalScaleMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldScaleMatrix();
		}

		return result;
	}

	RocketEngine::Matrix Transform::GetWorldRotationMatrix() const
	{
		Matrix result = GetLocalRotationMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldRotationMatrix();
		}

		return result;
	}

	RocketEngine::Matrix Transform::GetWorldTranslateMatrix() const
	{
		Matrix result = GetLocalTranslateMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldTranslateMatrix();
		}

		return result;
	}

	RocketEngine::Matrix Transform::GetWorldTM() const
	{
		Matrix result = GetLocalScaleMatrix() * GetLocalRotationMatrix() * GetLocalTranslateMatrix();
		
		if (_parent)
		{
			result *= _parent->GetWorldTM();
		}

		return result;
	}

	RocketEngine::Vector3X3 Transform::Get2DLocalScaleMatrix() const
	{
		Vector3X3 scaleMatrix =
		{
			_scale.x,	0,		0,
			0,		_scale.y,	0,
			0,			0,		1
		};

		return scaleMatrix;
	}

	RocketEngine::Vector3X3 Transform::Get2DLocalRotationMatrix() const
	{
		Vector3X3 rotationMatrix =
		{
			1.0f - 2.0f * (_rotation.y * _rotation.y + _rotation.z * _rotation.z),
			2.0f * (_rotation.x * _rotation.y + _rotation.z * _rotation.w),
			0,

			2.0f * (_rotation.x * _rotation.y - _rotation.z * _rotation.w),
			1.0f - 2.0f * (_rotation.x * _rotation.x + _rotation.z * _rotation.z),
			0,

			0,
			0,
			1
		};

		return rotationMatrix;
	}

	RocketEngine::Vector3X3 Transform::Get2DLocalTranslateMatrix() const
	{
		Vector3X3 translateMatrix =
		{
			1,				0,				0,
			0,				1,				0,
			_position.x,	_position.y,	1
		};

		return translateMatrix;
	}

	RocketEngine::Vector3X3 Transform::Get2DWorldTM() const
	{
		Vector3X3 result = Get2DLocalScaleMatrix() * Get2DLocalRotationMatrix() * Get2DLocalTranslateMatrix();

		if (_parent)
		{
			result *= _parent->Get2DWorldTM();
		}

		return result;
	}

	void Transform::Translate(const Vector3& position)
	{
		_position.x += position.x;
		_position.y += position.y;
		_position.z += position.z;
	}

	void Transform::Translate(float x, float y, float z)
	{
		_position.x += x;
		_position.y += y;
		_position.z += z;
	}

	void Transform::Rotate(float angleX, float angleY, float angleZ)
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

		// 쿼터니언 요소를 계산합니다.
		Vector4 rotQuat;
		rotQuat.x = sin_half_radianX * cos_half_radianY * cos_half_radianZ + cos_half_radianX * sin_half_radianY * sin_half_radianZ;
		rotQuat.y = cos_half_radianX * sin_half_radianY * cos_half_radianZ - sin_half_radianX * cos_half_radianY * sin_half_radianZ;
		rotQuat.z = cos_half_radianX * cos_half_radianY * sin_half_radianZ + sin_half_radianX * sin_half_radianY * cos_half_radianZ;
		rotQuat.w = cos_half_radianX * cos_half_radianY * cos_half_radianZ - sin_half_radianX * sin_half_radianY * sin_half_radianZ;

		// 원본 쿼터니언과 회전 쿼터니언의 곱으로 결과 쿼터니언을 계산합니다.
		Quaternion result;
		result.x = _rotation.w * rotQuat.x + _rotation.x * rotQuat.w + _rotation.y * rotQuat.z - _rotation.z * rotQuat.y;
		result.y = _rotation.w * rotQuat.y - _rotation.x * rotQuat.z + _rotation.y * rotQuat.w + _rotation.z * rotQuat.x;
		result.z = _rotation.w * rotQuat.z + _rotation.x * rotQuat.y - _rotation.y * rotQuat.x + _rotation.z * rotQuat.w;
		result.w = _rotation.w * rotQuat.w - _rotation.x * rotQuat.x - _rotation.y * rotQuat.y - _rotation.z * rotQuat.z;

		// 결과를 저장합니다.
		_rotation = result;
	}

	void Transform::Rotate(Quaternion quaternion)
	{
		_rotation = QuaternionMultiply(_rotation, quaternion);
	}

	/// <summary>
	/// target과 up벡터를 주면 그것을 기준으로 카메라가 바라본다.
	/// 
	/// 23.04.20 강석원 인재원
	/// </summary>
	/// <param name="target">바라볼 타겟</param>
	/// <param name="up"> 카메라가 right vector를 구할때 사용할 up 벡터</param>
	void Transform::LookAt(const Vector3& target, const Vector3& up)
	{
// 		Vector3 look = Vector3Normalize(target - gameObject->transform.GetPosition());
// 		Vector3 right = Vector3Normalize(Vector3Cross(up, look));
// 		Vector3 up = Vector3Normalize(Vector3Cross(look, right));
// 
// 		_look
// 
// 			XMStoreFloat3(&look_, lookVector);
// 		XMStoreFloat3(&right_, rightVector);
// 		XMStoreFloat3(&up_, upVector);
	}

	void Transform::LookAt(const Vector3& pos, const Vector3& target, const Vector3& up)
	{

	}

	void Transform::SetParent(Transform* parent)
	{
		auto name = gameObject->objName;
		if (name != "playerLeg" && name != "playerBody" && name != "playerHead")
		{
			Vector4 tempPos = Vector4MultiplyMatrix({ GetPosition(), 1.0f }, parent->GetWorldTM().Inverse());
			_position.x = tempPos.x;
			_position.y = tempPos.y;
			_position.z = tempPos.z;

			_rotation = QuaternionMultiply(GetRotation(), parent->GetRotation().conjugate());
		}

		Vector4 tempScale = Vector4MultiplyMatrix({ GetScale(), 1.0f }, parent->GetWorldScaleMatrix().Inverse());
		_scale.x = tempScale.x;
		_scale.y = tempScale.y;
		_scale.z = tempScale.z;

		_parent = parent;
		_parent->AddChild(this);
	}

	void Transform::SetParent(GameObject* parentObj)
	{
		SetParent(&(parentObj->transform));
	}

	Transform* Transform::GetParent()
	{
		return _parent;
	}

	Transform* Transform::GetChild(int index)
	{
		if (_children.size() <= index)
		{
			return nullptr;
		}

		return _children[index];
	}

	void Transform::ReleaseParent()
	{
		if (!_parent)
		{
			return;
		}

		Vector4 tempPos = Vector4MultiplyMatrix({ _position.x,_position.y,_position.z,1.0f }, _parent->GetWorldTM());
		SetLocalPosition(tempPos.x, tempPos.y, tempPos.z);

		SetLocalRotation(QuaternionMultiply(_rotation, _parent->GetRotation()));
		Vector4 tempScale = Vector4MultiplyMatrix({ _scale,1.0f }, _parent->GetWorldScaleMatrix());
		SetLocalScale(tempScale.x, tempScale.y, tempScale.z);

		_parent->ReleaseChild(this);
		_parent = nullptr;
	}

	void Transform::ReleaseChild(Transform* child)
	{
		std::erase_if(_children, [child](Transform* tr) {return tr == child; });
	}

	std::vector<Transform*>& Transform::GetChildrenVec()
	{
		return _children;
	}

	void Transform::AddChild(Transform* child)
	{
		_children.push_back(child);
	}

}

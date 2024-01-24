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

	Vector3 Transform::GetPosition() const
	{
		Vector3 result = { _position.x, _position.y, _position.z };

		if (_parent)
		{
			result = Vector3::Transform(result, _parent->GetWorldTM());
		}

		return result;
	}

	Quaternion Transform::GetRotation() const
	{
		Quaternion result = _rotation;

		if (_parent)
		{
			result = Quaternion::Concatenate(result, _parent->GetRotation());
		}

		return result;
	}

	Vector3 Transform::GetEuler() const
	{
		return GetRotation().ToEuler();
	}

	Vector3 Transform::GetScale() const
	{
		Vector3 result = _scale;

		if (_parent)
		{
			Vector3 parentScale = _parent->GetScale();
			result.x *= parentScale.x;
			result.y *= parentScale.y;
			result.z *= parentScale.z;
		}

		return result;
	}

	void Transform::SetPosition(const Vector3& position)
	{
		SetPosition(position.x, position.y, position.z);
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		Vector3 result = { x,y,z };

		if (_parent)
		{
			result = Vector3::Transform(result, _parent->GetWorldTM().Invert());
		}

		_position = result;
	}

	void Transform::SetRotation(const Quaternion& quaternion)
	{
		SetRotation(quaternion.w, quaternion.x, quaternion.y, quaternion.z);
	}

	void Transform::SetRotation(float x, float y, float z, float w)
	{
		Quaternion result = { x,y,z,w };

		if (_parent)
		{
			Quaternion parentRot = _parent->GetRotation();
			parentRot.Conjugate();

			result = Quaternion::Concatenate(result, parentRot);
		}

		_rotation = result;
	}

	void Transform::SetRotationEuler(const Vector3& euler)
	{
		SetRotationEuler(euler.x, euler.y, euler.z);
	}

	void Transform::SetRotationEuler(float angleX, float angleY, float angleZ)
	{
		SetRotation(Quaternion::CreateFromYawPitchRoll(angleY, angleX, angleZ));
	}

	void Transform::SetScale(const Vector3& scale)
	{
		SetScale(scale.x, scale.y, scale.z);
	}

	void Transform::SetScale(float x, float y, float z)
	{
		Vector3 result = { x,y,z };

		if (_parent)
		{
			Vector3 parentScale = _parent->GetScale();
			result.x /= parentScale.x;
			result.y /= parentScale.y;
			result.z /= parentScale.z;
		}

		_scale = result;
	}

	Vector3 Transform::GetLocalPosition() const
	{
		return _position;
	}

	Quaternion Transform::GetLocalRotation() const
	{
		return _rotation;
	}

	Vector3 Transform::GetLocalEuler() const
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

	Vector3 Transform::GetLocalScale() const
	{
		return _scale;
	}

	Vector3& Transform::GetLocalPositionRef()
	{
		return _position;
	}

	Quaternion& Transform::GetLocalRotationRef()
	{
		return _rotation;
	}

	Vector3& Transform::GetLocalScaleRef()
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

	void Transform::SetLocalRotation(float x, float y, float z, float w)
	{
		_rotation.x = x;
		_rotation.y = y;
		_rotation.z = z;
		_rotation.w = w;

		_rotation.Normalize();
	}

	void Transform::SetLocalRotationEuler(const Vector3& euler)
	{
		SetLocalRotationEuler(euler.x, euler.y, euler.z);
	}

	void Transform::SetLocalRotationEuler(float angleX, float angleY, float angleZ)
	{
		_rotation = Quaternion::CreateFromYawPitchRoll(angleX, angleY, angleZ);
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

	Vector3 Transform::GetForward() const
	{
		return Vector3::Transform(Vector3::Forward, GetRotation());
	}

	Vector3 Transform::GetUp() const
	{
		return Vector3::Transform(Vector3::Up, GetRotation());
	}

	Vector3 Transform::GetRight() const
	{
		return Vector3::Transform(Vector3::Right, GetRotation());
	}

	Matrix Transform::GetLocalScaleMatrix() const
	{
		return Matrix::CreateScale(_scale);
	}

	Matrix Transform::GetLocalRotationMatrix() const
	{
		return Matrix::CreateFromQuaternion(_rotation);
	}

	Matrix Transform::GetLocalTranslateMatrix() const
	{
		return Matrix::CreateTranslation(_position);
	}

	Matrix Transform::GetWorldScaleMatrix() const
	{
		Matrix result = GetLocalScaleMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldScaleMatrix();
		}

		return result;
	}

	Matrix Transform::GetWorldRotationMatrix() const
	{
		Matrix result = GetLocalRotationMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldRotationMatrix();
		}

		return result;
	}

	Matrix Transform::GetWorldTranslateMatrix() const
	{
		Matrix result = GetLocalTranslateMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldTM();
		}

		return result;
	}

	Matrix Transform::GetWorldTM() const
	{
		// 이거 한번 써봐야 할듯?
		// return Matrix::CreateWorld(_position, GetForward(), GetUp());
		
		Matrix result;
		result *= GetLocalScaleMatrix();
		result *= GetLocalRotationMatrix();
		result *= GetLocalTranslateMatrix();

		if (_parent)
		{
			result *= _parent->GetWorldTM();
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
		Quaternion rot = Quaternion::CreateFromYawPitchRoll(angleX, angleY, angleZ);
		_rotation = Quaternion::Concatenate(_rotation, rot);
	}

	void Transform::Rotate(Quaternion quaternion)
	{
		_rotation = Quaternion::Concatenate(_rotation, quaternion);
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
		Vector3 forward = target - GetPosition();
		forward.Normalize();
		Vector3 right = up.Cross(forward);
		right.Normalize();
		Vector3 upVec = forward.Cross(right);
		upVec.Normalize();

		_rotation = Quaternion::CreateFromRotationMatrix(Matrix::CreateWorld(GetPosition(), forward, upVec));
	}

	void Transform::SetParent(Transform* parent)
	{
		_position = Vector3::Transform(_position, parent->GetWorldTM().Invert());
		Quaternion parentRot = parent->GetRotation();
		parentRot.Conjugate();
		_rotation = Quaternion::Concatenate(_rotation, parentRot);
		_scale = Vector3::Transform(_scale, parent->GetWorldScaleMatrix().Invert());

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

		SetLocalPosition(Vector3::Transform(_position, _parent->GetWorldTM()));
		SetLocalRotation(Quaternion::Concatenate(_rotation, _parent->GetRotation()));
		SetLocalScale(Vector3::Transform(_scale, _parent->GetWorldScaleMatrix()));

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

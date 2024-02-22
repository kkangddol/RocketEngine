#include "Transform.h"


namespace Rocket::Core
{
	Transform::Transform()
		: _position(Vector3::Zero)
		, _rotation(Quaternion::Identity)
		, _scale(Vector3::One)
		, _parent(nullptr)
		, _children()
	{

	}

	void Transform::SetPosition(const Vector3& position)
	{
		Vector3 result = position;

		if (_parent)
		{
			result = Vector3::Transform(result, _parent->GetWorldTM().Invert());
		}

		_position = result;
	}

	void Transform::SetRotation(const Quaternion& rotation)
	{
		Quaternion result = rotation;

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
		SetRotation(Quaternion::CreateFromYawPitchRoll(euler));
	}

	void Transform::SetScale(const Vector3& scale)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Vector3 Transform::GetPosition() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Quaternion Transform::GetRotation() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Vector3 Transform::GetEuler() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Vector3 Transform::GetScale() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::SetLocalPosition(const Vector3& position)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::SetLocalRotation(const Quaternion& rotation)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::SetLocalRotationEuler(const Vector3& euler)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::SetLocalScale(const Vector3& scale)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Vector3 Transform::GetLocalPosition() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Quaternion Transform::GetLocalRotation() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Vector3 Transform::GetLocalEuler() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Vector3 Transform::GetLocalScale() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Vector3 Transform::GetForward() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Vector3 Transform::GetUp() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Vector3 Transform::GetRight() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Matrix Transform::GetScaleMatrix() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Matrix Transform::GetRotationMatrix() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Matrix Transform::GetTranslationMatrix() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Matrix Transform::GetWorldTM() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Matrix Transform::GetLocalScaleMatrix() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Matrix Transform::GetLocalRotationMatrix() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Matrix Transform::GetLocalTranslationMatrix() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Matrix Transform::GetLocalTM() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::Translate(const Vector3& translation)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::Rotate(const Quaternion& rotation)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::LookAt(const Vector3& target, const Vector3& up /*= Vector3::Up*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::LookAt(const ITransform* target, const Vector3& up /*= Vector3::Up*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::RotateAround(const Vector3& point, const Vector3& axis, float angle)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::Scale(const Vector3& scale)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::SetParent(ITransform* parent)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	ITransform* Transform::GetParent() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	ITransform* Transform::GetChild(int index) const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	int Transform::GetChildCount() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::AddChild(ITransform* child)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void Transform::RemoveChild(ITransform* child)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}
}


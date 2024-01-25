#pragma once
#include <vector>

#include "DLLExporter.h"
#include "Component.h"
#include "MathHeader.h"

namespace Rocket
{
	class GameObject;

	/// <summary>
	/// GameObject�� ���� Transform Ŭ����.
	/// 
	/// 23.06.29 ������ �����.
	/// </summary>
	class ROCKET_API Transform final : public Component
	{
		/// ������.
	public:
		Transform();		// ������ �� �� Entity�� �������� �����Ѵ�.

		/// Get World Position, Rotation, Scale
	public:
		Vector3 GetPosition() const;	// world ����
		Quaternion GetRotation() const;	// world ���� ���ʹϾ�
		Vector3 GetEuler() const;		// world ���� 60�й�
		Vector3 GetScale() const;		// world ����

		/// Set World Position, Rotation, Scale
	public:
		void SetPosition(const Vector3& position);
		void SetPosition(float x, float y, float z);
		void SetRotation(const Quaternion& quaternion);
		void SetRotation(float x, float y, float z, float w);
		void SetRotationEuler(const Vector3& euler);
		void SetRotationEuler(float angleX, float angleY, float angleZ);
		void SetScale(const Vector3& scale);
		void SetScale(float x, float y, float z);
		

		/// Tween���� ������ �����ϱ� ����.(�ӽ�)
		/// ������ �����ϱ� �� ���� ������ ��� �����غ���.
		/// 23.07.26 ������ && ������ �����
	public:
		Vector3& GetLocalPositionRef();
		Quaternion& GetLocalRotationRef();
		Vector3& GetLocalScaleRef();

		/// Get Local Position, Rotation, Scale ����
	public:
		Vector3 GetLocalPosition() const;
		Quaternion GetLocalRotation() const;
		Vector3 GetLocalEuler() const;
		Vector3 GetLocalScale() const;

	public:
		void SetLocalPosition(const Vector3& position);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalRotation(const Quaternion& quaternion);
		void SetLocalRotation(float x, float y, float z, float w);
		void SetLocalRotationEuler(const Vector3& euler);
		void SetLocalRotationEuler(float angleX, float angleY, float angleZ);
		void SetLocalRotationEulerXZConvert(float angleX, float angleY, float angleZ);
		void SetLocalScale(const Vector3& scale);
		void SetLocalScale(float x, float y, float z);

	public:
		/// Forwar,Up,Right vector (World ����)
		Vector3 GetForward() const;
		Vector3 GetUp() const;
		Vector3 GetRight() const;

		Matrix GetLocalScaleMatrix() const;
		Matrix GetLocalRotationMatrix() const;
		Matrix GetLocalTranslateMatrix() const;

		Matrix GetWorldScaleMatrix() const;
		Matrix GetWorldRotationMatrix() const;
		Matrix GetWorldTranslateMatrix() const;

		Matrix GetWorldTM() const;

	public:
		void Translate(const Vector3& position);
		void Translate(float x, float y, float z);
		void Rotate(Quaternion quaternion);					// ���ʹϾ� ���� ȸ��
		void Rotate(float angleX, float angleY, float angleZ);	// ���Ϸ� �� ���� ȸ��, radian

		void LookAt(const Vector3& target, const Vector3& up);

	private:
		Vector3 _position;
		Quaternion _rotation;		// ���ʹϾ�
		Vector3 _scale;

		/// ��������.
	public:
		void SetParent(Transform* parent);
		void SetParent(GameObject* parentObj);
		Transform* GetParent();
		Transform* GetChild(int index);

		void ReleaseParent();

	private:
		void ReleaseChild(Transform* child);
		
	public:
		std::vector<Transform*>& GetChildrenVec();

	private:
		void AddChild(Transform* child);

	private:
		Transform* _parent;
		std::vector<Transform*> _children;
	};
}

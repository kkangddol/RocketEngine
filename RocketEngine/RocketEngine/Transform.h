#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include <vector>

#include "MathHeader.h"

namespace RocketEngine
{
	class GameObject;

	/// <summary>
	/// GameObject에 붙을 Transform 클래스.
	/// 
	/// 23.06.29 강석원 인재원.
	/// </summary>
	class ROCKET_API Transform final : public Component
	{
		/// 생성자.
	public:
		Transform(GameObject* owner);		// 생성할 때 내 Entity가 무엇인지 저장한다.

		/// Get World Position, Rotation, Scale
	public:
		Vector3 GetPosition() const;	// world 기준
		Quaternion GetRotation() const;	// world 기준 쿼터니언
		Vector3 GetEuler() const;		// world 기준 60분법
		Vector3 GetScale() const;		// world 기준

		/// Set World Position, Rotation, Scale
	public:
		void SetPosition(const Vector3& position);
		void SetPosition(float x, float y, float z);
		void SetRotation(const Quaternion& quaternion);
		void SetRotation(float w, float x, float y, float z);
		void SetRotationEuler(const Vector3& euler);
		void SetRotationEuler(float angleX, float angleY, float angleZ);
		void SetScale(const Vector3& scale);
		void SetScale(float x, float y, float z);
		

		/// Tween에게 참조로 전달하기 위함.(임시)
		/// 참조로 전달하기 더 좋고 안전한 방법 생각해보셈.
		/// 23.07.26 강석원 && 오수안 인재원
	public:
		Vector3& GetLocalPositionRef();
		Quaternion& GetLocalRotationRef();
		Vector3& GetLocalScaleRef();

		/// Get Local Position, Rotation, Scale 정보
	public:
		Vector3 GetLocalPosition() const;
		Quaternion GetLocalRotation() const;
		Vector3 GetLocalEuler() const;
		Vector3 GetLocalScale() const;

	public:
		void SetLocalPosition(const Vector3& position);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalRotation(const Quaternion& quaternion);
		void SetLocalRotation(float w, float x, float y, float z);
		void SetLocalRotationEuler(const Vector3& euler);
		void SetLocalRotationEuler(float angleX, float angleY, float angleZ);
		void SetLocalRotationEulerXZConvert(float angleX, float angleY, float angleZ);
		void SetLocalScale(const Vector3& scale);
		void SetLocalScale(float x, float y, float z);

	public:
		/// Forwar,Up,Right vector (World 기준)
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

		// UI에서 사용할 2D Transform 정보 Get.
	public:
		Vector3X3 Get2DLocalScaleMatrix() const;
		Vector3X3 Get2DLocalRotationMatrix() const;
		Vector3X3 Get2DLocalTranslateMatrix() const;

		Vector3X3 Get2DWorldTM() const;

	public:
		void Translate(const Vector3& position);
		void Translate(float x, float y, float z);
		void Rotate(Quaternion quaternion);					// 쿼터니언 기준 회전
		void Rotate(float angleX, float angleY, float angleZ);	// 오일러 각 기준 회전, radian

		void LookAt(const Vector3& target, const Vector3& up);
		void LookAt(const Vector3& pos, const Vector3& target, const Vector3& up);

	private:
		Vector3 _position;
		Quaternion _rotation;		// 쿼터니언
		Vector3 _scale;

		/// 계층구조.
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

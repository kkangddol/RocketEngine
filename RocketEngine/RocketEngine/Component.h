#pragma once
#include "DLLExporter.h"
#include "IComponent.h"

namespace Rocket
{
	class GameObject;
}

namespace Rocket::Core
{
	class ObjectSystem;
}

/// <summary>
/// ������Ʈ�� ���̽� Ŭ����.
/// IComponent �������̽��� ��ӹް� �� �������� �д�.
/// �ٸ� ������Ʈ�� �� Ŭ������ ��ӹ޴´�.
/// �̰� �³�? -> ��ɺ��� �������̽��� ����?
/// 
/// 23.06.29 ������ �����.
/// </summary>
namespace Rocket
{
	class ROCKET_API Component : public Rocket::Core::IComponent
	{
		friend class GameObject;
		friend class Rocket::Core::ObjectSystem;
	
	public:
		Component(GameObject* owner)
			: gameObject(owner)
		{

		}

		//virtual ~Component() = default;

	protected:
		virtual void Awake() override {}
		virtual void Start() override {}
		virtual void FixedUpdate() override {}
		virtual void Update() override {}
		virtual void LateUpdate() override {}
		virtual void Finalize() override {}

	protected:
		virtual void OnDestroy() override {}
		virtual void OnCollisionEnter() override {}
		virtual void OnCollisionStay() override {}
		virtual void OnCollisionExit() override {}


	public:
		GameObject* gameObject;

	private:
		void SetRestart();

	private:
		bool _isStarted = false;
	};
}

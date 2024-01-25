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
	class GraphicsSystem;
}

/// <summary>
/// 컴포넌트의 베이스 클래스.
/// IComponent 인터페이스를 상속받고 빈 구현으로 둔다.
/// 다른 컴포넌트가 이 클래스를 상속받는다.
/// 이게 맞냐? -> 기능별로 인터페이스를 나눠?
/// 
/// 23.06.29 강석원 인재원.
/// </summary>
namespace Rocket
{
	class ROCKET_API Component : public Rocket::Core::IComponent
	{
		friend class GameObject;
		friend class Rocket::Core::ObjectSystem;
		friend class Rocket::Core::GraphicsSystem;
	
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

	protected:
		virtual void SetRenderData() override {}


	public:
		GameObject* gameObject;

	private:
		void SetRestart();

	private:
		bool _isStarted = false;
	};
}

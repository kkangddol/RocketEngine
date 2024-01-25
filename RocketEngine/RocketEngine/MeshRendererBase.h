#pragma once
#include <string>

#include "Component.h"
#include "DLLExporter.h"
#include "../RocketGraphicsInterface/IRenderable.h"

namespace Rocket
{
	class GameObject;
}

namespace Rocket::Core
{

	/// <summary>
	/// Renderer 컴포넌트들이 상속받을 인터페이스.
	/// 근데 얘는 abstract인데 Component 베이스클래스를 상속받아도 되는건가?
	/// 
	/// 23.06.29 강석원 인재원.
	/// </summary>
	class ROCKET_API MeshRendererBase : public Component
	{
	protected:
		virtual void SetRenderData() override;

	protected:
		Core::IRenderable* _renderable;
	};
}

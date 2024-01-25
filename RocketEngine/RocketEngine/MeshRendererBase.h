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
	/// Renderer ������Ʈ���� ��ӹ��� �������̽�.
	/// �ٵ� ��� abstract�ε� Component ���̽�Ŭ������ ��ӹ޾Ƶ� �Ǵ°ǰ�?
	/// 
	/// 23.06.29 ������ �����.
	/// </summary>
	class ROCKET_API MeshRendererBase : public Component
	{
	protected:
		virtual void SetRenderData() override;

	protected:
		Core::IRenderable* _renderable;
	};
}

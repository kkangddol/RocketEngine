#pragma once
#include "MeshRendererBase.h"
#include "DLLExporter.h"
#include "../RocketGraphicsInterface/IMeshRenderer.h"

namespace Rocket
{
	class GameObject;

	class ROCKET_API MeshRenderer final : public Core::MeshRendererBase
	{
	public:
		MeshRenderer();

	protected:
		Core::IMeshRenderer* _meshRenderer;
	};
}


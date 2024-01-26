#pragma once
#include "MeshRendererBase.h"
#include "DLLExporter.h"
#include "../RocketGraphicsInterface/GraphicsEnum.h"

namespace Rocket::Core
{
	class IMeshRenderer;
}

namespace Rocket
{
	class GameObject;

	class ROCKET_API MeshRenderer final : public Core::MeshRendererBase
	{
	public:
		MeshRenderer();
		void SetMesh(eMeshType meshType);

	protected:
		Core::IMeshRenderer* _meshRenderer;
	};
}


#pragma once
#include "DLLExporter.h"
#include "MeshRendererBase.h"

namespace Rocket
{
	class GameObject;

	class ROCKET_API SkinnedMeshRenderer final : public Rocket::Core::MeshRendererBase
	{
	public:
		SkinnedMeshRenderer(GameObject* owner);
	};
}

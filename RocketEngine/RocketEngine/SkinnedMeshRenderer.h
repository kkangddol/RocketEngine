#pragma once
#include "DLLExporter.h"
#include "MeshRendererBase.h"

namespace RocketEngine
{
	class GameObject;

	class ROCKET_API SkinnedMeshRenderer final : public RocketCore::MeshRendererBase
	{
	public:
		SkinnedMeshRenderer(GameObject* owner);
	};
}

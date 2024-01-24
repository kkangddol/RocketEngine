#pragma once
#include "DLLExporter.h"
#include "MeshRendererBase.h"

namespace Rocket
{
	class GameObject;

	class ROCKET_API MeshRenderer final : public Rocket::Core::MeshRendererBase
	{
	public:
		MeshRenderer(GameObject* owner);
	};
}


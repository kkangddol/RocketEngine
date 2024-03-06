#pragma once
#include <string>

#include "DLLExporter.h"
#include "MeshRendererBase.h"

namespace Rocket::Core
{
	class ISkinnedMeshRenderer;
}

namespace Rocket
{
	class GameObject;

	class ROCKET_API SkinnedMeshRenderer final : public Core::MeshRendererBase
	{
	public:
		SkinnedMeshRenderer();
		void SetMesh(std::string fileName);
		void SetTexture(std::string fileName);

	protected:
		virtual void UpdateRenderData() override;

	private:
		Core::ISkinnedMeshRenderer* _skinnedMeshRenderer;
	};
}

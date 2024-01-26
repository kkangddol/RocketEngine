#include "MeshRenderer.h"
#include "GraphicsSystem.h"
#include "../RocketGraphicsInterface/IMeshRenderer.h"

namespace Rocket
{
	MeshRenderer::MeshRenderer()
		: _meshRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateMeshRenderer())
	{
		_renderable = _meshRenderer;
	}

	void MeshRenderer::SetMesh(eMeshType meshType)
	{
		_meshRenderer->SetMesh(meshType);
	}

}

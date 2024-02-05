#include "MeshRenderer.h"
#include "GraphicsSystem.h"
#include "../RocketGraphicsInterface/IMeshRenderer.h"

namespace Rocket
{
	MeshRenderer::MeshRenderer()
		: _meshRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateMeshRenderer())
	{

	}

	void MeshRenderer::SetMesh(eMeshType meshType)
	{
		_meshRenderer->SetMesh(meshType);
	}

	void MeshRenderer::UpdateRenderData()
	{
		_meshRenderer->SetWorldTM(gameObject->transform.GetWorldTM());
		_meshRenderer->SetActive(gameObject->IsActive());
	}

}

#include "MeshRenderer.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../RocketCommon/IStaticModelRenderer.h"

namespace Rocket
{
	MeshRenderer::MeshRenderer()
		: _meshRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateStaticModelRenderer())
	{

	}

// 	void StaticModelRenderer::SetMesh(eMeshType meshType)
// 	{
// 		_meshRenderer->LoadModel(meshType);
// 	}

	void MeshRenderer::SetMesh(std::string fileName)
	{
		_meshRenderer->LoadModel(fileName);
	}

	void MeshRenderer::SetTexture(std::string fileName)
	{
		_meshRenderer->LoadTexture(fileName);
	}

	void MeshRenderer::UpdateRenderData()
	{
		_meshRenderer->SetWorldTM(gameObject->transform.GetWorldTM());
		_meshRenderer->SetActive(gameObject->IsActive());
	}

}

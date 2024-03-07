#include "StaticModelRenderer.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../GraphicsInterface/IStaticModelRenderer.h"

namespace Rocket
{
	StaticModelRenderer::StaticModelRenderer()
		: _meshRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateStaticModelRenderer())
	{

	}

	void StaticModelRenderer::SetMesh(eMeshType meshType)
	{
		_meshRenderer->LoadMesh(meshType);
	}

	void StaticModelRenderer::SetMesh(std::string fileName)
	{
		_meshRenderer->LoadMesh(fileName);
	}

	void StaticModelRenderer::SetTexture(std::string fileName)
	{
		_meshRenderer->LoadTexture(fileName);
	}

	void StaticModelRenderer::UpdateRenderData()
	{
		_meshRenderer->SetWorldTM(gameObject->transform.GetWorldTM());
		_meshRenderer->SetActive(gameObject->IsActive());
	}

}

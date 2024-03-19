#include "DynamicModelRenderer.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../RocketCommon/IDynamicModelRenderer.h"

namespace Rocket
{
	DynamicModelRenderer::DynamicModelRenderer()
		: _skinnedMeshRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateDynamicModelRenderer())
	{

	}

	void DynamicModelRenderer::LoadModel(std::string fileName)
	{
		_skinnedMeshRenderer->LoadModel(fileName);
	}

	void DynamicModelRenderer::SetTexture(std::string fileName)
	{
		_skinnedMeshRenderer->LoadTexture(fileName);
	}

	void DynamicModelRenderer::UpdateRenderData()
	{
		_skinnedMeshRenderer->SetWorldTM(gameObject->transform.GetWorldTM());

	}

}

#include "DynamicModelRenderer.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../RocketCommon/IDynamicModelRenderer.h"

namespace Rocket
{
	DynamicModelRenderer::DynamicModelRenderer()
		: _graphicsComponent(Core::GraphicsSystem::Instance().GetFactory()->CreateDynamicModelRenderer())
	{

	}

	void DynamicModelRenderer::LoadModel(std::string fileName)
	{
		_graphicsComponent->LoadModel(fileName);
	}

	void DynamicModelRenderer::SetTexture(std::string fileName)
	{
		_graphicsComponent->LoadTexture(fileName);
	}

	void DynamicModelRenderer::UpdateRenderData()
	{
		_graphicsComponent->SetWorldTM(gameObject->transform.GetWorldTM());

	}

	void DynamicModelRenderer::BindTransform(Transform* transform)
	{
		_graphicsComponent->BindTransform(transform->_rocketTransform);
	}

}

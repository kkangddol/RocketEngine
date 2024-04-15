﻿#include "MeshRenderer.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../RocketCommon/IMeshRenderer.h"

namespace Rocket
{
	MeshRenderer::MeshRenderer()
		: _graphicsRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateStaticModelRenderer())
	{

	}

	void MeshRenderer::SetMesh(eMeshType meshType)
	{
		_graphicsRenderer->SetMesh(meshType);
	}

	void MeshRenderer::LoadMesh(std::string fileName)
	{
		_graphicsRenderer->LoadMesh(fileName);
		_graphicsRenderer->BindTransform(gameObject->transform._rocketTransform);
	}

	void MeshRenderer::SetTexture(std::string fileName)
	{
		_graphicsRenderer->LoadBaseColorTexture(fileName);
	}

	void MeshRenderer::SetMetallic(float value)
	{
		_graphicsRenderer->SetMetallic(value);
	}

	void MeshRenderer::SetRoughness(float value)
	{
		_graphicsRenderer->SetRoughness(value);
	}

	void MeshRenderer::BindTransform()
	{
		_graphicsRenderer->BindTransform(gameObject->transform._rocketTransform);
	}

	void MeshRenderer::UpdateRenderData()
	{
		_graphicsRenderer->SetWorldTM(gameObject->transform.GetWorldTM());
		_graphicsRenderer->SetActive(gameObject->IsActive());
	}

}

#include "SkinnedMeshRenderer.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "../GraphicsInterface/ISkinnedMeshRenderer.h"

namespace Rocket
{
	SkinnedMeshRenderer::SkinnedMeshRenderer()
		: _skinnedMeshRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateSkinnedMeshRenderer())
	{

	}

	void SkinnedMeshRenderer::SetMesh(std::string fileName)
	{
		_skinnedMeshRenderer->LoadMesh(fileName);
	}

	void SkinnedMeshRenderer::SetTexture(std::string fileName)
	{
		_skinnedMeshRenderer->LoadTexture(fileName);
	}

	void SkinnedMeshRenderer::UpdateRenderData()
	{
		_skinnedMeshRenderer->SetWorldTM(gameObject->transform.GetWorldTM());

	}

}

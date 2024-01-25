#include "MeshRenderer.h"
#include "GraphicsSystem.h"

namespace Rocket
{
	MeshRenderer::MeshRenderer()
		: _meshRenderer(Core::GraphicsSystem::Instance().GetFactory()->CreateMeshRenderer())
	{
		_renderable = _meshRenderer;
	}
}

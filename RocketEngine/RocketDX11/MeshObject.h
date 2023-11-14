#pragma once
#include "..\\RocketGraphicsInterface\\IRenderable.h"

namespace RocketCore::Graphics
{
	class MeshObject : public IRenderable
	{
		virtual void UpdateRenderData() override;
	};
}

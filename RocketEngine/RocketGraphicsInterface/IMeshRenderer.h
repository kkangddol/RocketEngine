#pragma once
#include <string>
#include "IRenderable.h"
#include "GraphicsEnum.h"

namespace Rocket::Core
{
	class IMeshRenderer : public IRenderable
	{
	public:
		virtual ~IMeshRenderer() {}
		virtual void SetMesh(eMeshType meshType) = 0;

	};
}

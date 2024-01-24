#pragma once
#include "MathHeader.h"
#include "MaterialData.h"

namespace RocketCore::Graphics
{
	class IRenderable abstract
	{
	public:
		virtual void UpdateRenderData() abstract;
	};
}

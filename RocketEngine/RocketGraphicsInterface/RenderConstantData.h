#pragma once
#include <string>
#include <vector>
#include "MathHeader.h"
#include "LightData.h"

namespace RocketCore::Graphics
{
	struct RenderConstantData
	{
		//실제로 셰이더 넣을 때는, 개수 제한 넣어야.
		std::vector<DirectionalLightData> dirLightList;
		std::vector<PointLightData> pointLightList;
		std::vector<SpotLightData> spotLightList;

		float deltaTime;
	};
}

#pragma once
#include <string>
#include <vector>
#include "MathHeader.h"
#include "LightData.h"

namespace RocketCore::Graphics
{
	struct RenderConstantData
	{
		//������ ���̴� ���� ����, ���� ���� �־��.
		std::vector<DirectionalLightData> dirLightList;
		std::vector<PointLightData> pointLightList;
		std::vector<SpotLightData> spotLightList;

		float deltaTime;
	};
}

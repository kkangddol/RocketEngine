#pragma once
#include "MathHeader.h"

namespace RocketCore::Graphics
{
	struct DirectionalLightData
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector3 direction;
		float intensity = 1.0f; 
		//자동으로 바이트 패딩 됨. Directional Light 한정 빛의 밝기 정렬을 위해 Intensity 마련.
	};

	struct PointLightData
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;

		//GPU용 4D Vector 패킹.
		Vector3 position;
		float range;

		//GPU용 4D Vector 패킹.
		Vector3 attenuation;
		float tPad;
	};

	struct SpotLightData
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;

		//GPU용 4D Vector 패킹.
		Vector3 position;
		float range;

		//GPU용 4D Vector 패킹.
		Vector3 direction;
		float spot;

		//GPU용 4D Vector 패킹.
		Vector3 attenuation;
		float tPad;
	};
}

///[TW] : TBA - 내가 본 Flax 등 Lighting 예시 : 전부 PBR 베이스였다!
///그러니, 일단 Legacy가 제대로 돌아가는 것을 보고 실행해라.


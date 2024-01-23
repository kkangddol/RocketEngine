#pragma once
#include "DLLExporter.h"
#include "Light.h"

#include "..\\RocketGraphicsInterface\\LightData.h"

namespace RocketEngine
{
	class GameObject;
}

namespace RocketEngine
{
	class ROCKET_API PointLight final : public Light
	{
	public:
		PointLight(GameObject* owner);

	public:
		RocketCore::Graphics::PointLightData GetLightData() const;

	public:
		float GetRange() const;
		void SetRange(float range);
		Vector3 GetAttenuation() const;
		void SetAttenuation(Vector3 attenuation);

	private:
		float _range;
		Vector3 _attenuation;
	};
}

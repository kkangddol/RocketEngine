#pragma once
#include "DLLExporter.h"
#include "Light.h"

namespace Rocket
{
	class GameObject;
}

namespace Rocket
{
	class ROCKET_API DirectionalLight final : public Light
	{
	public:
		DirectionalLight(GameObject* owner);

	public:
		Rocket::Core::DirectionalLightData GetLightData() const;

	public:
		float GetIntensity() const;
		void SetIntensity(float intensity);

	private:
		float _intensity;
	};
}

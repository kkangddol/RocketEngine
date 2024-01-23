#include "DLLExporter.h"
#include "Light.h"
#include "RenderSystem.h"

namespace RocketEngine
{
	Light::Light(GameObject* owner)
		: Component(owner),
		_ambient(1.0f,1.0f,1.0f,1.0f),
		_diffuse(1.0f,1.0f,1.0f,1.0f),
		_specular(1.0f,1.0f,1.0f,1.0f)
	{
		//RocketCore::RenderSystem::Instance().AddLight(this);
	}

	RocketEngine::Vector4 Light::GetAmbient() const
	{
		return _ambient;
	}

	void Light::SetAmbient(Vector4 ambient)
	{
		_ambient = ambient;
	}

	RocketEngine::Vector4 Light::GetDiffuse() const
	{
		return _diffuse;
	}

	void Light::SetDiffuse(Vector4 diffuse)
	{
		_diffuse = diffuse;
	}

	RocketEngine::Vector4 Light::GetSpecular() const
	{
		return _specular;
	}

	void Light::SetSpecular(Vector4 specular)
	{
		_specular = specular;
	}

}

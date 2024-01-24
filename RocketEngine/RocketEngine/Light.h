#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include "MathHeader.h"

namespace RocketEngine
{
	class GameObject;
}

namespace RocketEngine
{
	class ROCKET_API Light : public Component
	{
	public:
		Light(GameObject* owner);

	public:
		Vector4 GetAmbient() const;
		void SetAmbient(Vector4 ambient);
		Vector4 GetDiffuse() const;
		void SetDiffuse(Vector4 diffuse);
		Vector4 GetSpecular() const;
		void SetSpecular(Vector4 specular);

	protected:
		Vector4 _ambient;
		Vector4 _diffuse;
		Vector4 _specular;
	};
}

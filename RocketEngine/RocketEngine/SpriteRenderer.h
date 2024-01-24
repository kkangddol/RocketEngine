#pragma once
#include "DLLExporter.h"
#include "UIRenderer.h"
#include <string>

namespace Rocket
{
	class GameObject;

	class ROCKET_API SpriteRenderer : public Rocket::Core::UIRenderer
	{
	public:
		SpriteRenderer(GameObject* owner);

	public:
		virtual std::string GetSketchData() override;

	public:
		std::string GetPath() const;
		void SetPath(std::string path);

	private:
		std::string _spritePath;
	};
}

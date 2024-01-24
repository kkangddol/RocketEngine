#pragma once
#include "..\\HODO3DGraphicsInterface\\IFactory.h"

namespace Rocket::Core
{
	class ObjectFactory : public Rocket::Core::IFactory
	{
	public:
		virtual Rocket::Core::ICamera* CreateCamera() override;
		virtual Rocket::Core::IStaticMesh* CreateStaticMeshObject() override;
		virtual Rocket::Core::ISketchableText* CreateText() override;
		virtual Rocket::Core::ISketchableImage* CreateImage() override;
		virtual Rocket::Core::ILineRenderer* CreateLineRenderer() override;
	};
}


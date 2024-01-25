#pragma once
#include "..\\RocketGraphicsInterface\\IFactory.h"

namespace Rocket::Core
{
	class ObjectFactory : public Rocket::Core::IFactory
	{
	public:
		virtual ICamera* CreateCamera() override;
		virtual IStaticMesh* CreateStaticMeshObject() override;
		virtual ISketchableText* CreateText() override;
		virtual ISketchableImage* CreateImage() override;
		virtual ILineRenderer* CreateLineRenderer() override;

		// IFactory을(를) 통해 상속됨
		virtual ISkinnedMesh* CreateSkinnedMeshObject() override;
		virtual CubePrimitive* CreateCubePrimitive() override;
		virtual SpherePrimitive* CreateSpherePrimitive() override;
		virtual CylinderPrimitive* CreateCylinderPrimitive() override;
	};
}


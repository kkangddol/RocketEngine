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

		// IFactory��(��) ���� ��ӵ�
		virtual ISkinnedMesh* CreateSkinnedMeshObject() override;
		virtual CubePrimitive* CreateCubePrimitive() override;
		virtual SpherePrimitive* CreateSpherePrimitive() override;
		virtual CylinderPrimitive* CreateCylinderPrimitive() override;
	};
}


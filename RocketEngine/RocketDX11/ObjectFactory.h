#pragma once
#include "..\\RocketGraphicsInterface\\IFactory.h"

namespace Rocket::Core
{
	class ObjectManager;

	class ObjectFactory : public Rocket::Core::IFactory
	{
	public:
		ObjectFactory();

	public:
		virtual ICamera* CreateCamera() override;
		virtual IMeshRenderer* CreateMeshRenderer() override;
		virtual ISketchableText* CreateText() override;
		virtual ISketchableImage* CreateImage() override;
		virtual ILineRenderer* CreateLineRenderer() override;

		// IFactory��(��) ���� ��ӵ�
		virtual ISkinnedMesh* CreateSkinnedMeshObject() override;
		virtual CubePrimitive* CreateCubePrimitive() override;
		virtual SpherePrimitive* CreateSpherePrimitive() override;
		virtual CylinderPrimitive* CreateCylinderPrimitive() override;

	private:
		ObjectManager& _objectManager;
	};
}


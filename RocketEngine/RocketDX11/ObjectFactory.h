﻿#pragma once
#include "..\\RocketCommon\\IFactory.h"

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
		virtual ITextRenderer* CreateTextRenderer() override;
		virtual ISpriteRenderer* CreateSpriteRenderer() override;
		virtual ILineRenderer* CreateLineRenderer() override;
		virtual IDirectionalLight* CreateDirectionalLight() override;
		virtual IDynamicModelRenderer* CreateDynamicModelRenderer() override;
		virtual IStaticModelRenderer* CreateStaticModelRenderer() override;
		virtual CubePrimitive* CreateCubePrimitive() override;
		virtual SpherePrimitive* CreateSpherePrimitive() override;
		virtual CylinderPrimitive* CreateCylinderPrimitive() override;

	private:
		ObjectManager& _objectManager;
	};
}


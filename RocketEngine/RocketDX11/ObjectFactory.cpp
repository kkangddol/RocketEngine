#include "ObjectFactory.h"
#include "ObjectManager.h"
#include "ResourceManager.h"

#include "Camera.h"
#include "StaticMeshObject.h"
#include "CubeMesh.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "LineRenderer.h"

namespace Rocket::Core
{
	IFactory* CreateGraphicsObjectFactory()
	{
		return new Rocket::Core::ObjectFactory();
	}

	void ReleaseFactory(IFactory* instance)
	{
		delete instance;
	}
}

namespace Rocket::Core
{
	Rocket::Core::ICamera* ObjectFactory::CreateCamera()
	{
		return ObjectManager::Instance().CreateCamera();
	}

	Rocket::Core::IStaticMesh* ObjectFactory::CreateStaticMeshObject()
	{
		ObjectManager& objMgr = ObjectManager::Instance();
		ResourceManager& rscMgr = ResourceManager::Instance();

		StaticMeshObject* obj = objMgr.CreateStaticMeshObject();

		obj->SetModel(rscMgr.GetCubeModel());
		obj->SetVertexShader(rscMgr.GetVertexShader("LightVS"));
		obj->SetPixelShader(rscMgr.GetPixelShader("LightPS"));
		obj->SetRenderState(rscMgr.GetRenderState(ResourceManager::eRenderState::SOLID));

		return obj;
	}

	Rocket::Core::ISketchableText* ObjectFactory::CreateText()
	{
		return ObjectManager::Instance().CreateText();
	}

	Rocket::Core::ISketchableImage* ObjectFactory::CreateImage()
	{
		return ObjectManager::Instance().CreateImage();
	}

	Rocket::Core::ILineRenderer* ObjectFactory::CreateLineRenderer()
	{
		return ObjectManager::Instance().CreateLineRenderer();
	}

	ISkinnedMesh* Core::ObjectFactory::CreateSkinnedMeshObject()
	{
		return nullptr;
	}

	CubePrimitive* Core::ObjectFactory::CreateCubePrimitive()
	{
		return nullptr;
	}

	SpherePrimitive* Core::ObjectFactory::CreateSpherePrimitive()
	{
		return nullptr;
	}

	CylinderPrimitive* Core::ObjectFactory::CreateCylinderPrimitive()
	{
		return nullptr;
	}

}

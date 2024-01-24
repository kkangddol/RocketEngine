#include "ObjectManager.h"

#include "..\\HODO3DGraphicsInterface\\IRenderable.h"

#include "Camera.h"
#include "StaticMeshObject.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "ResourceManager.h"
#include "LineRenderer.h"

namespace Rocket::Core
{
	ObjectManager::ObjectManager()
	{

	}

	Camera* ObjectManager::CreateCamera()
	{
		Camera* temp = new Camera();
		_cameraList.emplace_back(temp);

		return temp;
	}

	StaticMeshObject* ObjectManager::CreateStaticMeshObject()
	{
		StaticMeshObject* temp = new StaticMeshObject();
		_staticMeshObjectList.emplace_back(temp);

		return temp;
	}

	Rocket::Core::ImageRenderer* ObjectManager::CreateImage()
	{
		auto& resourceMgr = ResourceManager::Instance();

		ImageRenderer* temp = new ImageRenderer();
		temp->InitalizeImageRenderer(resourceMgr.GetDevice(), resourceMgr.GetDeviceContext());
		temp->SetImage("abcd.jpg");
		_ImageList.emplace_back(temp);

		return temp;
	}

	std::vector<ImageRenderer*>& ObjectManager::GetImageList()
	{
		return _ImageList;
	}

	std::vector<StaticMeshObject*>& ObjectManager::GetStaticMeshObjList()
	{
		return _staticMeshObjectList;
	}

	Rocket::Core::TextRenderer* ObjectManager::CreateText()
	{
		TextRenderer* TextObject = new TextRenderer();
		_textList.emplace_back(TextObject);
		return TextObject;
	}

	std::vector<TextRenderer*>& ObjectManager::GetTextList()
	{
		return _textList;
	}

	LineRenderer* ObjectManager::CreateLineRenderer()
	{
		_lineRenderer = new LineRenderer();
		return _lineRenderer;
	}

	LineRenderer* ObjectManager::GetLineRenderer()
	{
		return _lineRenderer;
	}

}

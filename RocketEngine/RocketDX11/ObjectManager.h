#pragma once
#include <vector>

#include "Singleton.h"


namespace Rocket::Core
{
	class ResourceManager;

	class Camera;
	class MeshRenderer;
	class TextRenderer;
	class ImageRenderer;
	class LineRenderer;

	class ObjectManager : public Singleton<ObjectManager>
	{
		friend Singleton;
	private:
		ObjectManager();

	public:
		std::vector<MeshRenderer*>& GetStaticMeshObjList();
		std::vector<TextRenderer*>& GetTextList();
		std::vector<ImageRenderer*>& GetImageList();
		LineRenderer* GetLineRenderer();

	public:
		Camera* CreateCamera();
		MeshRenderer* CreateMeshRenderer();
		TextRenderer* CreateText();
		ImageRenderer* CreateImage();
		LineRenderer* CreateLineRenderer();

	private:
		std::vector<Camera*> _cameraList;
		std::vector<MeshRenderer*> _meshRendererList;
		std::vector<TextRenderer*> _textList;
		std::vector<ImageRenderer*> _ImageList;
		LineRenderer* _lineRenderer;

	private:
		ResourceManager& _resourceManager;
	};
}

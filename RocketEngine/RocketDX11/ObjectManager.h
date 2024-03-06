#pragma once
#include <vector>

#include "Singleton.h"


namespace Rocket::Core
{
	class ResourceManager;

	class Camera;
	class MeshRenderer;
	class SkinnedMeshRenderer;
	class TextRenderer;
	class SpriteRenderer;
	class LineRenderer;

	class ObjectManager : public Singleton<ObjectManager>
	{
		friend Singleton;
	private:
		ObjectManager();

	public:
		std::vector<MeshRenderer*>& GetStaticMeshRenderers();
		std::vector<SkinnedMeshRenderer*>& GetSkinnedMeshRenderers();
		std::vector<TextRenderer*>& GetTextList();
		std::vector<SpriteRenderer*>& GetImageList();
		LineRenderer* GetLineRenderer();

	public:
		Camera* CreateCamera();
		MeshRenderer* CreateMeshRenderer();
		SkinnedMeshRenderer* CreateSkinnedMeshRenderer();
		TextRenderer* CreateText();
		SpriteRenderer* CreateImage();
		LineRenderer* CreateLineRenderer();

	private:
		std::vector<Camera*> _cameraList;
		std::vector<MeshRenderer*> _meshRendererList;
		std::vector<SkinnedMeshRenderer*> _skinnedMeshRendererList;
		std::vector<TextRenderer*> _textList;
		std::vector<SpriteRenderer*> _ImageList;
		LineRenderer* _lineRenderer;

	private:
		ResourceManager& _resourceManager;
	};
}

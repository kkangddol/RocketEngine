#pragma once
#include <vector>
#include <memory>
#include <d3d11_2.h>
#include <dxgi1_3.h>

#include "Singleton.h"
#include "TextRenderer.h"
#include "Axis.h"
#include "Grid.h"

namespace Rocket::Core
{
	class ResourceManager;

	class Camera;
	class MeshRenderer;
	class DynamicModelRenderer;
	class SpriteRenderer;
	class LineRenderer;
	class DirectionalLight;
//	class TextRenderer;
// 	class Axis;
// 	class Grid;

	class ObjectManager : public Singleton<ObjectManager>
	{
		friend Singleton;
	private:
		ObjectManager();

	public:
		void Initialize(ID3D11Device* device);
		void Finalize();

	public:
		std::vector<MeshRenderer*>& GetStaticModelRenderers();
		std::vector<DynamicModelRenderer*>& GetDynamicModelRenderers();
		std::vector<TextRenderer*>& GetTextList();
		std::vector<SpriteRenderer*>& GetImageList();
		LineRenderer* GetLineRenderer();
		std::vector<DirectionalLight*>& GetDirectionalLightList();

	public:
		Camera* CreateCamera();
		MeshRenderer* CreateStaticModelRenderer();
		DynamicModelRenderer* CreateDynamicModelRenderer();
		TextRenderer* CreateText();
		SpriteRenderer* CreateImage();
		LineRenderer* CreateLineRenderer();
		DirectionalLight* CreateDirectionalLight();

		/// Debug 객체들.
	public:
		std::unique_ptr<TextRenderer> _fpsText;
		std::unique_ptr<Axis> _axis;
		std::unique_ptr<Grid> _grid;

	private:
		std::vector<Camera*> _cameraList;
		std::vector<MeshRenderer*> _staticModelRendererList;
		std::vector<DynamicModelRenderer*> _dynamicModelRendererList;
		std::vector<TextRenderer*> _textList;
		std::vector<SpriteRenderer*> _ImageList;
		LineRenderer* _lineRenderer;
		std::vector<DirectionalLight*> _directionalLightList;

	private:
		ResourceManager& _resourceManager;
	};
}

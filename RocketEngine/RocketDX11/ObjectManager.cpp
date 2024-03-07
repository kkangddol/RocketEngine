#include "ObjectManager.h"

#include "..\\GraphicsInterface\\IRenderable.h"

#include "ResourceManager.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "LineRenderer.h"
#include "Mesh.h"
#include "CubeMesh.h"

namespace Rocket::Core
{
	ObjectManager::ObjectManager()
		: _resourceManager(ResourceManager::Instance()),
		_lineRenderer(nullptr),
		_meshRendererList(),
		_cameraList(),
		_textList()
	{

	}

	Camera* ObjectManager::CreateCamera()
	{
		Camera* temp = new Camera();
		_cameraList.emplace_back(temp);

		return temp;
	}

	MeshRenderer* ObjectManager::CreateMeshRenderer()
	{
		MeshRenderer* meshRenderer = new MeshRenderer();

		meshRenderer->LoadMesh(_resourceManager.GetCubeMesh());

		//skinnedMeshRenderer->LoadMesh(_resourceManager.GetCubeMesh());
		// TODO : 기본 Material을 넣어주고 앞단에서 Material을 바꿔서 넣어줄 수 있도록 하자
		//meshRenderer->SetMaterial(_resourceManager.GetDefaultMaterial());
		Material* material = new Material();
		material->SetTexture(_resourceManager.GetDefaultTexture());
		material->SetVertexShader(_resourceManager.GetVertexShader("StaticMeshVS"));
		material->SetPixelShader(_resourceManager.GetPixelShader("StaticMeshPS"));
		material->SetRenderState(_resourceManager.GetRenderState(ResourceManager::eRenderState::SOLID));
		meshRenderer->SetMaterial(material);
		
		_meshRendererList.emplace_back(meshRenderer);

		return meshRenderer;
	}

	Rocket::Core::SpriteRenderer* ObjectManager::CreateImage()
	{
		SpriteRenderer* temp = new SpriteRenderer();
		temp->SetImage("test.jpg");
		_ImageList.emplace_back(temp);

		return temp;
	}

	std::vector<SpriteRenderer*>& ObjectManager::GetImageList()
	{
		return _ImageList;
	}

	std::vector<MeshRenderer*>& ObjectManager::GetStaticMeshRenderers()
	{
		return _meshRendererList;
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

	std::vector<SkinnedMeshRenderer*>& ObjectManager::GetSkinnedMeshRenderers()
	{
		return _skinnedMeshRendererList;
	}

	SkinnedMeshRenderer* ObjectManager::CreateSkinnedMeshRenderer()
	{
		SkinnedMeshRenderer* skinnedMeshRenderer = new SkinnedMeshRenderer();

		//skinnedMeshRenderer->LoadMesh(_resourceManager.GetCubeMesh());
		// TODO : 기본 Material을 넣어주고 앞단에서 Material을 바꿔서 넣어줄 수 있도록 하자
		//skinnedMeshRenderer->SetMaterial(_resourceManager.GetDefaultMaterial());
		Material* material = new Material();
		material->SetTexture(_resourceManager.GetDefaultTexture());
		material->SetVertexShader(_resourceManager.GetVertexShader("SkinnedMeshVS"));
		material->SetPixelShader(_resourceManager.GetPixelShader("SkinnedMeshPS"));
		material->SetRenderState(_resourceManager.GetRenderState(ResourceManager::eRenderState::SOLID));
		skinnedMeshRenderer->SetMaterial(material);

		_skinnedMeshRendererList.emplace_back(skinnedMeshRenderer);

		return skinnedMeshRenderer;
	}

}

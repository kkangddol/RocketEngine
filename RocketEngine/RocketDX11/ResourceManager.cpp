﻿#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <cassert>

#include "ResourceManager.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "TextRenderer.h"
#include "SpriteRenderer.h"
#include "GraphicsMacro.h"
#include "texture.h"
#include "material.h"
#include "FBXLoader.h"
#include "VertexStruct.h"
#include "ModelStruct.h"
#include "CubeMap.h"
#include "../RocketCommon/RawModelStruct.h"

const std::string TEXTURE_PATH = "Resources/Textures/";
const std::string MODEL_PATH = "Resources/Models/";

namespace Rocket::Core
{
	ResourceManager::ResourceManager()
		: _device(),
		_deviceContext(),
		_defaultFont(),
		_cubeMesh(),
		_sphereMesh(),
		_defaultTexture(),
		_defaultMaterial(),
		_fbxLoader(new FBXLoader())
	{
		
	}

	void ResourceManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		_device = device;
		_deviceContext = deviceContext;

		// TODO : 너 Common으로 갔으니까 바뀌어야 할 필요가 있단다.
		_fbxLoader->Initialize(device);

		// Color Shader
		{
			VertexShader* colorVS = new VertexShader();
			colorVS->Initialize(_device.Get(), L"RocketDX11/ColorVS.hlsl");
			colorVS->SetVertexType(eVertexType::COLOR_VERTEX);
			_vertexShaders["ColorVS"] = colorVS;

			PixelShader* colorPS = new PixelShader();
			colorPS->Initialize(_device.Get(), L"RocketDX11/ColorPS.hlsl");
			_pixelShaders["ColorPS"] = colorPS;
		}

		// Texture Shader
		{
			VertexShader* textureVS = new VertexShader();
			textureVS->Initialize(_device.Get(), L"RocketDX11/TextureVS.hlsl");
			textureVS->SetVertexType(eVertexType::TEXTURE_VERTEX);
			_vertexShaders["TextureVS"] = textureVS;

			PixelShader* texturePS = new PixelShader();
			texturePS->Initialize(_device.Get(), L"RocketDX11/TexturePS.hlsl");
			_pixelShaders["TexturePS"] = texturePS;
		}

		// Light Shader
		{
			VertexShader* lightVS = new VertexShader();
			lightVS->Initialize(_device.Get(), L"RocketDX11/LightVS.hlsl");
			lightVS->SetVertexType(eVertexType::LIGHT_VERTEX);
			_vertexShaders["LightVS"] = lightVS;

			PixelShader* lightPS = new PixelShader();
			lightPS->Initialize(_device.Get(), L"RocketDX11/LightPS.hlsl");
			_pixelShaders["LightPS"] = lightPS;
		}

		// StaticMesh Shader
		{
			VertexShader* staticMeshVS = new VertexShader();
			staticMeshVS->Initialize(_device.Get(), L"RocketDX11/StaticMeshVS.hlsl");
			staticMeshVS->SetVertexType(eVertexType::VERTEX);
			_vertexShaders["StaticMeshVS"] = staticMeshVS;

			PixelShader* staticMeshPS = new PixelShader();
			staticMeshPS->Initialize(_device.Get(), L"RocketDX11/StaticMeshPS.hlsl");
			_pixelShaders["StaticMeshPS"] = staticMeshPS;
		}

		// SkinnedMesh Shader
		{
			VertexShader* skinnedMeshVS = new VertexShader();
			skinnedMeshVS->Initialize(_device.Get(), L"RocketDX11/SkinnedMeshVS.hlsl");
			skinnedMeshVS->SetVertexType(eVertexType::SKINNED_VERTEX);
			_vertexShaders["SkinnedMeshVS"] = skinnedMeshVS;

			PixelShader* skinnedMeshPS = new PixelShader();
			skinnedMeshPS->Initialize(_device.Get(), L"RocketDX11/SkinnedMeshPS.hlsl");
			_pixelShaders["SkinnedMeshPS"] = skinnedMeshPS;
		}

		// CubeMap Shader
		{
			VertexShader* cubeMapVS = new VertexShader();
			cubeMapVS->Initialize(_device.Get(), L"RocketDX11/CubeMapVS.hlsl");
			cubeMapVS->SetVertexType(eVertexType::VERTEX);
			_vertexShaders["CubeMapVS"] = cubeMapVS;

			PixelShader* cubeMapPS = new PixelShader();
			cubeMapPS->Initialize(_device.Get(), L"RocketDX11/CubeMapPS.hlsl");
			_pixelShaders["CubeMapPS"] = cubeMapPS;
		}

		CreateRenderStates();

		_cubeMesh = new CubeMesh();
		_cubeMesh->Initialize(device);

		_sphereMesh = new SphereMesh();
		_sphereMesh->Initialize(device);

		_cubeMap = new CubeMap();
		_cubeMap->Initialize(device);
		_cubeMap->LoadTexture("tori1024.dds");

		_defaultTexture = LoadTextureFile("darkbrickdxt1.dds");

		_defaultFont = new DirectX::SpriteFont(_device.Get(), L"Resources/Font/NotoSansKR.spritefont");
		
		_defaultMaterial = new Material();
		_defaultMaterial->SetVertexShader(GetVertexShader("StaticMeshVS"));
		_defaultMaterial->SetPixelShader(GetPixelShader("StaticMeshPS"));
		_defaultMaterial->SetRenderState(GetRenderState(eRenderState::SOLID));
		_defaultMaterial->SetTexture(_defaultTexture);

		_cubePrimitive = DirectX::DX11::GeometricPrimitive::CreateCube(deviceContext, 1.0f, false);
		_spherePrimitive = DirectX::DX11::GeometricPrimitive::CreateSphere(deviceContext, 1.0f, 8, false, false);
		_cylinderPrimitive = DirectX::DX11::GeometricPrimitive::CreateCylinder(deviceContext, 2.0f, 1.0f, 8, false);
	}

	void ResourceManager::LoadModel(const std::string& fileName, const RawModel* rawModel)
	{
		// TODO : FBXLoader에서 뭔가 생성하던 것들을 여기서 해주면 된다.
		if (rawModel->animations.empty())
		{
			_models.insert({ fileName,new StaticModel });
			ProcessStaticModel(fileName, rawModel);
		}
		else
		{
			_models.insert({ fileName, new DynamicModel });
			ProcessDynamicModel(fileName, rawModel);
		}
	}

	VertexShader* ResourceManager::GetVertexShader(const std::string& name)
	{
		if (_vertexShaders.find(name) == _vertexShaders.end())
		{
			return nullptr;
		}

		return _vertexShaders[name];
	}

	PixelShader* ResourceManager::GetPixelShader(const std::string& name)
	{
		if (_pixelShaders.find(name) == _pixelShaders.end())
		{
			return nullptr;
		}

		return _pixelShaders[name];
	}

	DirectX::SpriteFont* ResourceManager::GetDefaultFont()
	{
		return _defaultFont;
	}

	ID3D11Device* ResourceManager::GetDevice()
	{
		return _device.Get();
	}

	ID3D11DeviceContext* ResourceManager::GetDeviceContext()
	{
		return _deviceContext.Get();
	}

	void ResourceManager::CreateRenderStates()
	{
		// Render State 중 Rasterizer State
		D3D11_RASTERIZER_DESC solidDesc;
		ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
		solidDesc.FillMode = D3D11_FILL_SOLID;
		solidDesc.CullMode = D3D11_CULL_BACK;
		solidDesc.FrontCounterClockwise = false;
		solidDesc.DepthClipEnable = true;
		ID3D11RasterizerState* solid;
		HR(_device->CreateRasterizerState(&solidDesc, &solid));
		_renderStates.emplace_back(solid);

		D3D11_RASTERIZER_DESC wireframeDesc;
		ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc.CullMode = D3D11_CULL_BACK;
		wireframeDesc.FrontCounterClockwise = false;
		wireframeDesc.DepthClipEnable = true;
		ID3D11RasterizerState* wireframe;
		HR(_device->CreateRasterizerState(&wireframeDesc, &wireframe));
		_renderStates.emplace_back(wireframe);
	}

	ID3D11RasterizerState* ResourceManager::GetRenderState(eRenderState eState)
	{
		return _renderStates[static_cast<int>(eState)];
	}

	Mesh* ResourceManager::GetMesh(eMeshType meshType) const
	{
		switch (meshType)
		{
		case eMeshType::CUBE:
			return _cubeMesh;
		case eMeshType::SPHERE:
			return _sphereMesh;
		default:
			return nullptr;
		}
	}

	Texture* ResourceManager::GetTexture(std::string fileName)
	{
		if (_textures.find(fileName) == _textures.end())
		{
			return LoadTextureFile(fileName);
		}

		return _textures.at(fileName);
	}

	Texture* ResourceManager::LoadTextureFile(std::string fileName)
	{
		std::string fullPath = TEXTURE_PATH + fileName;
		std::wstring wFileName(fullPath.begin(), fullPath.end());

		std::string extension = fileName.substr(fileName.find_last_of(".") + 1);

		ID3D11Resource* rawTexture = nullptr;
		ID3D11ShaderResourceView* textureView = nullptr;

		if (extension == "dds")
		{
			HR(DirectX::CreateDDSTextureFromFile(_device.Get(), wFileName.c_str(), &rawTexture, &textureView));
		}
		else if (extension == "jpg" || extension == "png")
		{
			HR(DirectX::CreateWICTextureFromFile(_device.Get(), wFileName.c_str(), &rawTexture, &textureView));
		}
		else
		{
			assert(false);
		}

		Texture* texture = new Texture(rawTexture, textureView);

		_textures.insert({ fileName,texture });

		return texture;
	}

// 	std::vector<Mesh*>& ResourceManager::GetMeshes(const std::string& fileName)
// 	{
// 		if (_models.find(fileName) == _models.end())
// 		{
// 			_fbxLoader->LoadFBXFile(fileName);
// 		}
// 
// 		return _models[fileName]->meshes;
// 	}

	Rocket::Core::Model* ResourceManager::GetModel(const std::string& fileName)
	{
		if (_models.find(fileName) == _models.end())
		{
			_fbxLoader->LoadFBXFile(fileName);
		}

		return _models[fileName];
	}

	void ResourceManager::ProcessStaticModel(const std::string& fileName, const RawModel* rawModel)
	{
		StaticModel* resultModel = reinterpret_cast<StaticModel*>(_models.at(fileName));


	}

	void ResourceManager::ProcessDynamicModel(const std::string& fileName, const RawModel* rawModel)
	{
		DynamicModel* resultModel = reinterpret_cast<DynamicModel*>(_models.at(fileName));

	}
}

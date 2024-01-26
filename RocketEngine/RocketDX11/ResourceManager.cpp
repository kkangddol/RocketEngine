#include "ResourceManager.h"
#include "Camera.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "Mesh.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "TextRenderer.h"
#include "ImageRenderer.h"
#include "RocketMacroDX11.h"
#include "texture.h"
#include "material.h"


namespace Rocket::Core
{
	ResourceManager::ResourceManager()
		: _device(),
		_deviceContext(),
		_defaultFont(),
		_cubeMesh(),
		_sphereMesh(),
		_defaultTexture(),
		_defaultMaterial()
	{
		
	}

	void ResourceManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		_device = device;
		_deviceContext = deviceContext;
		// Color Shader
		{
			VertexShader* colorVS = new VertexShader();
			D3D11_INPUT_ELEMENT_DESC colorDesc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			colorVS->SetVertexDesc(colorDesc, ARRAYSIZE(colorDesc));
			colorVS->Initialize(_device.Get(), "Resources/Shaders/ColorVS.cso");
			colorVS->SetVertexType(VertexType::COLOR_VERTEX);
			_vertexShaders["ColorVS"] = colorVS;

			PixelShader* colorPS = new PixelShader();
			colorPS->Initialize(_device.Get(), "Resources/Shaders/ColorPS.cso");
			_pixelShaders["ColorPS"] = colorPS;
		}

		// Texture Shader
		{
			VertexShader* textureVS = new VertexShader();
			D3D11_INPUT_ELEMENT_DESC textureDesc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			textureVS->SetVertexDesc(textureDesc, ARRAYSIZE(textureDesc));
			textureVS->Initialize(_device.Get(), "Resources/Shaders/TextureVS.cso");
			textureVS->SetVertexType(VertexType::TEXTURE_VERTEX);
			_vertexShaders["TextureVS"] = textureVS;

			PixelShader* texturePS = new PixelShader();
			texturePS->Initialize(_device.Get(), "Resources/Shaders/TexturePS.cso");
			_pixelShaders["TexturePS"] = texturePS;
		}

		// Light Shader
		{
			VertexShader* lightVS = new VertexShader();
			D3D11_INPUT_ELEMENT_DESC lightDesc[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			lightVS->SetVertexDesc(lightDesc, ARRAYSIZE(lightDesc));
			lightVS->Initialize(_device.Get(), "Resources/Shaders/LightVS.cso");
			lightVS->SetVertexType(VertexType::LIGHT_VERTEX);
			_vertexShaders["LightVS"] = lightVS;

			PixelShader* lightPS = new PixelShader();
			lightPS->Initialize(_device.Get(), "Resources/Shaders/LightPS.cso");
			_pixelShaders["LightPS"] = lightPS;
		}

		CreateRenderStates();



		_cubeMesh = new CubeMesh();
		_cubeMesh->Initialize(device);

		_sphereMesh = new SphereMesh();
		_sphereMesh->Initialize(device);

		_defaultTexture = new Texture();
		_defaultTexture->Initialize(device, L"Resources/Textures/darkbrickdxt1.dds");

		_defaultFont = new DirectX::SpriteFont(_device.Get(), L"Resources/Font/NotoSansKR.spritefont");
		
		_defaultMaterial = new Material();
		_defaultMaterial->SetVertexShader(GetVertexShader("LightVS"));
		_defaultMaterial->SetPixelShader(GetPixelShader("LightPS"));
		_defaultMaterial->SetRenderState(GetRenderState(eRenderState::SOLID));
		_defaultMaterial->SetTexture(_defaultTexture);

		_cubePrimitive = DirectX::DX11::GeometricPrimitive::CreateCube(deviceContext, 1.0f, false);
		_spherePrimitive = DirectX::DX11::GeometricPrimitive::CreateSphere(deviceContext, 1.0f, 8, false, false);
		_cylinderPrimitive = DirectX::DX11::GeometricPrimitive::CreateCylinder(deviceContext, 2.0f, 1.0f, 8, false);
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

}

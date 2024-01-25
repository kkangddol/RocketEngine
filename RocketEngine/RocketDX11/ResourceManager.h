#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <unordered_map>
#include <SpriteFont.h>
#include <vector>

#include "string"
#include "Singleton.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class Mesh;
	class CubeMesh;
	class VertexShader;
	class PixelShader;
	class ImageRenderer;
	class Texture;
	class Material;

	class ResourceManager : public Singleton<ResourceManager>
	{
		friend Singleton;
	private:
		ResourceManager();

	public:
		enum class eRenderState
		{
			SOLID,
			WIREFRAME
		};

	public:
		void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		CubeMesh* GetCubeMesh() const { return _cubeMesh; }
		Texture* GetDefaultTexture() const { return _defaultTexture; }
		Material* GetDefaultMaterial() const { return _defaultMaterial; }
		VertexShader* GetVertexShader(const std::string& name);
		PixelShader* GetPixelShader(const std::string& name);
		DirectX::SpriteFont* GetDefaultFont();

	public:
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		ID3D11RasterizerState* GetRenderState(eRenderState eState);

	private:
		void CreateRenderStates();

	private:
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;

		// 기본 큐브 모델
		CubeMesh* _cubeMesh;

		// 기본 머터리얼
		Material* _defaultMaterial;

		// 기본 텍스쳐
		Texture* _defaultTexture;

		// 기본 폰트 들고있음
		DirectX::SpriteFont* _defaultFont;

		std::unordered_map<std::string, VertexShader*> _vertexShaders;
		std::unordered_map<std::string, PixelShader*> _pixelShaders;
		std::vector<ID3D11RasterizerState*> _renderStates;
	};
}

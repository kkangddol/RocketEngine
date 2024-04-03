#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

const int BUFFER_COUNT = 4;

namespace Rocket::Core
{
	class DeferredBuffers
	{
	public:
		DeferredBuffers();
		~DeferredBuffers();

	public:
		void Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear);
		void SetRenderTargets(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView);
		void ClearRenderTargets(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView, float r, float g, float b, float a);
		ID3D11ShaderResourceView* GetShaderResourceView(int index);

	private:
		int _textureWidth;
		int _textureHeight;
		ComPtr<ID3D11Texture2D> _renderTargetTextureArray[BUFFER_COUNT];
		ComPtr<ID3D11RenderTargetView> _renderTargetViewArray[BUFFER_COUNT];
		ComPtr<ID3D11ShaderResourceView> _shaderResourceViewArray[BUFFER_COUNT];

		ComPtr<ID3D11Texture2D> _depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> _depthStencilView;

		D3D11_VIEWPORT _viewport;
	};
}

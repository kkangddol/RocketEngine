﻿#pragma once
#include <windows.h>
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>

#include "../RocketCommon/IDX11Renderer.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class ObjectManager;
	class ResourceManager;
	class DeferredBuffers;
	class LightPass;
	class ShadowPass;
	class IRenderable;
}

namespace Rocket::Core
{	
	class RocketDX11 final : public IDX11Renderer
	{
	public:
		RocketDX11();
		~RocketDX11();

	public:
		//그래픽스 엔진을 초기화한다.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight) override;
		virtual void SetDebugMode(bool isDebug) override;
		virtual void Update(float deltaTime, int fps = 0) override;
		virtual void Render() override;
		virtual void OnResize(int _width, int _height) override;
		virtual void Finalize() override;

	private:
		void CreateDepthStencilStates();

		void BeginRender(float r = 0.1f, float g = 0.1f, float b = 0.1f, float a = 0.1f);
		void RenderHelperObject();
		void RenderMesh();
		void RenderText();
		void RenderLine();
		void RenderTexture();
		void RenderCubeMap();
		void RenderDebug();
		void GBufferPass();
		void EndRender();

		/// Initialize Member
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;
		bool _vSyncEnabled;
		float _deltaTime;
		bool _isDebugMode;

		/// 초기화 관련
	private:
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;	// immediateContext
		D3D_FEATURE_LEVEL _featureLevel;

		UINT _m4xMsaaQuality;

		ComPtr<IDXGISwapChain> _swapChain;
		ComPtr<ID3D11Texture2D> _backBuffer;
		ComPtr<ID3D11RenderTargetView> _renderTargetView;
		ComPtr<ID3D11Texture2D> _depthStencilBuffer;					// Deferred일때는 이것을 사용하지않고, DeferredBuffer의 뎁스스텐실 버퍼를 사용한다.
		ComPtr<ID3D11DepthStencilState> _defaultDepthStencilState;
		ComPtr<ID3D11DepthStencilState> _cubeMapDepthStencilState;
		ComPtr<ID3D11DepthStencilView> _depthStencilView;				// Deferred일때는 이것을 사용하지않고, DeferredBuffer의 뎁스스텐실 버퍼를 사용한다.
		ComPtr<ID3D11BlendState> _defaultBlendState;
		
		D3D11_VIEWPORT _viewport;

	private:
		DirectX::SpriteBatch* _spriteBatch;
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* _lineBatch;
		std::unique_ptr<DirectX::BasicEffect> _basicEffect;
		ComPtr<ID3D11InputLayout> _lineInputLayout;

	private:
		// 매 프레임 Frustum Culling 적용 후 살아남은 객체들
		std::vector<IRenderable*> _renderList;

		/// deferred 관련
	private:
		std::unique_ptr<DeferredBuffers> _deferredBuffers;
		std::unique_ptr<LightPass> _lightPass;
		std::unique_ptr<ShadowPass> _shadowPass;

	private:
		ObjectManager& _objectManager;
		ResourceManager& _resourceManager;
	};
}

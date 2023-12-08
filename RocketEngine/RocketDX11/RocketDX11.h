#pragma once

#include "..\\RocketGraphicsInterface\\IRocketGraphics.h"
#include "Camera.h"

#include <windows.h>
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace RocketCore::Graphics
{
	class Grid;
	class Axis;
	
	class RocketDX11 final : public IRocketGraphics
	{
	public:
		RocketDX11();
		~RocketDX11();

	public:
		//그래픽스 엔진을 초기화한다.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight, bool isEditor = false) override;

		virtual void UpdateCamera(const CameraData& cameraData) override;
		virtual void UpdateConstantData(const RenderConstantData& renderConstData) override;

		virtual void Render() override;

		virtual void OnResize(int _width, int _height) override;

		virtual void Finalize() override;

		/// 렌더스테이트 제작 함수
	private:
		void CreateRenderStates();

	private:
		void BeginRender();
		void BeginRender(float r, float g, float b, float a);
		void RenderMesh();
		void RenderText();
		void RenderTexture();
		void EndRender();

		/// Initialize Member
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;
		bool _vSyncEnabled;

		/// 초기화 관련
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> _device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;	// immediateContext
		D3D_FEATURE_LEVEL _featureLevel;

		UINT _m4xMsaaQuality;

		Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _backBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> _depthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _depthStencilView;
		D3D11_VIEWPORT _viewport;

		/// Render State
		// 미리 여러 세트를 만들어두고 스위칭한다.
	private:
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> _wireframeRenderState;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> _solidRenderState;
		// 폰트때문에 뎁스스탠실 스테이트가 강제가 됐다.
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _NormalDepthStencilState;

	private:
		Grid* _grid;
		Axis* _axis;
		Camera _camera;

	};
}

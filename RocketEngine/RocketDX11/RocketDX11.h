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

using Microsoft::WRL::ComPtr;

namespace RocketCore::Graphics
{
	class Grid;
	class Axis;
	class VertexShader;
	class PixelShader;
	
	class RocketDX11 final : public IRocketGraphics
	{
	public:
		RocketDX11();
		~RocketDX11();

	public:
		//�׷��Ƚ� ������ �ʱ�ȭ�Ѵ�.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight, bool isEditor = false) override;

		virtual void UpdateCamera(const CameraData& cameraData) override;
		virtual void UpdateConstantData(const RenderConstantData& renderConstData) override;

		virtual void Render() override;

		virtual void OnResize(int _width, int _height) override;

		virtual void Finalize() override;

		/// ����������Ʈ ���� �Լ�
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

		/// �ʱ�ȭ ����
	private:
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _deviceContext;	// immediateContext
		D3D_FEATURE_LEVEL _featureLevel;

		UINT _m4xMsaaQuality;

		ComPtr<IDXGISwapChain> _swapChain;
		ComPtr<ID3D11Texture2D> _backBuffer;
		ComPtr<ID3D11RenderTargetView> _renderTargetView;
		ComPtr<ID3D11Texture2D> _depthStencilBuffer;
		ComPtr<ID3D11DepthStencilView> _depthStencilView;
		D3D11_VIEWPORT _viewport;

		/// Render State
		// �̸� ���� ��Ʈ�� �����ΰ� ����Ī�Ѵ�.
	private:
		ComPtr<ID3D11RasterizerState> _wireframeRenderState;
		ComPtr<ID3D11RasterizerState> _solidRenderState;
		// ��Ʈ������ �������Ľ� ������Ʈ�� ������ �ƴ�.
		ComPtr<ID3D11DepthStencilState> _NormalDepthStencilState;

	private:
		Grid* _grid;
		Axis* _axis;
		Camera _camera;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
	};
}

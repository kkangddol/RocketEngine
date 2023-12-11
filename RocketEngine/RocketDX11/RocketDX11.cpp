#include "RocketDX11.h"
#include "Grid.h"
#include "Axis.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "RocketMacroDX11.h"
#include "DeviceBuilderDX11.h"

#include <cassert>

/// IRocketGraphics.h �� �ִ� dllexport �Լ���.
/// USE_DX11 �� ��ó���⿡ �־��ָ� DX11�� dll�� ���������.
/// 
/// 23.06.16 ������ �����.
namespace RocketCore::Graphics
{
	IRocketGraphics* CreateGraphicsInstance()
	{
		return new RocketDX11();
	}

	void ReleaseGraphicsInstance(IRocketGraphics* instance)
	{
		delete instance;
	}

	RocketDX11::RocketDX11()
		: _hWnd(), _screenWidth(), _screenHeight(), _vSyncEnabled(),
		_device(), _deviceContext(),
		_featureLevel(),_m4xMsaaQuality(),
		_swapChain(), _backBuffer(),
		_renderTargetView(), _depthStencilBuffer(), _depthStencilView(),
		_viewport(), _wireframeRenderState(), _solidRenderState(), _NormalDepthStencilState()
	{

	}

	RocketDX11::~RocketDX11()
	{

	}

	void RocketDX11::Initialize(void* hWnd, int screenWidth, int screenHeight, bool isEditor /*= false*/)
	{
		// ��ũ�η� �����Ϸ��� �۾���
		HRESULT hr = S_OK;

		_hWnd = static_cast<HWND>(hWnd);
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		// device ���� Ŭ������ �̿��� device�� deviceContext ����.
		DeviceBuilderDX11 deviceBuilder;
		deviceBuilder.SetDevice(_device.GetAddressOf());
		deviceBuilder.SetLevelHolder(&_featureLevel);
		deviceBuilder.SetDeviceContext(_deviceContext.GetAddressOf());
		HR(deviceBuilder.Build());

		/// �Ʒ� if���� ��å������ �ڵ�
		/// ������ 11_0�� �ƴҶ� false�� �����Ѵ�.
		/// �� �ڵ忡���� 11_1�� ����ϴµ� �̷��� �Ǵ°ǰ�?!
		/// 23.04.07 ������ �����
	// 	if (_featureLevel != D3D_FEATURE_LEVEL_11_0)
	// 	{
	// 		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
	// 		return false;
	// 	}

		/// ��Ƽ ���ø� ǰ������ üũ
		/// Direct11 ������ �׻� �����ǹǷ�, ��ȯ�� ǰ�� ���� ���� �ݵ�� 0���� Ŀ�� �Ѵ�.
		/// 23.04.07 ������ �����
		hr = _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_m4xMsaaQuality);
		assert(_m4xMsaaQuality > 0);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 4;      //multisampling setting	// ���� 4x�� ����ϹǷ� 4
		swapChainDesc.SampleDesc.Quality = _m4xMsaaQuality - 1;	//vendor-specific flag	// ������ �޾ƿ� ����Ƽ ������ �־��ش�.	// -1 �� �����ٱ�?
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;		// �̰� 2�� �ϰԵǸ� ����Ÿ�� �並 ������ ���ۿ� ���� ������ �־���ϳ�?
		swapChainDesc.OutputWindow = _hWnd;
		swapChainDesc.Windowed = TRUE; // Sets the initial state of full-screen mode.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// ��ȯȿ��. DXGI_SWAP_EFFECT_DISCARD�� ���÷��� �����Ⱑ ���� ȿ������ ���� ����� �����ϰ� ��
		//desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// MSDN �״�� ���� ģ ��.
		swapChainDesc.Flags = 0;

		/// DXGIDevice�� DXGIAdapter�� �����
		/// DXGIAdapter�� DXGIFactory�� �����
		/// DXGIFactory�� SwapChain�� �����!
		/// ���� swapchain ���� �̿��� ���� ������� swapchain�� ����� ��!
		/// 
		/// 23.04.10 ������ �����
		// Create the DXGI device object to use in other factories, such as Direct2D.
		Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
		_device.As(&dxgiDevice);

		// Create swap chain.
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
		Microsoft::WRL::ComPtr<IDXGIFactory> factory;

		hr = dxgiDevice->GetAdapter(&adapter);

		if (SUCCEEDED(hr))
		{
			adapter->GetParent(IID_PPV_ARGS(&factory));

			hr = factory->CreateSwapChain(
				_device.Get(),
				//_device.Get(),
				&swapChainDesc,
				&_swapChain
			);
		}

		hr = _swapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			(void**)&_backBuffer);

		hr = _device->CreateRenderTargetView(
			_backBuffer.Get(),
			nullptr,
			_renderTargetView.GetAddressOf()
		);

		D3D11_TEXTURE2D_DESC backBufferDesc;

		_backBuffer->GetDesc(&backBufferDesc);

		CD3D11_TEXTURE2D_DESC depthStencilDesc(
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			static_cast<UINT> (backBufferDesc.Width),
			static_cast<UINT> (backBufferDesc.Height),
			1, // This depth stencil view has only one texture.
			1, // Use a single mipmap level.
			D3D11_BIND_DEPTH_STENCIL
		);

		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = _m4xMsaaQuality - 1;

		hr = _device->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			&_depthStencilBuffer
		);

		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

		// 	hr = d3dDevice_->CreateDepthStencilView(
		// 		depthStencilBuffer_.Get(),
		// 		&depthStencilViewDesc,
		// 		&depthStencilView_
		// 	);

		hr = _device->CreateDepthStencilView(
			_depthStencilBuffer.Get(),
			nullptr,
			&_depthStencilView
		);

		/// RenderTargetView �� DepthStencilBuffer�� ��� ���� �ܰ�(Output Merger Stage)�� ���ε�
		_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());

		ZeroMemory(&_viewport, sizeof(D3D11_VIEWPORT));
		_viewport.Height = (float)backBufferDesc.Height;
		_viewport.Width = (float)backBufferDesc.Width;
		_viewport.MinDepth = 0;
		_viewport.MaxDepth = 1;

		_deviceContext->RSSetViewports(
			1,
			&_viewport
		);

		// Render State
		CreateRenderStates();

		_axis = new Axis(_device.Get(), _deviceContext.Get(), _wireframeRenderState.Get());
		_axis->Initialize();
		
		_vertexShader = new VertexShader();
		_vertexShader->CreateShader(_device.Get(), "../x64/Debug/VertexShader.cso");

		_pixelShader = new PixelShader();
		_pixelShader->CreateShader(_device.Get(), "../x64/Debug/PixelShader.cso");

		_grid = new Grid();
		_grid->Initialize(_device.Get());
	}

	void RocketDX11::OnResize(int _width, int _height)
	{

	}

	void RocketDX11::BeginRender()
	{
		float color[4];

		// Setup the color to clear the buffer to.
		color[0] = 0.1f;	// r
		color[1] = 0.1f;	// g
		color[2] = 0.1f;	// b
		color[3] = 0.1f;	// a
		// Clear the back buffer.
		_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), color);
		// Clear the depth buffer.
		_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		//d3dDeviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

		return;
	}

	void RocketDX11::BeginRender(float r, float g, float b, float a)
	{
		float color[4];

		// Setup the color to clear the buffer to.
		color[0] = r;	// r
		color[1] = g;	// g
		color[2] = b;	// b
		color[3] = a;	// a
		// Clear the back buffer.
		_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), color);
		// Clear the depth buffer.
		_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		//d3dDeviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

		return;
	}

	void RocketDX11::RenderMesh()
	{
		// �����ϴ� RenderableObject�� �׸��� ��

	}

	void RocketDX11::RenderText()
	{

	}

	void RocketDX11::RenderTexture()
	{

	}

	void RocketDX11::EndRender()
	{
		// Present the back buffer to the screen since rendering is complete.
		if (_vSyncEnabled)
		{
			// Lock to screen refresh rate.
			_swapChain->Present(1, 0);
		}
		else
		{
			// Present as fast as possible.
			_swapChain->Present(0, 0);
		}

		return;
	}



	void RocketDX11::CreateRenderStates()
	{
		// Render State �� Rasterizer State
		D3D11_RASTERIZER_DESC solidDesc;
		ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
		solidDesc.FillMode = D3D11_FILL_SOLID;
		solidDesc.CullMode = D3D11_CULL_BACK;
		solidDesc.FrontCounterClockwise = false;
		solidDesc.DepthClipEnable = true;

		HR(_device->CreateRasterizerState(&solidDesc, &_solidRenderState));

		D3D11_RASTERIZER_DESC wireframeDesc;
		ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
		wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
		wireframeDesc.CullMode = D3D11_CULL_BACK;
		wireframeDesc.FrontCounterClockwise = false;
		wireframeDesc.DepthClipEnable = true;

		HR(_device->CreateRasterizerState(&wireframeDesc, &_wireframeRenderState));

		//
		// ��Ʈ�� DSS
		//
		D3D11_DEPTH_STENCIL_DESC equalsDesc;
		ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		equalsDesc.DepthEnable = true;
		equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// ���̹��ۿ� ����� �Ѵ�
		equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		HR(_device->CreateDepthStencilState(&equalsDesc, &_NormalDepthStencilState));

	}

	void RocketDX11::UpdateCamera(const CameraData& cameraData)
	{
		_camera.SetPosition(cameraData.position.x, cameraData.position.y, cameraData.position.z);
		_camera.SetRotation(cameraData.rotation.w, cameraData.rotation.x, cameraData.rotation.y, cameraData.rotation.z);
		_camera.SetFrustum(cameraData.fovY, cameraData.aspect, cameraData.nearZ, cameraData.farZ);
		_camera.UpdateViewMatrix();
	}

	void RocketDX11::Render()
	{
		BeginRender(0.0f, 0.0f, 0.0f, 1.0f);
		_grid->Update(DirectX::XMMatrixIdentity(), _camera.GetViewMatrix(), _camera.GetProjectionMatrix());
		_grid->Render(_deviceContext.Get(), _vertexShader->GetVertexShader(), _pixelShader->GetPixelShader(), _vertexShader->GetMatrixBuffer(), _vertexShader->GetInputLayout(), _wireframeRenderState.Get());
		_axis->Update(DirectX::XMMatrixIdentity(), _camera.GetViewMatrix(), _camera.GetProjectionMatrix());
		_axis->Render();
		RenderMesh();
		EndRender();
	}

	void RocketDX11::Finalize()
	{
		delete _grid;
		delete _axis;
	}

	void RocketDX11::UpdateConstantData(const RenderConstantData& renderConstData)
	{

	}

}

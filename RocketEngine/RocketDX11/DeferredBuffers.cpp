﻿#include "DeferredBuffers.h"
#include "GraphicsMacro.h"

namespace Rocket::Core
{

	DeferredBuffers::DeferredBuffers()
		: _textureWidth(0)
		, _textureHeight(0)
	{

	}

	DeferredBuffers::~DeferredBuffers()
	{
		for (int i = 0; i < BUFFER_COUNT; i++)
		{
			_renderTargetTextureArray[i].Reset();
			_renderTargetViewArray[i].Reset();
			_shaderResourceViewArray[i].Reset();
		}

		_depthStencilBuffer.Reset();
		_depthStencilView.Reset();

		_shadowMapTexture.Reset();
		_shadowMapRenderTargetView.Reset();
		_shadowMapShaderResourceView.Reset();
		_shadowDepthBuffer.Reset();
		_shadowDepthStencilView.Reset();
	}

	void DeferredBuffers::Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear)
	{
		_textureWidth = textureWidth;
		_textureHeight = textureHeight;

		UINT m4xMsaaQuality;
		HR(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));

		// 렌더 타겟 텍스쳐 생성.
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = _textureWidth;
		textureDesc.Height = _textureHeight;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		for (int i = 0; i < BUFFER_COUNT; i++)
		{
			HR(device->CreateTexture2D(&textureDesc, nullptr, _renderTargetTextureArray[i].GetAddressOf()));
		}

		// 렌더 타겟 뷰 생성.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));


		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		for (int i = 0; i < BUFFER_COUNT; i++)
		{
			HR(device->CreateRenderTargetView(_renderTargetTextureArray[i].Get(), &renderTargetViewDesc, _renderTargetViewArray[i].GetAddressOf()));
		}

		// 셰이더 리소스 뷰 생성.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));

		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		for (int i = 0; i < BUFFER_COUNT; i++)
		{
			HR(device->CreateShaderResourceView(_renderTargetTextureArray[i].Get(), &shaderResourceViewDesc, _shaderResourceViewArray[i].GetAddressOf()));
		}

		// 깊이 스텐실 버퍼 생성.
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		depthBufferDesc.Width = _textureWidth;
		depthBufferDesc.Height = _textureHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0; // m4xMsaaQuality - 1;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		HR(device->CreateTexture2D(&depthBufferDesc, NULL, _depthStencilBuffer.GetAddressOf()));

		// 깊이 스텐실 뷰 생성.
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		HR(device->CreateDepthStencilView(_depthStencilBuffer.Get(), &depthStencilViewDesc, _depthStencilView.GetAddressOf()));


		/// Shadow Map 관련
		_shadowMapTexture;
		_shadowMapRenderTargetView;
		_shadowMapShaderResourceView;
		_shadowDepthBuffer;
		_shadowDepthStencilView;

		// ShadowMapTexture 생성
		HR(device->CreateTexture2D(&textureDesc, nullptr, _shadowMapTexture.GetAddressOf()));

		// ShadowMapRenderTargetView 생성
		HR(device->CreateRenderTargetView(_shadowMapTexture.Get(), &renderTargetViewDesc, _shadowMapRenderTargetView.GetAddressOf()));

		// ShadowMapShaderResourceView 생성
		HR(device->CreateShaderResourceView(_shadowMapTexture.Get(), &shaderResourceViewDesc, _shadowMapShaderResourceView.GetAddressOf()));

		// ShadowDepthBuffer 생성
		HR(device->CreateTexture2D(&depthBufferDesc, NULL, _shadowDepthBuffer.GetAddressOf()));

		// ShadowDepthStencilView 생성
		HR(device->CreateDepthStencilView(_shadowDepthBuffer.Get(), &depthStencilViewDesc, _shadowDepthStencilView.GetAddressOf()));
	}

	void DeferredBuffers::SetRenderTargets(ID3D11DeviceContext* deviceContext)
	{
		// 렌더링 대상 뷰 배열 및 깊이 스텐실 버퍼를 출력 렌더 파이프라인에 바인딩 합니다.
		deviceContext->OMSetRenderTargets(BUFFER_COUNT, _renderTargetViewArray->GetAddressOf(), _depthStencilView.Get());
	}

	void DeferredBuffers::SetRenderTargets(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView)
	{
		deviceContext->OMSetRenderTargets(BUFFER_COUNT, _renderTargetViewArray->GetAddressOf(), depthStencilView);
	}

	void DeferredBuffers::ClearRenderTargets(ID3D11DeviceContext* deviceContext, float r, float g, float b, float a)
	{
		float color[4] = { r, g, b, a };

		for (int i = 0; i < BUFFER_COUNT; i++)
		{
			deviceContext->ClearRenderTargetView(_renderTargetViewArray[i].Get(), color);
		}

		deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	ID3D11ShaderResourceView* DeferredBuffers::GetShaderResourceView(int index)
	{
		return _shaderResourceViewArray[index].Get();
	}

	ID3D11ShaderResourceView** DeferredBuffers::GetAddressOfShaderResourceView(int index)
	{
		return _shaderResourceViewArray[index].GetAddressOf();
	}

	void DeferredBuffers::ReleaseRenderTargets(ID3D11DeviceContext* deviceContext)
	{

	}

	void DeferredBuffers::SetShadowMapRenderTarget(ID3D11DeviceContext* deviceContext)
	{
		deviceContext->OMSetRenderTargets(1, _shadowMapRenderTargetView.GetAddressOf(), _shadowDepthStencilView.Get());
	}

}

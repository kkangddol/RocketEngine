#pragma once
#include "..\\RocketGraphicsInterface\\IRocketGraphics.h"

#include <windows.h>
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace RocketCore::Graphics
{
	class RocketDX12 final : public IRocketGraphics
	{
	public:
		//그래픽스 엔진을 초기화한다.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight, bool isEditor = false) override;

		virtual void UpdateCamera(const CameraData& cameraData) override;

		virtual void Render() override;

		virtual void OnResize(int _width, int _height) override;

		virtual void Finalize() override;
	};
}
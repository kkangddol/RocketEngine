#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class VertexShader;
	class PixelShader;
}


namespace Rocket::Core
{
	class ShadowPass
	{
	public:
		ShadowPass();
		~ShadowPass();

	public:
		void Initialize(VertexShader* vs, PixelShader* ps);
		void GenerateShadowMap(ID3D11DeviceContext* deviceContext);

	private:
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
	};

}


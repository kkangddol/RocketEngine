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
	class LightPass
	{
	public:
		LightPass();
		~LightPass();

	public:
		void Initialize(VertexShader* vertexShader, PixelShader* pixelShader);
		void Render(ID3D11DeviceContext* deviceContext);

	private:
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
	};
}

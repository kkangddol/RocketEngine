#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class VertexShader;
	class PixelShader;
	class DeferredBuffers;
}


namespace Rocket::Core
{
	class ShadowPass
	{
	public:
		ShadowPass();
		~ShadowPass();

	public:
		void Initialize(VertexShader* staticMeshVS, PixelShader* staticMeshPS, VertexShader* dynamicModelVS, PixelShader* dynamicModelPS);
		void GenerateShadowMap(ID3D11DeviceContext* deviceContext, DeferredBuffers* g_buffer); 

	private:
		VertexShader* _staticMeshVS;
		PixelShader* _staticMeshPS;
		VertexShader* _dynamicModelVS;
		PixelShader* _dynamicModelPS;
	};

}


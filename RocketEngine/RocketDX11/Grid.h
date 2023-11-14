#pragma once
#include "..\\RocketGraphicsInterface\\IRenderable.h"
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

#include "d3dx11Effect.h"	// effect, tech

namespace RocketCore::Graphics
{
	class Grid : public IRenderable
	{
	public:
		Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
		~Grid();

		struct Vertex
		{
			DirectX::XMFLOAT3 Pos;
			DirectX::XMFLOAT4 Color;
		};

	public:
		void Initialize();
		void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
		void UpdateRenderData() override;
		void Render();


	private:
		void BuildGeometryBuffers();
		void BuildFX();
		void BuildVertexLayout();


		// 뭐뭐 있어야 하니..
	private:
		ID3D11Device* md3dDevice;						// D3D11 디바이스
		ID3D11DeviceContext* md3dImmediateContext;		// 디바이스 컨텍스트

		ID3D11Buffer* mVB;
		ID3D11Buffer* mIB;

		ID3DX11Effect* mFX;
		ID3DX11EffectTechnique* mTech;
		ID3DX11EffectMatrixVariable* mfxWorldViewProj;

		ID3D11InputLayout* mInputLayout;

		DirectX::XMMATRIX mWorld;	// Transform Matrix
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProj;

		// 와이어로 그리기 위한 RenderState. 쉐이더에서 해도 된다.
		// 예제에서 On/Off로 바꾸는 등의 일을 하는 경우 이것을 바꿔줘도 될 것 같다.
		// 쉐이더에서 하는 경우도 스위칭 비용도 줄일 수 있는 방법은 많다.
		ID3D11RasterizerState* m_pRenderstate;;


	};
}


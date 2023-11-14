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


		// ���� �־�� �ϴ�..
	private:
		ID3D11Device* md3dDevice;						// D3D11 ����̽�
		ID3D11DeviceContext* md3dImmediateContext;		// ����̽� ���ؽ�Ʈ

		ID3D11Buffer* mVB;
		ID3D11Buffer* mIB;

		ID3DX11Effect* mFX;
		ID3DX11EffectTechnique* mTech;
		ID3DX11EffectMatrixVariable* mfxWorldViewProj;

		ID3D11InputLayout* mInputLayout;

		DirectX::XMMATRIX mWorld;	// Transform Matrix
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProj;

		// ���̾�� �׸��� ���� RenderState. ���̴����� �ص� �ȴ�.
		// �������� On/Off�� �ٲٴ� ���� ���� �ϴ� ��� �̰��� �ٲ��൵ �� �� ����.
		// ���̴����� �ϴ� ��쵵 ����Ī ��뵵 ���� �� �ִ� ����� ����.
		ID3D11RasterizerState* m_pRenderstate;;


	};
}


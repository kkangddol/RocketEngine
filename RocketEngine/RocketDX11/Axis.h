#pragma once
#include "..\\RocketGraphicsInterface\\IRenderable.h"

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>

namespace RocketCore::Graphics
{
	class Axis : public IRenderable
	{
	public:
		Axis(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
		~Axis();

		struct Vertex
		{
			DirectX::XMFLOAT3 Pos;
			DirectX::XMFLOAT4 Color;
		};

		struct MatrixBufferType
		{
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};

	public:
		void Initialize();
		void Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
		virtual void UpdateRenderData() override;
		void Render();

	private:
		void BuildGeometryBuffers();

	private:
		ID3D11Device* _device;						// D3D11 ����̽�
		ID3D11DeviceContext* _deviceContext;		// ����̽� ���ؽ�Ʈ

		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;

		ID3D11InputLayout* _inputLayout;

		DirectX::XMMATRIX _world;	// Transform Matrix
		DirectX::XMMATRIX _view;
		DirectX::XMMATRIX _proj;

		// ���̾�� �׸��� ���� RenderState. ���̴����� �ص� �ȴ�.
		// �������� On/Off�� �ٲٴ� ���� ���� �ϴ� ��� �̰��� �ٲ��൵ �� �� ����.
		// ���̴����� �ϴ� ��쵵 ����Ī ��뵵 ���� �� �ִ� ����� ����.
		ID3D11RasterizerState* _renderstate;;

		ID3D11VertexShader* _vertexShader;
		ID3D11PixelShader* _pixelShader;
		ID3D11Buffer* _matrixBuffer;

		void CreateShader();


	};
}

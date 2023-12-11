#include "Grid.h"
#include <DirectXColors.h>	// �̹� ���� �κ� DX11�� ��ģ��.
#include <sstream>
#include <fstream>
#include <vector>
#include "RocketMacroDX11.h"

namespace RocketCore::Graphics
{
	Grid::Grid()
		: _vertexBuffer(nullptr), _indexBuffer(nullptr),
		_world(), _view(), _proj()
	{

	}

	Grid::~Grid()
	{
		_vertexBuffer.Reset();
		_indexBuffer.Reset();
	}


	void Grid::Initialize(ID3D11Device* device)
	{
		BuildGeometryBuffers(device);
	}

	void Grid::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		_world = world;
		_view = view;
		_proj = proj;
	}

	void Grid::Render(ID3D11DeviceContext* deviceContext, ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11Buffer* matrixBuffer, ID3D11InputLayout* inputLayout, ID3D11RasterizerState* renderstate)
	{
		// Grid�� ���� Shader deviceContext �̿��� ����.
		deviceContext->VSSetShader(vertexShader, nullptr, 0);
		deviceContext->PSSetShader(pixelShader, nullptr, 0);

		// �Է� ��ġ ��ü ����
		deviceContext->IASetInputLayout(inputLayout);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// �ε������ۿ� ���ؽ����� ����
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBufferType* dataPtr;
		unsigned int bufferNumber;

		_world = DirectX::XMMatrixTranspose(_world);
		_view = DirectX::XMMatrixTranspose(_view);
		_proj = DirectX::XMMatrixTranspose(_proj);

		HR(deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		dataPtr = (MatrixBufferType*)mappedResource.pData;

		dataPtr->world = _world;
		dataPtr->view = _view;
		dataPtr->projection = _proj;

		deviceContext->Unmap(matrixBuffer, 0);

		bufferNumber = 0;

		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

		// ����������Ʈ
		deviceContext->RSSetState(renderstate);

		deviceContext->DrawIndexed(40, 0, 0);
	}

	void Grid::BuildGeometryBuffers(ID3D11Device* device)
	{
		// ���� ���۸� �����Ѵ�. 
		// 40���� ������ �������.
		Vertex vertices[100];
		for (int i = 0; i < 100; i++)
		{
			vertices[i].Pos = DirectX::XMFLOAT3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
			vertices[i].Color = DirectX::XMFLOAT4((const float*)&DirectX::Colors::DarkGray);	// (��ο� ȸ��)
		}

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * 100;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;
		HR(device->CreateBuffer(&vbd, &vinitData, &_vertexBuffer));


		// �ε��� ���۸� �����Ѵ�.
		// ���� 40���� ������ ��Ÿ������ �ߴ�.
		UINT indices[40];
		for (int i = 0; i < 10; i++)
		{
			indices[i * 2] = i;
			indices[i * 2 + 1] = i + 90;
		}

		for (int i = 0; i < 10; i++)
		{
			indices[20 + (i * 2)] = i * 10;
			indices[20 + (i * 2) + 1] = i * 10 + 9;
		}

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * 40;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		HR(device->CreateBuffer(&ibd, &iinitData, &_indexBuffer));
	}
}
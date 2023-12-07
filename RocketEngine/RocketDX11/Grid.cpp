#include "Grid.h"
#include <DirectXColors.h>	// �̹� ���� �κ� DX11�� ��ģ��.
#include <sstream>
#include <fstream>
#include <vector>
#include "RocketMacroDX11.h"

#ifdef _DEBUG
#pragma comment( lib, "../Lib/Effects11d.lib" )
#else
#pragma comment( lib, "../Lib/Effects11.lib" )
#endif

namespace RocketCore::Graphics
{
	Grid::Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
		: md3dDevice(pDevice), md3dImmediateContext(pDeviceContext), m_pRenderstate(pRS),
		mVB(nullptr), mIB(nullptr), mInputLayout(nullptr),
		mWorld(), mView(), mProj(),
		_vertexShader(nullptr),_pixelShader(nullptr)
	{

	}

	Grid::~Grid()
	{
		ReleaseCOM(mInputLayout);

		ReleaseCOM(mVB);
		ReleaseCOM(mIB);
	}


	void Grid::Initialize()
	{
		BuildGeometryBuffers();
		CreateShader();
	}

	void Grid::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		mWorld = world;
		mView = view;
		mProj = proj;
	}

	void Grid::Render()
	{
		// Grid�� ���� Shader deviceContext �̿��� ����.
		md3dImmediateContext->VSSetShader(_vertexShader, nullptr, 0);
		md3dImmediateContext->PSSetShader(_pixelShader, nullptr, 0);

		// �Է� ��ġ ��ü ����
		md3dImmediateContext->IASetInputLayout(mInputLayout);
		md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// �ε������ۿ� ���ؽ����� ����
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		/// WVP TM���� ����
		// Set constants
		DirectX::XMMATRIX worldViewProj = mWorld * mView * mProj;

		// ����������Ʈ
		md3dImmediateContext->RSSetState(m_pRenderstate);

		md3dImmediateContext->DrawIndexed(40, 0, 0);
	}

	void Grid::BuildGeometryBuffers()
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
		HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));


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
		HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
	}

	void Grid::UpdateRenderData()
	{

	}

	void Grid::CreateShader()
	{
		std::ifstream vsFile("../x64/Debug/VertexShader.cso", std::ios::binary);
		std::ifstream psFile("../x64/Debug/PixelShader.cso", std::ios::binary);

		std::vector<char> vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

		md3dDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &_vertexShader);
		md3dDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &_pixelShader);


		// ���̴��� �����ΰ� ���̾ƿ��� �����ΰ� �̷��� ������ ���� �� �� �ְ��ϴ°� ������
		// �̷��� �ڵ������� �ھƵθ� ������ �� ���ٴ� ��⸦ �ϴ� �� ������?

		// Create the vertex input layout.
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			//{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
			{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		md3dDevice->CreateInputLayout(vertexDesc, 2, vsData.data(), vsData.size(), &mInputLayout);
	}

}
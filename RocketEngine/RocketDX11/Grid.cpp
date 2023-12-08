#include "Grid.h"
#include <DirectXColors.h>	// 이미 많은 부분 DX11과 겹친다.
#include <sstream>
#include <fstream>
#include <vector>
#include "RocketMacroDX11.h"

namespace RocketCore::Graphics
{
	Grid::Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
		: _device(pDevice), _deviceContext(pDeviceContext), _renderstate(pRS),
		mVB(nullptr), mIB(nullptr), _inputLayout(nullptr),
		_world(), _view(), _proj(),
		_vertexShader(nullptr),_pixelShader(nullptr)
	{

	}

	Grid::~Grid()
	{
		ReleaseCOM(_inputLayout);

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
		_world = world;
		_view = view;
		_proj = proj;
	}

	void Grid::Render()
	{
		// Grid가 쓰는 Shader deviceContext 이용해 연결.
		_deviceContext->VSSetShader(_vertexShader, nullptr, 0);
		_deviceContext->PSSetShader(_pixelShader, nullptr, 0);

		// 입력 배치 객체 셋팅
		_deviceContext->IASetInputLayout(_inputLayout);
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		_deviceContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
		_deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

		/// WVP TM등을 셋팅
		// Set constants
		DirectX::XMMATRIX worldViewProj = _world * _view * _proj;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		MatrixBufferType* dataPtr;
		unsigned int bufferNumber;

		_world = DirectX::XMMatrixTranspose(_world);
		_view = DirectX::XMMatrixTranspose(_view);
		_proj = DirectX::XMMatrixTranspose(_proj);

		HR(_deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

		dataPtr = (MatrixBufferType*)mappedResource.pData;

		dataPtr->world = _world;
		dataPtr->view = _view;
		dataPtr->projection = _proj;

		_deviceContext->Unmap(_matrixBuffer, 0);

		bufferNumber = 0;

		_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

		// 렌더스테이트
		_deviceContext->RSSetState(_renderstate);

		_deviceContext->DrawIndexed(40, 0, 0);
	}

	void Grid::BuildGeometryBuffers()
	{
		// 정점 버퍼를 생성한다. 
		// 40개의 정점을 만들었다.
		Vertex vertices[100];
		for (int i = 0; i < 100; i++)
		{
			vertices[i].Pos = DirectX::XMFLOAT3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
			vertices[i].Color = DirectX::XMFLOAT4((const float*)&DirectX::Colors::DarkGray);	// (어두운 회색)
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
		HR(_device->CreateBuffer(&vbd, &vinitData, &mVB));


		// 인덱스 버퍼를 생성한다.
		// 역시 40개의 라인을 나타내도록 했다.
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
		HR(_device->CreateBuffer(&ibd, &iinitData, &mIB));
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

		_device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &_vertexShader);
		_device->CreatePixelShader(psData.data(), psData.size(), nullptr, &_pixelShader);


		// 셰이더도 만들어두고 레이아웃도 만들어두고 이런거 저런거 갖다 쓸 수 있게하는게 좋겠지
		// 이렇게 코드적으로 박아두면 안좋을 것 같다는 얘기를 하는 것 같은데?

		// Create the vertex input layout.
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			//{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
			{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

		HR(_device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer));

		_device->CreateInputLayout(vertexDesc, 2, vsData.data(), vsData.size(), &_inputLayout);
	}

}
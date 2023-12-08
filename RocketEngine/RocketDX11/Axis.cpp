#include "Axis.h"
#include <DirectXColors.h>	// �̹� ���� �κ� DX11�� ��ģ��.
#include <sstream>
#include <fstream>
#include <vector>
#include "RocketMacroDX11.h"

namespace RocketCore::Graphics
{
	Axis::Axis(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS)
		: _device(pDevice), _deviceContext(pDeviceContext), _renderstate(pRS),
		_vertexBuffer(nullptr), _indexBuffer(nullptr), _inputLayout(nullptr),
		_world(), _view(), _proj(),
		_vertexShader(nullptr), _pixelShader(nullptr)
	{

	}

	Axis::~Axis()
	{
		ReleaseCOM(_inputLayout);

		ReleaseCOM(_vertexBuffer);
		ReleaseCOM(_indexBuffer);
	}


	void Axis::Initialize()
	{
		BuildGeometryBuffers();
		CreateShader();
	}

	void Axis::Update(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		_world = world;
		_view = view;
		_proj = proj;
	}

	void Axis::Render()
	{
		// Axis�� ���� Shader deviceContext �̿��� ����.
		_deviceContext->VSSetShader(_vertexShader, nullptr, 0);
		_deviceContext->PSSetShader(_pixelShader, nullptr, 0);

		// �Է� ��ġ ��ü ����
		_deviceContext->IASetInputLayout(_inputLayout);
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// �ε������ۿ� ���ؽ����� ����
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
		_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

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

		// ����������Ʈ
		_deviceContext->RSSetState(_renderstate);

		_deviceContext->DrawIndexed(6, 0, 0);
	}

	void Axis::BuildGeometryBuffers()
	{
		// ���� ���۸� �����Ѵ�. 
		// �� �࿡ �µ��� 6���� ������ �������.
		Vertex vertices[] =
		{
			{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Red)  },	// x�� (����)
			{ DirectX::XMFLOAT3(10.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Red)  },

			{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Green)},	// y�� (�ʷ�)
			{ DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Green)},

			{ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Blue)	},	// z�� (�Ķ�)
			{ DirectX::XMFLOAT3(0.0f, 0.0f, 10.0f), DirectX::XMFLOAT4((const float*)&DirectX::Colors::Blue) }
		};

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(Vertex) * 6;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;
		HR(_device->CreateBuffer(&vbd, &vinitData, &_vertexBuffer));


		// �ε��� ���۸� �����Ѵ�.
		// ���� 3���� ���� ��Ÿ������ �ߴ�.

		UINT indices[] = {
			// x��
			0, 1,

			// y��
			2, 3,

			// z��
			4, 5,
		};

		D3D11_BUFFER_DESC ibd;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(UINT) * 6;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;
		HR(_device->CreateBuffer(&ibd, &iinitData, &_indexBuffer));
	}

	void Axis::UpdateRenderData()
	{

	}

	void Axis::CreateShader()
	{
		std::ifstream vsFile("../x64/Debug/VertexShader.cso", std::ios::binary);
		std::ifstream psFile("../x64/Debug/PixelShader.cso", std::ios::binary);

		std::vector<char> vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
		std::vector<char> psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

		_device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &_vertexShader);
		_device->CreatePixelShader(psData.data(), psData.size(), nullptr, &_pixelShader);


		// ���̴��� �����ΰ� ���̾ƿ��� �����ΰ� �̷��� ������ ���� �� �� �ְ��ϴ°� ������
		// �̷��� �ڵ������� �ھƵθ� ������ �� ���ٴ� ��⸦ �ϴ� �� ������?

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

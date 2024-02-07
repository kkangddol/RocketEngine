#include "Mesh.h"
#include "ResourceManager.h"
#include "GraphicsMacro.h"

namespace Rocket::Core
{
	Mesh::Mesh()
		: _vertexBuffer(), _indexBuffer(),
		vertexCount(0),
		indexCount(0),
		_vertexType(VertexType::VERTEX)
	{

	}

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices)
		: _vertexBuffer(), _indexBuffer(),
		vertexCount((int)vertices.size()),
		indexCount((int)indices.size()),
		_vertexType(VertexType::VERTEX)
	{
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = {};
		vertexData.pSysMem = vertices.data();

		HR(ResourceManager::Instance().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer));

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(UINT) * indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = indices.data();

		HR(ResourceManager::Instance().GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer));
	}

	ID3D11Buffer* Mesh::GetVertexBuffer() const
	{
		return _vertexBuffer.Get();
	}

	ID3D11Buffer* Mesh::GetIndexBuffer() const
	{
		return _indexBuffer.Get();
	}

	ID3D11Buffer** Mesh::GetAddressOfVertexBuffer()
	{
		return _vertexBuffer.GetAddressOf();
	}

	ID3D11Buffer** Mesh::GetAddressOfIndexBuffer()
	{
		return _indexBuffer.GetAddressOf();
	}

	int Mesh::GetVertexCount() const
	{
		return vertexCount;
	}

	int Mesh::GetIndexCount() const
	{
		return indexCount;
	}

	void Mesh::SetVertexType(VertexType type)
	{
		_vertexType = type;
	}

	VertexType Mesh::GetVertexType() const
	{
		return _vertexType;
	}



}

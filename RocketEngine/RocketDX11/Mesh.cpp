#include "Mesh.h"

namespace Rocket::Core
{
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
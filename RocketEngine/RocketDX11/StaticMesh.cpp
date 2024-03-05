#include "StaticMesh.h"

namespace Rocket::Core
{
	StaticMesh::StaticMesh()
	{

	}

	StaticMesh::StaticMesh(std::vector<Vertex> vertices, std::vector<UINT> indices)
		: Mesh(vertices, indices)
	{

	}
}

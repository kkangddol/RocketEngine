#include "SkinnedMesh.h"

namespace Rocket::Core
{
	SkinnedMesh::SkinnedMesh()
	{

	}

	SkinnedMesh::SkinnedMesh(std::vector<Vertex> vertices, std::vector<UINT> indices)
		: Mesh(vertices, indices)
	{

	}
}

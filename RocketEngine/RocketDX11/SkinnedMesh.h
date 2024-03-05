#pragma once
#include "Mesh.h"

namespace Rocket::Core
{
	class SkinnedMesh : public Mesh
	{
	public:
		SkinnedMesh();
		SkinnedMesh(std::vector<Vertex> vertices, std::vector<UINT> indices);
	};
}


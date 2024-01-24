#pragma once
#include <string>
#include "IRenderable.h"

namespace Rocket::Core
{
	class IStaticMesh : public IRenderable
	{
	public:
		virtual ~IStaticMesh() {}
		virtual void LoadMesh(const std::string& fileName) = 0;
		virtual void LoadNormalMap(const std::string& fileName) = 0;
		virtual void LoadDiffuseMap(const std::string& fileName) = 0;
		virtual void LoadVertexShader(const std::string& fileName) = 0;
		virtual void LoadPixelShader(const std::string& fileName) = 0;
	};
}
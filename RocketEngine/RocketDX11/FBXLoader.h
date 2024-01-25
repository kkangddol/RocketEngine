#pragma once

#include <string>
// #include <Importer.hpp>      // C++ importer interface
// #include <scene.h>           // Output data structure
// #include <postprocess.h>     // Post processing flags

namespace Rocket::Core
{
	class FBXLoader
	{
	public:
		FBXLoader();

		void Initialize();

		//bool DoTheImportThing(const std::string& path, Entity* parentEntity);

	};
}

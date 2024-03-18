#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "Singleton.h"

namespace Rocket::Core
{
	class RawModel;
}

namespace Rocket::Core
{
	class ResourceManager : public Singleton<ResourceManager>
	{
		friend Singleton;
	private:
		ResourceManager();		// 싱글턴이기 때문에 외부에서 생성할 수 없도록.



	private:
		std::unordered_map<std::string, RawModel*> _modelMap;
	};
}

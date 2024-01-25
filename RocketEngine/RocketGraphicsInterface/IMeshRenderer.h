#pragma once
#include <string>
#include "IRenderable.h"

namespace Rocket::Core
{
	class IMeshRenderer : public IRenderable
	{
	public:
		virtual ~IMeshRenderer() {}
	};
}
#pragma once
#include "..\\RocketMath\\RocketMath.h"
#include "MaterialData.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketCore::Graphics
{
	class IRenderable abstract
	{
	public:
		virtual void UpdateRenderData() abstract;
	};
}

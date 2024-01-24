#pragma once
#include "MathHeader.h"

namespace RocketCore::Graphics
{
	class ISketchable abstract
	{
	public:
		virtual void SetData(const char* data) abstract;
		virtual void SetAlpha(float alpha) abstract;
		virtual void SetFillRatio(float ratio) abstract;
		virtual void Render(const Matrix& rectTM) abstract;
		//path,text
	};
}

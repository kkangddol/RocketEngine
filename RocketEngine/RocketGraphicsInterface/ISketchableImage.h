﻿#pragma once
#include <string>
#include "ISketchable.h"

namespace Rocket::Core
{
	class ISketchableImage : public ISketchable
	{
	public:
		virtual void SetImage(const std::string& filePath) = 0;
		virtual void SetScreenSpacePosition(float x, float y) = 0;
		virtual float GetScreenSpacePositionX() = 0;
		virtual float GetScreenSpacePositionY() = 0;
		virtual DirectX::FXMVECTOR SetColor(DirectX::FXMVECTOR color) = 0;
		virtual void SetWorldSpace() = 0;
		virtual float GetWidth() override = 0;
		virtual float GetHeight() override = 0;
		virtual void ChangeScale(float x, float y) = 0;
	};
}

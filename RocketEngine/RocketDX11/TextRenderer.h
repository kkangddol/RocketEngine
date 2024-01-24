#pragma once

#include <d3d11.h>
#include <memory>
#include <DirectXColors.h>
#include <string>
#include <DXTK/SpriteFont.h>

#include "..\\HODO3DGraphicsInterface\\ISketchableText.h"

namespace Rocket::Core
{
	/// <summary>
	/// 텍스트랜더러 클래스
	/// </summary>
	class TextRenderer : public Rocket::Core::ISketchableText
	{
	public:
		TextRenderer();
		~TextRenderer();

		virtual void SetText(const std::string& str) override;

		virtual void SetFloatValue(const float value) override;

		virtual void SetintValue(const int value) override;

		virtual void SetColor(DirectX::FXMVECTOR color);

		virtual const std::string GetText() override;

		virtual void SetScreenSpacePosition(float x, float y) override;

		virtual void SetScreenSpace() override;

		virtual void SetWorldSpace() override;

		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& worldTM) override;

		virtual void SetActive(bool isActive) override;

	public:
		void Render(DirectX::SpriteBatch* spriteBatch);

		DirectX::SpriteFont* _font;

	private:
		// 내부 변수들
		std::string _str;
		float _renderFloat;
		int _renderInt;
		DirectX::XMVECTOR _color;
		float _xLocation;
		float _yLocation;

		bool _isTranslated;

	};
}
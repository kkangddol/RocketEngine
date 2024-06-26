﻿#include "TextRenderer.h"

namespace Rocket::Core
{
	TextRenderer::TextRenderer()
		: _isActive(true),
		_font(nullptr), 
		_text("Default Text"), 
		_color(DirectX::Colors::White),
		_worldTM(Matrix::Identity)
	{

	}

	TextRenderer::~TextRenderer()
	{

	}

	void TextRenderer::SetText(const std::string& str)
	{
		_text = str;
	}

	std::string& TextRenderer::GetText()
	{
		return _text;
	}

	void TextRenderer::SetWorldTM(const Matrix& worldTM)
	{
		_worldTM = worldTM;
	}

	void TextRenderer::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void TextRenderer::SetColor(Color color)
	{
		_color = color;
	}

	void TextRenderer::Render(DirectX::SpriteBatch* spriteBatch)
	{
		if(!_isActive)
			return;

		std::wstring wstr(_text.begin(), _text.end());
		_font->DrawString(spriteBatch, wstr.c_str(), DirectX::XMFLOAT2(_worldTM.m[3][0], _worldTM.m[3][1]), _color);
	}

	Color TextRenderer::GetColor()
	{
		return _color;
	}

	void TextRenderer::SetFont(DirectX::SpriteFont* font)
	{
		_font = font;
	}

	void TextRenderer::Append(const std::string& str)
	{
		_text.append(str);
	}

}

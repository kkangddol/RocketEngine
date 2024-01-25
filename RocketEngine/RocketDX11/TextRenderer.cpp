#include "TextRenderer.h"
#include "ResourceManager.h"

Rocket::Core::TextRenderer::TextRenderer()
	: _renderFloat(),
	_renderInt(),
	_xLocation(),
	_yLocation()
{
	_font = ResourceManager::Instance().GetDefaultFont();
	_str = "Default Text";
	_color = DirectX::Colors::White;
	_isTranslated = false;

}

Rocket::Core::TextRenderer::~TextRenderer()
{

}

void Rocket::Core::TextRenderer::SetText(const std::string& str)
{
	_str = str;
}

const std::string Rocket::Core::TextRenderer::GetText()
{
	return _str;
}

void Rocket::Core::TextRenderer::SetScreenSpace()
{

}

void Rocket::Core::TextRenderer::SetWorldSpace()
{

}

void Rocket::Core::TextRenderer::SetWorldTM(const Matrix& worldTM)
{
	if (_isTranslated != true)
	{
		_xLocation = worldTM._41;
		_yLocation = worldTM._42;
	}
}

void Rocket::Core::TextRenderer::SetActive(bool isActive)
{
	isActive = 1;
}

void Rocket::Core::TextRenderer::SetColor(DirectX::FXMVECTOR color)
{
	_color = color;
}

void Rocket::Core::TextRenderer::SetScreenSpacePosition(float x, float y)
{
	_xLocation = x;
	_yLocation = y;

	_isTranslated = true;
}

void Rocket::Core::TextRenderer::Render(DirectX::SpriteBatch* spriteBatch)
{
	std::wstring wstr(_str.begin(), _str.end());
	_font->DrawString(spriteBatch, wstr.c_str(), DirectX::XMFLOAT2(_xLocation, _yLocation), _color);
}

float Rocket::Core::TextRenderer::GetWidth()
{
	return 0.0f;
}

float Rocket::Core::TextRenderer::GetHeight()
{
	return 0.0f;
}


void Rocket::Core::TextRenderer::SetFloatValue(const float value)
{
	_renderFloat = value;
}

void Rocket::Core::TextRenderer::SetintValue(const int value)
{
	_renderInt = value;
}

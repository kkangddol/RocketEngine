#include "SpriteRenderer.h"

//����׿�
#include <windows.h>
#include "GameObject.h"
#include "Transform.h"
#include "TimeSystem.h"

namespace Rocket
{
	SpriteRenderer::SpriteRenderer()
		: _spritePath()
	{

	}

	std::string SpriteRenderer::GetSketchData()
	{
		return _spritePath;
	}

	void SpriteRenderer::SetPath(std::string path)
	{
		_spritePath = path;
	}

	std::string SpriteRenderer::GetPath() const
	{
		return _spritePath;
	}
}

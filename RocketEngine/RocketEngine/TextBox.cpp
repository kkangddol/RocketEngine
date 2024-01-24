#include "TextBox.h" 
#include "InputSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "DebugSystem.h"
#include "TimeSystem.h"

namespace Rocket
{
	float downTime = 0.0f;

	TextBox::TextBox(GameObject* owner)
		: Rocket::Core::UIRenderer(owner),
		_text("default"),
		_fontSize(20.0f),
		_color(1.0f, 1.0f, 1.0f, 1.0f)
	{

	}

	std::string TextBox::GetSketchData()
	{
		return _text;
	}

	void TextBox::Update()
	{
		if (!_isFocused)
		{
			return;
		}

		for (int keyCode = 0x30; keyCode <= 0x5A; keyCode++)
		{
			if (Rocket::Core::InputSystem::Instance().GetKeyDown(keyCode))
			{
				AppendText(keyCode);
			}
		}

		if (Rocket::Core::InputSystem::Instance().GetKeyDown(VK_BACK))
		{
			PopBackText();
		}

		if (Rocket::Core::InputSystem::Instance().GetKey(VK_BACK))
		{
			downTime += Rocket::Core::TimeSystem::Instance().GetDeltaTime();

			if (downTime >= 0.5f)
			{
				PopBackText();
			}
		}

		if (Rocket::Core::InputSystem::Instance().GetKeyUp(VK_BACK))
		{
			downTime = 0.0f;
		}

		if (Rocket::Core::InputSystem::Instance().GetKeyDown(VK_DELETE))
		{
			PopBackText();
		}

		if (Rocket::Core::InputSystem::Instance().GetKey(VK_DELETE))
		{
			downTime += Rocket::Core::TimeSystem::Instance().GetDeltaTime();

			if (downTime >= 0.5f)
			{
				PopBackText();
			}
		}

		if (Rocket::Core::InputSystem::Instance().GetKeyUp(VK_DELETE))
		{
			downTime = 0.0f;
		}
	}

	void TextBox::LateUpdate()
	{
		// 디버그 정보 출력.
		Vector3 worldPos = gameObject->transform.GetPosition();
		Vector2 LT = { worldPos.x - _width / 2, worldPos.y - _height / 2 };
		Vector2 RB = { worldPos.x + _width / 2, worldPos.y + _height / 2 };
		Vector4 color = { 0.0f,0.0f,1.0f,1.0f };
		Rocket::Core::DebugSystem::Instance().DrawDebug2DBox(LT, RB, color);
	}

	void TextBox::SetText(std::string text)
	{
		_text = text;
	}

	void TextBox::AppendText(std::string text)
	{
		_text += text;
	}

	void TextBox::AppendText(int keyCode)
	{
		_text += keyCode;
	}

	void TextBox::PopBackText()
	{
		if (_text.size() > 0)
		{
			_text.pop_back();
		}
	}

	std::string TextBox::GetText() const
	{
		return _text;
	}

	float TextBox::GetFontSize() const
	{
		return _fontSize;
	}

	Color TextBox::GetColor() const
	{
		return _color;
	}

	void TextBox::SetFontSize(float size)
{
		_fontSize = size;
	}

	void TextBox::SetFontColor(Color color)
	{
		_color = color;
	}

}

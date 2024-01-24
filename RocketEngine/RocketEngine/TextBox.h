#pragma once
#include "DLLExporter.h"
#include "UIRenderer.h"
#include <string>
#include "MathHeader.h"

namespace Rocket
{
	class GameObject;
}

namespace Rocket
{
	/// <summary>
	/// Text ������ ��� ����� ����ϴ� ������Ʈ.
	/// TextBox�� ���ҵ� �Ѵ�.
	/// 
	/// 23.08.11 ������
	/// </summary>
	class ROCKET_API TextBox : public Rocket::Core::UIRenderer
	{
	public:
		TextBox(GameObject* owner);

	public:
		virtual std::string GetSketchData() override;

	protected:
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		std::string GetText() const;
		void SetText(std::string text);

		void AppendText(std::string text);
		void AppendText(int keyCode);
		void PopBackText();

	public:
		float GetFontSize() const;
		Color GetColor() const;
		void SetFontSize(float size);
		void SetFontColor(Color color);

	private:
		std::string _text;
		float _fontSize;
		Color _color;
	};
}

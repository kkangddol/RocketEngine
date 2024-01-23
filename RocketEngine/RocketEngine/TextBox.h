#pragma once
#include "DLLExporter.h"
#include "UIRenderer.h"
#include <string>
#include "MathHeader.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif //_DEBUG

namespace RocketEngine
{
	class GameObject;
}

namespace RocketEngine
{
	/// <summary>
	/// Text ������ ��� ����� ����ϴ� ������Ʈ.
	/// TextBox�� ���ҵ� �Ѵ�.
	/// 
	/// 23.08.11 ������
	/// </summary>
	class ROCKET_API TextBox : public RocketCore::UIRenderer
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
		Vector4 GetColor() const;
		void SetFontSize(float size);
		void SetFontColor(Vector4 color);

	private:
		std::string _text;
		float _fontSize;
		Vector4 _color;
	};
}

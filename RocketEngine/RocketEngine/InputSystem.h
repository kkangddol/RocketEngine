#pragma once
#include "Singleton.h"
#include <windows.h>
#include "MathHeader.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketCore
{
	class InputSystem : public Singleton<InputSystem>
	{
		friend Singleton;
	private:
		InputSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

	public:
		void Initialize(HWND hWnd, int screenWidth, int screenHeight);
		void Finalize();

	public:
		void InputUpdate();
		void InputUpdateForEditor();
		void Flush();
		void FlushForEditor();

	private:
		HWND _hWnd;

	public:
		bool GetKeyDown(int keyCode);
		bool GetKeyUp(int keyCode);
		bool GetKey(int keyCode);

		/// Ű�� 0xFF, �� 256������ ���� �� �ְ� �� ���� ������
		/// �׳� ���� ������ ������; ������ ����ȭ �غ���.
	private:
		bool currentKeyState[0xB0];
		bool previousKeyState[0xB0];

	public:
		RocketEngine::Vector2 GetMousePosition() const;
		RocketEngine::Vector2 GetMouseDelta() const;
		RocketEngine::Vector2 MouseDeltaSmooth();
		float GetMouseSensitivity() const;
		void SetMouseSensitivity(float sensitivity);

	private:
		float _mouseSensitivity;
		POINT _mousePoint;
		RocketEngine::Vector2 _currentMousePosition;
		RocketEngine::Vector2 _previousMousePosition;
		RocketEngine::Vector2 _currentMouseDelta;
		RocketEngine::Vector2 _previousMouseDelta;

	private:
		int _screenWidth;
		int _screenHeight;
		int _widthOffset;
		int _heightOffset;
	};
}

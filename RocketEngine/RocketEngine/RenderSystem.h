#pragma once
#include "Singleton.h"
#include <windows.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "..\\RocketGraphicsInterface\\IRocketGraphics.h"
#include "RocketAPI.h"

#define GRAPHICSDLL_PATH (L"RocketDX11.dll")

namespace RocketCore::Graphics
{
	class IRenderable;
	class ISketchable;
	class RenderConstantData;
}

namespace RocketCore
{
	class MeshRendererBase;
	class UIRenderer;
}

namespace RocketEngine
{
	class Light;
}

/// <summary>
/// �������� Graphics ����� �ҷ��� �ʱ�ȭ��Ű��
/// ������ ������Ʈ�� �Ѱ��Ͽ� �׸� �� �ֵ��� �����ϴ� Ŭ����.
/// 
/// 23.06.20 ������ �����.
/// </summary>
namespace RocketCore
{
	class RenderSystem : public Singleton<RenderSystem>
	{
		friend Singleton;
		friend void RocketEngine::RocketDestroyWindow();

	private:
		RenderSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

		/// �ý��� �ʱ�ȭ ����
	public:
		void Initialize(HWND hWnd, int screenWidth, int screenHeight, bool isEditor = false);
		void Finalize();

	private:
		bool _isEditor = false;

		/// ���� �ʱ�ȭ ����
	public:
		void MakeRenderableAll();
		void MakeAnimationAll();
		void MakeUIAll();

	public:
		void OnResize(int width, int height);

		/// ������ ����
	public:
		void DrawProcess();

	private:
		void UpdateRenderData();
		void UpdateConstantData(RocketCore::Graphics::RenderConstantData& data);
		void DrawCurrentScene();

	private:
		void DrawCurrentUI();

	public:
		int GetScreenWidth() const;
		int GetScreenHeight() const;

		/// �⺻ ����(������ �ڵ�, ������ ������ ��)
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;

		/// DLL ����
	private:
		HMODULE hGraphicsModule;
		std::unique_ptr<RocketCore::Graphics::IRocketGraphics> _rocketGraphics;

	public:
		void AddLight(RocketEngine::Light* light);

	private:
		std::unordered_map<MeshRendererBase*, Graphics::IRenderable*> _renderMap;
		std::unordered_map<UIRenderer*, Graphics::ISketchable*> _uiMap;
		std::vector<RocketEngine::Light*> _lights;
	};
}

#pragma once
#include "Singleton.h"
#include <windows.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "../RocketGraphicsInterface/I3DRenderer.h"
#include "../RocketGraphicsInterface/IFactory.h"
#include "../RocketGraphicsInterface/IGraphicsSystem.h"
#include "RocketAPI.h"

#define GRAPHICSDLL_PATH (L"RocketDX11.dll")

namespace Rocket::Core
{
	class IRenderable;
	class ISketchable;
	class RenderConstantData;
}

namespace Rocket::Core
{
	class MeshRendererBase;
	class UIRenderer;
}

namespace Rocket
{
	class Light;
}

/// <summary>
/// �������� Graphics ����� �ҷ��� �ʱ�ȭ��Ű��
/// ������ ������Ʈ�� �Ѱ��Ͽ� �׸� �� �ֵ��� �����ϴ� Ŭ����.
/// 
/// 23.06.20 ������ �����.
/// </summary>
namespace Rocket::Core
{
	class GraphicsSystem : public Singleton<GraphicsSystem>, public IGraphicsSystem
	{
		friend Singleton;
		friend void Rocket::RocketDestroyWindow();

	private:
		GraphicsSystem();		// �̱����̱� ������ �ܺο��� ������ �� ������.

		/// �ý��� �ʱ�ȭ ����
	public:
		void Initialize(HWND hWnd, int screenWidth, int screenHeight, bool isEditor = false);
		void Finalize();

	private:
		bool _isEditor = false;

	public:
		void OnResize(int width, int height);

		/// ������ ����
	public:
		void DrawProcess();

 	private:
 		void SetRenderData();
// 		void UpdateConstantData(Rocket::Core::RenderConstantData& data);
// 		void DrawCurrentScene();
// 
// 	private:
// 		void DrawCurrentUI();

	public:
		int GetScreenWidth() const;
		int GetScreenHeight() const;
		IFactory* GetFactory() const;

		/// �⺻ ����(������ �ڵ�, ������ ������ ��)
	private:
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;

		/// DLL ����
	private:
		HMODULE hGraphicsModule;
		std::unique_ptr<Rocket::Core::I3DRenderer> _rocketGraphics;
		std::unique_ptr<Rocket::Core::IFactory> _factory;
	};
}

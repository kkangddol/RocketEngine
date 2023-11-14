#include "RocketDX12.h"

/// IRocketGraphics.h �� �ִ� dllexport �Լ���.
/// USE_DX12 �� ��ó���⿡ �־��ָ� DX12�� dll�� ���������.
/// 
/// 23.06.16 ������ �����.
namespace RocketCore::Graphics
{
	IRocketGraphics* CreateGraphicsInstance()
	{
		return new RocketDX12();
	}

	void ReleaseGraphicsInstance(IRocketGraphics* instance)
	{
		delete instance;
	}

	void RocketDX12::Initialize(void* hWnd, int screenWidth, int screenHeight, bool isEditor /*= false*/)
	{

	}


	void RocketDX12::OnResize(int _width, int _height)
	{

	}

	void RocketDX12::UpdateCamera(const CameraData& cameraData)
	{

	}

	void RocketDX12::Render()
	{

	}

	void RocketDX12::Finalize()
	{

	}

}


#include "RocketDX12.h"

/// IRocketGraphics.h 에 있는 dllexport 함수들.
/// USE_DX12 를 전처리기에 넣어주면 DX12용 dll이 만들어진다.
/// 
/// 23.06.16 강석원 인재원.
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


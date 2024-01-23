#pragma once
#include <string>

#include "RenderConstantData.h"
#include "CameraData.h"

#include "MathHeader.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG


// Float2, Float3, Float4
// Fmatrix3x3, Fmatrix4x4

namespace RocketCore::Graphics
{
	class IRocketGraphics
	{
	public:
		//그래픽스 엔진을 초기화한다.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight, bool isEditor = false) abstract;

		virtual void UpdateCamera(const CameraData& cameraData) abstract;
		virtual void UpdateConstantData(const RenderConstantData& renderConstData) abstract;

		virtual void Render() abstract;

		virtual void OnResize(int _width, int _height) abstract;

		virtual void Finalize() abstract;
	};

	/// dllexport 하는 렌더러를 반환하는 팩토리 함수.

	// IRocketGraphics를 상속받는 렌더러를 생성해서 업캐스팅하여 반환한다.
	extern "C" __declspec(dllexport) IRocketGraphics * CreateGraphicsInstance();

	// 렌더러를 로드한쪽에서 렌더러를 해제할때 사용하는 함수.
	// 로드하는쪽에서 delete하지 않고 dll한테 요청하는 식으로 구성했다.
	// 각각의 렌더러에 맞는 Release 동작이 정의되어 있을 것이다.
	extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(IRocketGraphics * instance);
}

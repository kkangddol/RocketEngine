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
		//�׷��Ƚ� ������ �ʱ�ȭ�Ѵ�.
		virtual void Initialize(void* hWnd, int screenWidth, int screenHeight, bool isEditor = false) abstract;

		virtual void UpdateCamera(const CameraData& cameraData) abstract;
		virtual void UpdateConstantData(const RenderConstantData& renderConstData) abstract;

		virtual void Render() abstract;

		virtual void OnResize(int _width, int _height) abstract;

		virtual void Finalize() abstract;
	};

	/// dllexport �ϴ� �������� ��ȯ�ϴ� ���丮 �Լ�.

	// IRocketGraphics�� ��ӹ޴� �������� �����ؼ� ��ĳ�����Ͽ� ��ȯ�Ѵ�.
	extern "C" __declspec(dllexport) IRocketGraphics * CreateGraphicsInstance();

	// �������� �ε����ʿ��� �������� �����Ҷ� ����ϴ� �Լ�.
	// �ε��ϴ��ʿ��� delete���� �ʰ� dll���� ��û�ϴ� ������ �����ߴ�.
	// ������ �������� �´� Release ������ ���ǵǾ� ���� ���̴�.
	extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(IRocketGraphics * instance);
}

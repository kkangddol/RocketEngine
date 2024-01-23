#pragma once
#include "MathHeader.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib, "..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketCore::Graphics
{
	struct CameraData
	{
		RocketEngine::Vector3 position;
		RocketEngine::Quaternion rotation;
		//RocketEngine::Matrix matrix;		// Mk.2 �� ī�޶� ȿ��? �ٶ� ����� ����.

		float nearZ;					// frustum�� ����� �������� �Ÿ�
		float farZ;					// frustum�� �� �������� �Ÿ�
		float aspect;					// ���� / ���� ����
		float fovY;					// fov������ 60�й����� ��������
		float nearWindowHeight;		// frustum�� ����� ����� ����
		float farWindowHeight;			// frustum�� �� ����� ����
	};
}

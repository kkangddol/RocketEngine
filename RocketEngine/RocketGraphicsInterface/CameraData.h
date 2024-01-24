#pragma once
#include "MathHeader.h"

namespace RocketCore::Graphics
{
	struct CameraData
	{
		Vector3 position;
		Quaternion rotation;
		//Matrix matrix;		// Mk.2 �� ī�޶� ȿ��? �ٶ� ����� ����.

		float nearZ;					// frustum�� ����� �������� �Ÿ�
		float farZ;					// frustum�� �� �������� �Ÿ�
		float aspect;					// ���� / ���� ����
		float fovY;					// fov������ 60�й����� ��������
		float nearWindowHeight;		// frustum�� ����� ����� ����
		float farWindowHeight;			// frustum�� �� ����� ����
	};
}

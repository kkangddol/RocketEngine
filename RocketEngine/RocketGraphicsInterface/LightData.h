#pragma once
#include "MathHeader.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

namespace RocketCore::Graphics
{
	struct DirectionalLightData
	{
		RocketEngine::Vector4 ambient;
		RocketEngine::Vector4 diffuse;
		RocketEngine::Vector4 specular;
		RocketEngine::Vector3 direction;
		float intensity = 1.0f; 
		//�ڵ����� ����Ʈ �е� ��. Directional Light ���� ���� ��� ������ ���� Intensity ����.
	};

	struct PointLightData
	{
		RocketEngine::Vector4 ambient;
		RocketEngine::Vector4 diffuse;
		RocketEngine::Vector4 specular;

		//GPU�� 4D Vector ��ŷ.
		RocketEngine::Vector3 position;
		float range;

		//GPU�� 4D Vector ��ŷ.
		RocketEngine::Vector3 attenuation;
		float tPad;
	};

	struct SpotLightData
	{
		RocketEngine::Vector4 ambient;
		RocketEngine::Vector4 diffuse;
		RocketEngine::Vector4 specular;

		//GPU�� 4D Vector ��ŷ.
		RocketEngine::Vector3 position;
		float range;

		//GPU�� 4D Vector ��ŷ.
		RocketEngine::Vector3 direction;
		float spot;

		//GPU�� 4D Vector ��ŷ.
		RocketEngine::Vector3 attenuation;
		float tPad;
	};
}

///[TW] : TBA - ���� �� Flax �� Lighting ���� : ���� PBR ���̽�����!
///�׷���, �ϴ� Legacy�� ����� ���ư��� ���� ���� �����ض�.


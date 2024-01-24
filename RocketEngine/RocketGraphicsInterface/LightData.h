#pragma once
#include "MathHeader.h"

namespace RocketCore::Graphics
{
	struct DirectionalLightData
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;
		Vector3 direction;
		float intensity = 1.0f; 
		//�ڵ����� ����Ʈ �е� ��. Directional Light ���� ���� ��� ������ ���� Intensity ����.
	};

	struct PointLightData
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;

		//GPU�� 4D Vector ��ŷ.
		Vector3 position;
		float range;

		//GPU�� 4D Vector ��ŷ.
		Vector3 attenuation;
		float tPad;
	};

	struct SpotLightData
	{
		Vector4 ambient;
		Vector4 diffuse;
		Vector4 specular;

		//GPU�� 4D Vector ��ŷ.
		Vector3 position;
		float range;

		//GPU�� 4D Vector ��ŷ.
		Vector3 direction;
		float spot;

		//GPU�� 4D Vector ��ŷ.
		Vector3 attenuation;
		float tPad;
	};
}

///[TW] : TBA - ���� �� Flax �� Lighting ���� : ���� PBR ���̽�����!
///�׷���, �ϴ� Legacy�� ����� ���ư��� ���� ���� �����ض�.


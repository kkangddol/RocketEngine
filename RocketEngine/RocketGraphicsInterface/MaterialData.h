///[TW] Legacy, PBR Material ��θ� ��� �ڵ��̴�.
///���� Map���� �о���� ���� �󸶳� �ݿ������� ��� �ִ�.

#pragma once
#include "MathHeader.h"

namespace RocketCore::Graphics
{
	struct LegacyMaterialData
	{
		Vector4 _ambient;
		Vector4 _diffuse;
		Vector4 _specular; // w : Specular Power.
		Vector4 _reflect;
	};

	struct PBRMaterialData
	{
		//��� �� 0~1 ������ �־�� �Ѵ�. 
		//�ݼ�/����ü �񱳴� Shader�� �ٸ� ������ ������Ʈ�ؼ� ���� ��.
		Vector4 _albedo;		// RGBA, ���� 0-1 ����. Base Color/Albedo.
		float _metallic;						// �ݼӼ�, 0-1. (����ü�� ��� ���� X)
		float _roughness;					// ��ħ�� ����, 0-1. 
		float _specular;						// ����ŧ��, 0-1. (�ݼ��� ��� ���� X)
		float _emission;						// Material ��ü�� �󸶳� ���� ���°�, 0-1. (�ϴ� �ٸ� ������ �����ÿ��� ���� X)
	
		///PBRMaterialData ��, Specular, Emission�� �ϴ� ������ ��ġ�� ���� ��!
	};
}



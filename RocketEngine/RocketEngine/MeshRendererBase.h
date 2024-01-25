#pragma once
#include <string>

#include "DLLExporter.h"
#include "Component.h"

#include "MathHeader.h"

namespace Rocket
{
	class GameObject;
}

namespace Rocket::Core
{

	/// <summary>
	/// Renderer ������Ʈ���� ��ӹ��� �������̽�.
	/// �ٵ� ��� abstract�ε� Component ���̽�Ŭ������ ��ӹ޾Ƶ� �Ǵ°ǰ�?
	/// 
	/// 23.06.29 ������ �����.
	/// </summary>
	class ROCKET_API MeshRendererBase : public Rocket::Component
	{
	public:
		MeshRendererBase(Rocket::GameObject* owner);

	public:
// 		Graphics::LegacyMaterialData& GetLegacyMaterialData();
// 		Graphics::PBRMaterialData& GetPBRMaterialData();

	public:
		void SetMeshPath(std::string path);
		void SetShaderPath(std::string path);
		void SetDiffusePath(std::string path);
		void SetNormalPath(std::string path);
		void SetARMPath(std::string path);

		

		std::string GetMeshPath() const;
		std::string GetShaderPath() const;
		std::string GetDiffusePath() const;
		std::string GetNormalPath() const;
		std::string GetARMPath() const;

	private:
		// Graphics::ModelData _modelData;

	public:
		void SetLegacyAmbient(Vector4 ambient);
		void SetLegacyDiffuse(Vector4 diffuse);
		void SetLegacySpecular(Vector4 specular);
		void SetLegacyReflect(Vector4 reflect);

		Vector4 GetLegacyAmbient() const;
		Vector4 GetLegacyDiffuse() const;
		Vector4 GetLegacySpecular() const;
		Vector4 GetLegacyReflect() const;

	private:
		//Graphics::LegacyMaterialData _legacyMaterialData;

	public:
		void SetPBRAlbedo(Vector4 albedo);
		void SetPBRMetallic(float metallic);
		void SetPBRRoughness(float roughness);
		void SetPBRSpecular(float specular);
		void SetPBREmission(float emission);

		Vector4 GetPBRAlbedo() const;
		float GetPBRMetallic() const;
		float GetPBRRoughness() const;
		float GetPBRSpecular() const;
		float GetPBREmission() const;

	private:
		//Graphics::PBRMaterialData _PBRMaterialData;
	};
}

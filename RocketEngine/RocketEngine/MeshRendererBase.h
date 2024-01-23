#pragma once
#include <string>

#include "DLLExporter.h"
#include "Component.h"
#include "..\\RocketGraphicsInterface\\MaterialData.h"

#include "MathHeader.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG


namespace RocketEngine
{
	class GameObject;
}

namespace RocketCore
{

	/// <summary>
	/// Renderer 컴포넌트들이 상속받을 인터페이스.
	/// 근데 얘는 abstract인데 Component 베이스클래스를 상속받아도 되는건가?
	/// 
	/// 23.06.29 강석원 인재원.
	/// </summary>
	class ROCKET_API MeshRendererBase : public RocketEngine::Component
	{
	public:
		MeshRendererBase(RocketEngine::GameObject* owner);

	public:
		Graphics::LegacyMaterialData& GetLegacyMaterialData();
		Graphics::PBRMaterialData& GetPBRMaterialData();

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
		void SetLegacyAmbient(RocketEngine::Vector4 ambient);
		void SetLegacyDiffuse(RocketEngine::Vector4 diffuse);
		void SetLegacySpecular(RocketEngine::Vector4 specular);
		void SetLegacyReflect(RocketEngine::Vector4 reflect);

		RocketEngine::Vector4 GetLegacyAmbient() const;
		RocketEngine::Vector4 GetLegacyDiffuse() const;
		RocketEngine::Vector4 GetLegacySpecular() const;
		RocketEngine::Vector4 GetLegacyReflect() const;

	private:
		Graphics::LegacyMaterialData _legacyMaterialData;

	public:
		void SetPBRAlbedo(RocketEngine::Vector4 albedo);
		void SetPBRMetallic(float metallic);
		void SetPBRRoughness(float roughness);
		void SetPBRSpecular(float specular);
		void SetPBREmission(float emission);

		RocketEngine::Vector4 GetPBRAlbedo() const;
		float GetPBRMetallic() const;
		float GetPBRRoughness() const;
		float GetPBRSpecular() const;
		float GetPBREmission() const;

	private:
		Graphics::PBRMaterialData _PBRMaterialData;
	};
}

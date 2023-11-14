#include "MeshRendererBase.h"
#include "EngineProcess.h"
#include "GameObject.h"

namespace RocketCore
{
	MeshRendererBase::MeshRendererBase(RocketEngine::GameObject* owner)
		: Component(owner)
	{

	}

	RocketCore::Graphics::LegacyMaterialData& MeshRendererBase::GetLegacyMaterialData()
	{
		return _legacyMaterialData;
	}

	RocketCore::Graphics::PBRMaterialData& MeshRendererBase::GetPBRMaterialData()
	{
		return _PBRMaterialData;
	}

	void MeshRendererBase::SetMeshPath(std::string path)
	{

	}

	void MeshRendererBase::SetShaderPath(std::string path)
	{

	}

	void MeshRendererBase::SetDiffusePath(std::string path)
	{

	}

	void MeshRendererBase::SetNormalPath(std::string path)
	{

	}

	void MeshRendererBase::SetARMPath(std::string path)
	{

	}

	std::string MeshRendererBase::GetMeshPath() const
	{
		return std::string();
	}

	std::string MeshRendererBase::GetShaderPath() const
	{
		return std::string();
	}

	std::string MeshRendererBase::GetDiffusePath() const
	{
		return std::string();
	}

	std::string MeshRendererBase::GetNormalPath() const
	{
		return std::string();
	}



	std::string MeshRendererBase::GetARMPath() const
	{
		return std::string();
	}

	void MeshRendererBase::SetLegacyAmbient(RocketEngine::RMFLOAT4 ambient)
	{
		_legacyMaterialData._ambient = ambient;
	}

	void MeshRendererBase::SetLegacyDiffuse(RocketEngine::RMFLOAT4 diffuse)
	{
		_legacyMaterialData._diffuse = diffuse;
	}

	void MeshRendererBase::SetLegacySpecular(RocketEngine::RMFLOAT4 specular)
	{
		_legacyMaterialData._specular = specular;
	}

	void MeshRendererBase::SetLegacyReflect(RocketEngine::RMFLOAT4 reflect)
	{
		_legacyMaterialData._reflect = reflect;
	}

	RocketEngine::RMFLOAT4 MeshRendererBase::GetLegacyAmbient() const
	{
		return _legacyMaterialData._ambient;
	}

	RocketEngine::RMFLOAT4 MeshRendererBase::GetLegacyDiffuse() const
	{
		return _legacyMaterialData._diffuse;
	}

	RocketEngine::RMFLOAT4 MeshRendererBase::GetLegacySpecular() const
	{
		return _legacyMaterialData._specular;
	}

	RocketEngine::RMFLOAT4 MeshRendererBase::GetLegacyReflect() const
	{
		return _legacyMaterialData._reflect;
	}

	void MeshRendererBase::SetPBRAlbedo(RocketEngine::RMFLOAT4 albedo)
	{
		_PBRMaterialData._albedo = albedo;
	}

	void MeshRendererBase::SetPBRMetallic(float metallic)
	{
		_PBRMaterialData._metallic = metallic;
	}

	void MeshRendererBase::SetPBRRoughness(float roughness)
	{
		_PBRMaterialData._roughness = roughness;
	}

	void MeshRendererBase::SetPBRSpecular(float specular)
	{
		_PBRMaterialData._specular = specular;
	}

	void MeshRendererBase::SetPBREmission(float emission)
	{
		_PBRMaterialData._emission = emission;
	}

	RocketEngine::RMFLOAT4 MeshRendererBase::GetPBRAlbedo() const
	{
		return _PBRMaterialData._albedo;
	}

	float MeshRendererBase::GetPBRMetallic() const
	{
		return _PBRMaterialData._metallic;
	}

	float MeshRendererBase::GetPBRRoughness() const
	{
		return _PBRMaterialData._roughness;
	}

	float MeshRendererBase::GetPBRSpecular() const
	{
		return _PBRMaterialData._specular;
	}

	float MeshRendererBase::GetPBREmission() const
	{
		return _PBRMaterialData._emission;
	}
}

#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <string>
#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class FBXLoader
	{
	public:
		FBXLoader();
		void Initialize(ID3D11Device* device);

	public:
		void LoadFBXFile(std::string fileName);
	
	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void ProcessStaticMesh(aiMesh* mesh, const aiScene* scene);
		void ProcessSkinnedMesh(aiMesh* mesh, const aiScene* scene);
		void LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
		ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);
		void LoadAnimation(const aiScene* scene);


	private:
		ComPtr<ID3D11Device> _device;
		std::string _nowFileName;
	};
}

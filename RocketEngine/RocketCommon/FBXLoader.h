#pragma once
#include <string>

#include "RawModelStruct.h"

/// 그래픽스에 있는 FBXLoader를 이쪽으로 빼올 예정.
/// FBXLoader랑 그래픽스 엔진이랑 엮여있을 이유가 없다.
/// 여기서 데이터를 만들고 로드한 다음 내가 정의한 .rfbx(json?) 으로 저장하고 그걸 읽어서 활용한다.

namespace Rocket::Core
{
	class FBXLoader
	{
	public:
		FBXLoader();
		~FBXLoader();

	public:
		RawModel* LoadFBXFile(std::string fileName);

	private:
		void ProcessNode(aiNode* ainode, const aiScene* scene);
		RawMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
		RawMesh* ProcessStaticMesh(aiMesh* mesh, const aiScene* scene);
		RawMesh* ProcessSkinnedMesh(aiMesh* mesh, const aiScene* scene);
		void LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene);
		RawTexture* LoadEmbeddedTexture(const aiTexture* embeddedTexture);
		void LoadAnimation(const aiScene* scene);

		RawNode* ReadNodeHierarchy(aiNode* ainode, const aiScene* scene);
		void ReadNodeRecur(RawNode* node, aiNode* ainode, const aiScene* scene, UINT& index);
	
	private:
		RawModel* _resultModel;
		std::unordered_map<aiNode*, RawNode*> _aiNodeToNodeMap;	// Bone에서 aiNode를 통해 Node를 찾기위한 맵
	};
}

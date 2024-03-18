#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "FBXLoader.h"
#include "AssimpMathConverter.h"

const std::string MODEL_PATH = "Resources/Models/";

namespace Rocket::Core
{
	FBXLoader::~FBXLoader()
	{

	}

	RawModel* FBXLoader::LoadFBXFile(std::string fileName)
	{
		std::string fileNameWithExtension;

		/// 경로 제외하기 위한 로직
		size_t slashIndex = fileName.find_last_of("/\\");
		if (slashIndex != std::string::npos)
		{
			fileNameWithExtension = fileName.substr(slashIndex + 1, fileName.length() - slashIndex);
		}
		else
		{
			fileNameWithExtension = fileName;
		}

		std::string path = MODEL_PATH + fileNameWithExtension;

		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded |
			aiProcess_PopulateArmatureData |
			aiProcess_CalcTangentSpace |
			aiProcess_LimitBoneWeights
		);

		if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		{
			MessageBox(NULL, L"Model file couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
			assert(false);
		}

		/// 모델 데이터 로드 (모델,메쉬,노드,본)
		_resultModel = new RawModel();

		// 1. 우선 노드를 쭉 읽으면서 Node의 Hierarchy를 만든다.
		_resultModel->rootNode = ReadNodeHierarchy(scene->mRootNode, scene);

		// 2. 노드를 읽어서 메쉬데이터를 처리한다. 이때 앞서 만든 노드 Hierarchy를 이용해 Bone 데이터도 처리한다.
		ProcessNode(scene->mRootNode, scene);

		// 3. 애니메이션 정보를 처리한다.
		if (scene->HasAnimations())
		{
			LoadAnimation(scene);						// 애니메이션 데이터 로드
		}

		return _resultModel;
	}

	void FBXLoader::ProcessNode(aiNode* ainode, const aiScene* scene)
	{
		for (UINT i = 0; i < ainode->mNumMeshes; ++i)
		{
			RawMesh* mesh = ProcessMesh(scene->mMeshes[ainode->mMeshes[i]], scene);

			// TODO : 여기서 if문으로 분기타지않게 할 것. -> ?
			if (!scene->HasAnimations())
			{
				mesh->node = _aiNodeToNodeMap.at(ainode);
			}
			else
			{
				// 얘는 메쉬랑 본 읽으면서 각각의 버텍스한테 노드를 셋 해줬을 것이다.
			}
			_resultModel->meshes.emplace_back(mesh);
		}

		for (UINT i = 0; i < ainode->mNumChildren; ++i)
		{
			ProcessNode(ainode->mChildren[i], scene);
		}
	}

	RawMesh* FBXLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<RawVertex> vertices;
		std::vector<UINT> indices;

		// Load position, normal, uv
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			RawVertex vertex;

			// process position
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			// process uv
			vertex.UV.x = mesh->mTextureCoords[0][i].x;
			vertex.UV.y = mesh->mTextureCoords[0][i].y;

			// process normal
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			// process tangent
			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

			// 우선 0으로 초기화
			vertex.nodeIndex = 0;
			vertex.boneIndices = DirectX::XMUINT4{ 0, 0, 0, 0 };
			vertex.weights = DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };

			vertices.push_back(vertex);
		}

		// Load indices
		for (UINT i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace& face = mesh->mFaces[i];

			for (UINT j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// 머터리얼 처리
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			for (UINT i = 0; i <= aiTextureType_UNKNOWN; ++i)
			{
				LoadMaterialTextures(material, (aiTextureType)i, scene);
			}
		}

		// 본데이터 처리
		// 각각의 버텍스에 영향을 주는 모든 본에 대해서 저장한 다음
		// 버텍스 기준으로 본인의 position과 normal을 다시 계산한다.
		std::vector<uint32_t> boneIndecesPerVertex;
		boneIndecesPerVertex.resize(vertices.size());

		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* aibone = mesh->mBones[i];

			RawBone* bone = new RawBone();
			bone->name = aibone->mName.C_Str();
			bone->index = _aiNodeToNodeMap.at(aibone->mNode)->index;
			bone->bindedNode = _aiNodeToNodeMap.at(aibone->mNode);
			bone->offsetMatrix = AIMatrix4x4ToXMMatrix(aibone->mOffsetMatrix.Transpose());

			_aiNodeToNodeMap.at(aibone->mNode)->bindedBone = bone;

			for (unsigned int j = 0; j < aibone->mNumWeights; j++)
			{
				int vertexIndex = aibone->mWeights[j].mVertexId;
				float weight = aibone->mWeights[j].mWeight;

				uint32_t boneCount = boneIndecesPerVertex[vertexIndex];

				switch (boneCount)
				{
				case 0:
					vertices[vertexIndex].weights.x = weight;
					vertices[vertexIndex].boneIndices.x = bone->index;
					break;
				case 1:
					vertices[vertexIndex].weights.y = weight;
					vertices[vertexIndex].boneIndices.y = bone->index;
					break;
				case 2:
					vertices[vertexIndex].weights.z = weight;
					vertices[vertexIndex].boneIndices.z = bone->index;
					break;
				case 3:
					vertices[vertexIndex].weights.w = weight;
					vertices[vertexIndex].boneIndices.w = bone->index;
					break;
				default:
					break;
				}
				boneIndecesPerVertex[vertexIndex]++;
				//vertices[vertexIndex].nodeIndex = bone->index;	// 버텍스가 여러 본(노드)에 영향을 받으므로 셰이더에서 그것을 이용해 연산한다.
			}
		}

		RawMesh* resultMesh = new RawMesh();
		resultMesh->vertices = vertices;
		resultMesh->indices = indices;

		return resultMesh;
	}

	void FBXLoader::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene)
	{
		UINT textureCount = material->GetTextureCount(type);
		for (UINT i = 0; i < textureCount; ++i)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			std::string s = std::string(str.C_Str());
			std::string fileName = s.substr(s.find_last_of("/\\") + 1, s.length() - s.find_last_of("/\\"));
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			auto iter = ResourceManager::Instance()._textures.find(fileName);
			if (iter == ResourceManager::Instance()._textures.end())
			{
				const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
				if (embeddedTexture != nullptr)
				{
					// Map에서 내가 만든 클래스말고 그냥 SRV만 관리할까..?
					ID3D11ShaderResourceView* textureSRV = LoadEmbeddedTexture(embeddedTexture);
					Texture* texture = new Texture(nullptr, textureSRV);
					ResourceManager::Instance()._textures.insert(std::make_pair(fileName, texture));
				}
				else
				{
					ResourceManager::Instance().LoadTextureFile(fileName);
				}
			}
		}
	}

	RawTexture* FBXLoader::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
	{
		HRESULT hr;
		ID3D11ShaderResourceView* texture = nullptr;

		if (embeddedTexture->mHeight != 0) {
			// Load an uncompressed ARGB8888 embedded texture
			D3D11_TEXTURE2D_DESC desc;
			desc.Width = embeddedTexture->mWidth;
			desc.Height = embeddedTexture->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA subresourceData;
			subresourceData.pSysMem = embeddedTexture->pcData;
			subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
			subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

			ID3D11Texture2D* texture2D = nullptr;
			hr = _device->CreateTexture2D(&desc, &subresourceData, &texture2D);
			if (FAILED(hr))
				MessageBox(nullptr, L"CreateTexture2D failed!", L"Error!", MB_ICONERROR | MB_OK);

			hr = _device->CreateShaderResourceView(texture2D, nullptr, &texture);
			if (FAILED(hr))
				MessageBox(nullptr, L"CreateShaderResourceView failed!", L"Error!", MB_ICONERROR | MB_OK);

			return texture;
		}

		return texture;
	}

	void FBXLoader::LoadAnimation(const aiScene* scene)
	{
		if (!scene->HasAnimations())
		{
			return;
		}

		// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
		// numChannels == numBones
		UINT animCount = scene->mNumAnimations;
		for (UINT i = 0; i < animCount; ++i)
		{
			auto aiAnim = scene->mAnimations[i];
			RawAnimation* myAnimStruct = new RawAnimation();

			myAnimStruct->name = aiAnim->mName.C_Str();
			myAnimStruct->duration = aiAnim->mDuration;

			// mTicksPerSeciond 이거 그건가? 초당 프레임 수?
			if (aiAnim->mTicksPerSecond != 0.0)
			{
				myAnimStruct->ticksPerSecond = aiAnim->mTicksPerSecond;
			}
			else
			{
				myAnimStruct->ticksPerSecond = 30.0f;
			}

			for (UINT j = 0; j < aiAnim->mNumChannels; ++j)
			{
				const aiNodeAnim* aiNodeAnim = aiAnim->mChannels[j];
				RawNodeAnimationData* myNodeAnim = new RawNodeAnimationData();

				myNodeAnim->nodeName = aiNodeAnim->mNodeName.C_Str();

				for (unsigned int k = 0; k < aiNodeAnim->mNumPositionKeys; ++k)
				{
					myNodeAnim->positionTimestamps.push_back(aiNodeAnim->mPositionKeys[k].mTime);
					myNodeAnim->positions.push_back(AIVec3ToXMFloat3(aiNodeAnim->mPositionKeys[k].mValue));
				}
				for (unsigned int k = 0; k < aiNodeAnim->mNumRotationKeys; ++k)
				{
					myNodeAnim->rotationTimestamps.push_back(aiNodeAnim->mRotationKeys[k].mTime);
					myNodeAnim->rotations.push_back(AIQuaternionToXMFloat4(aiNodeAnim->mRotationKeys[k].mValue));
				}
				for (unsigned int k = 0; k < aiNodeAnim->mNumScalingKeys; ++k)
				{
					myNodeAnim->scaleTimestamps.push_back(aiNodeAnim->mScalingKeys[k].mTime);
					myNodeAnim->scales.push_back(AIVec3ToXMFloat3(aiNodeAnim->mScalingKeys[k].mValue));
				}

				myAnimStruct->nodeAnimations.push_back(myNodeAnim);
			}

			_resultModel->animations.insert({ myAnimStruct->name, myAnimStruct });
		}
	}

	RawNode* FBXLoader::ReadNodeHierarchy(aiNode* ainode, const aiScene* scene)
	{
		UINT index = 0;
		RawNode* rootNode = new RawNode();

		ReadNodeRecur(rootNode, ainode, scene, index);

		return rootNode;
	}

	void FBXLoader::ReadNodeRecur(RawNode* node, aiNode* ainode, const aiScene* scene, UINT& index)
	{
		node->name = ainode->mName.C_Str();
		// Assimp가 Column Major로 Matrix를 읽어오므로 Row Major 하게 Transpose 해준다.
		node->transformMatrix = AIMatrix4x4ToXMMatrix(ainode->mTransformation.Transpose());
		node->index = index;
		index++;

		_resultModel->nodeMap.insert({ node->name, node });
		_aiNodeToNodeMap.insert({ ainode,node });

		for (UINT i = 0; i < ainode->mNumChildren; ++i)
		{
			RawNode* newNode = new RawNode();
			node->children.emplace_back(newNode);
			newNode->parent = node;

			ReadNodeRecur(newNode, ainode->mChildren[i], scene, index);
		}
	}
}

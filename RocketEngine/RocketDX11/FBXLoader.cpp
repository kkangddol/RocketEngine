#include <windows.h>

#include "FBXLoader.h"
#include "MathHeader.h"
#include "AssimpMathConverter.h"
#include "VertexStruct.h"
#include "Mesh.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "ResourceManager.h"
#include "GraphicsMacro.h"
#include "Texture.h"

const std::string MODEL_PATH = "Resources/Models/";

namespace Rocket::Core
{
	FBXLoader::FBXLoader()
		: _device()
	{

	}

	void FBXLoader::Initialize(ID3D11Device* device)
	{
		// TODO : 이거 왜 NULL 에러나지?
		_device = device;
	}

	void FBXLoader::LoadFBXFile(std::string fileName)
	{
		std::string fileNameWithExtension;

		/// 경로 제외하기 위한 로직
		UINT slashIndex = fileName.find_last_of("/\\");
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

		const aiScene* _scene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded |
			aiProcess_PopulateArmatureData |
			aiProcess_CalcTangentSpace);

		if (_scene == nullptr || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || _scene->mRootNode == nullptr)
		{
			MessageBox(NULL, L"Model file couldn't be loaded", L"Error!", MB_ICONERROR | MB_OK);
			assert(false);
		}

		// Node를 Process 하면서 이 ModelData에 저장
		if (_scene->HasAnimations())
		{
			DynamicModel* dynamicModel = new DynamicModel();
			dynamicModel->name = fileNameWithExtension;
			_nowModel = dynamicModel;
		}
		else
		{
			StaticModel* staticModel = new StaticModel();
			staticModel->name = fileNameWithExtension;
			_nowModel = staticModel;
		}

		ProcessModel(_scene->mRootNode, _scene);	

		/// 임시 주석
		//LoadAnimation(_scene);

		// 모든 작업이 끝나면 리소스매니저에 해당 모델 데이터 등록
		ResourceManager::Instance()._models.insert({ fileNameWithExtension, _nowModel });
	}


	void FBXLoader::ProcessModel(aiNode* rootaiNode, const aiScene* scene)
	{
		UINT index = 0;
		Node* rootNode = new Node();
		_nowModel->rootNode = rootNode;
		ProcessNode(rootNode, rootaiNode, scene, index);

		D3D11_BUFFER_DESC nodeBufferDesc;
		nodeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		nodeBufferDesc.ByteWidth = sizeof(NodeBufferType);
		nodeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		nodeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		nodeBufferDesc.MiscFlags = 0;
		nodeBufferDesc.StructureByteStride = 0;

// 		// TODO : reinterpret_cast 사용하지 않도록 수정하기.
// 		if (scene->HasAnimations())
// 		{
// 			auto dModel = reinterpret_cast<DynamicModel*>(_nowModel);
// 
// 			for (auto& mesh : dModel->meshes)
// 			{
// 				mesh->CreateBuffers();		// 모든 노드에 Index가 부여되었으므로 해당 정보를 포함해서 버퍼를 생성.
// 			}
// 		}
// 		else
// 		{
// 			auto sModel = reinterpret_cast<StaticModel*>(_nowModel);
// 
// 			for (auto& mesh : sModel->meshes)
// 			{
// 				mesh->CreateBuffers();		// 모든 노드에 Index가 부여되었으므로 해당 정보를 포함해서 버퍼를 생성.
// 			}
// 		}

		for (auto& mesh : _nowModel->GetMeshes())
		{
			mesh->CreateBuffers();
		}

		HR(_device->CreateBuffer(&nodeBufferDesc, NULL, _nowModel->nodeBuffer.GetAddressOf()));
	}

	void FBXLoader::ProcessNode(Node* node, aiNode* ainode, const aiScene* scene, UINT& index)
	{
		node->name = ainode->mName.C_Str();
		node->index = index;
		index++;

		// TODO : reinterpret_cast 사용하지 않도록 수정하기. modelData는 ResourceManager.h에 있음.
		if (scene->HasAnimations())
		{
			_nowModel->nodeMap.insert(std::make_pair(node->name, node));
		}

		// Assimp가 Column Major로 Matrix를 읽어오므로 Row Major 하게 Transpose 해준다.
		node->transformMatrix = AIMatrix4x4ToXMMatrix(ainode->mTransformation.Transpose());

		for (UINT i = 0; i < ainode->mNumMeshes; ++i)
		{
			Mesh* mesh = ProcessMesh(scene->mMeshes[ainode->mMeshes[i]], scene);
			
			// TODO : 여기서 if문으로 분기타지않게 할 것.
			// TODO : reinterpret_cast 및 dynamic_cast 사용하지 않도록 수정하기.
			if (!scene->HasAnimations())
			{
				mesh->SetNode(node);
				auto staticMesh = dynamic_cast<StaticMesh*>(mesh);
				reinterpret_cast<StaticModel*>(_nowModel)->meshes.emplace_back(staticMesh);
			}
			else
			{
				// 얘는 메쉬랑 본 읽으면서 각각의 버텍스한테 노드를 셋 해줬을것이다.. 아님말고?
				auto skinnedMesh = dynamic_cast<SkinnedMesh*>(mesh);
				reinterpret_cast<DynamicModel*>(_nowModel)->meshes.emplace_back(skinnedMesh);
			}
			//_nowModel->meshes.emplace_back(mesh);
		}

		for (UINT i = 0; i < ainode->mNumChildren; ++i)
		{
			Node* newNode = new Node();
			node->children.emplace_back(newNode);
			newNode->parent = node;

			ProcessNode(newNode, ainode->mChildren[i], scene, index);
		}
	}

	/// 임시 주석
	/*
	void FBXLoader::LoadAnimation(const aiScene* scene)
	{
		// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
		// numChannels == numBones
		UINT animCount = scene->mNumAnimations;
		for (UINT i = 0; i < animCount; ++i)
		{
			const aiAnimation* animation = scene->mAnimations[i];
			Animation* newAnimation = new Animation();
			newAnimation->duration = animation->mDuration;

			if (scene->mAnimations[i]->mTicksPerSecond != 0.0)
			{
				newAnimation->ticksPerSecond = animation->mTicksPerSecond;
			}
			else
			{
				newAnimation->ticksPerSecond = 30.0f;
			}

			for (UINT j = 0; j < animation->mNumChannels; ++j)
			{
				const aiNodeAnim* nodeAnim = animation->mChannels[j];
				NodeAnimation* newNodeAnim = new NodeAnimation();

				newNodeAnim->nodeName = nodeAnim->mNodeName.C_Str();

				for (int k = 0; k < nodeAnim->mNumPositionKeys; ++k)
				{
					newNodeAnim->positionTimestamps.push_back(nodeAnim->mPositionKeys[k].mTime);
					newNodeAnim->positions.push_back(AIVec3ToXMFloat3(nodeAnim->mPositionKeys[k].mValue));
				}
				for (int k = 0; k < nodeAnim->mNumRotationKeys; ++k)
				{
					newNodeAnim->rotationTimestamps.push_back(nodeAnim->mRotationKeys[k].mTime);
					newNodeAnim->rotations.push_back(AIQuaternionToXMFloat4(nodeAnim->mRotationKeys[k].mValue));
				}
				for (int k = 0; k < nodeAnim->mNumScalingKeys; ++k)
				{
					newNodeAnim->scaleTimestamps.push_back(nodeAnim->mScalingKeys[k].mTime);
					newNodeAnim->scales.push_back(AIVec3ToXMFloat3(nodeAnim->mScalingKeys[k].mValue));
				}

				newAnimation->nodeAnimations.push_back(newNodeAnim);
			}
			_loadedFileInfo[_fileName].loadedAnimation.insert(std::make_pair(animation->mName.C_Str(), newAnimation));
		}
	}
	*/

	Mesh* FBXLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		if (scene->HasAnimations())
		{
			
			return ProcessSkinnedMesh(mesh, scene);
		}
		else
		{
			return ProcessStaticMesh(mesh, scene);
		}
	}

	Mesh* FBXLoader::ProcessStaticMesh(aiMesh* mesh, const aiScene* scene)
	{		
		std::vector<Vertex> vertices;
		std::vector<UINT> indices;

		// Load position, normal, uv
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;

			// process position
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			// process normal
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			// process uv
			vertex.UV.x = mesh->mTextureCoords[0][i].x;
			vertex.UV.y = mesh->mTextureCoords[0][i].y;

			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

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

		/// 메타 데이터 활용하는 부분인 것 같은데 우선 주석
		/*

		int upAxis = 0;
		scene->mMetaData->Get<int>("UpAxis", upAxis);
		int upAxisSign = 1;
		scene->mMetaData->Get<int>("UpAxisSign", upAxisSign);
		int frontAxis = 0;
		scene->mMetaData->Get<int>("FrontAxis", frontAxis);
		int frontAxisSign = 1;
		scene->mMetaData->Get<int>("FrontAxisSign", frontAxisSign);
		int coordAxis = 0;
		scene->mMetaData->Get<int>("CoordAxis", coordAxis);
		int coordAxisSign = 1;
		scene->mMetaData->Get<int>("CoordAxisSign", coordAxisSign);

		int originalUpAxis = 0;
		scene->mMetaData->Get<int>("OriginalUpAxis", originalUpAxis);
		int originalUpAxisSign = 1;
		scene->mMetaData->Get<int>("OriginalUpAxisSign", originalUpAxisSign);

		float unitScaleFactor = 1.0f;
		scene->mMetaData->Get<float>("UnitScaleFactor", unitScaleFactor);
		float originalUnitScaleFactor = 1.0f;
		scene->mMetaData->Get<float>("OriginalUnitScaleFactor", originalUnitScaleFactor);

		aiVector3D upVec = upAxis == 0 ? aiVector3D(upAxisSign, 0, 0) : upAxis == 1 ? aiVector3D(0, upAxisSign, 0) : aiVector3D(0, 0, upAxisSign);
		aiVector3D forwardVec = frontAxis == 0 ? aiVector3D(frontAxisSign, 0, 0) : frontAxis == 1 ? aiVector3D(0, frontAxisSign, 0) : aiVector3D(0, 0, frontAxisSign);
		aiVector3D rightVec = coordAxis == 0 ? aiVector3D(coordAxisSign, 0, 0) : coordAxis == 1 ? aiVector3D(0, coordAxisSign, 0) : aiVector3D(0, 0, coordAxisSign);

		upVec *= unitScaleFactor;
		forwardVec *= unitScaleFactor;
		rightVec *= unitScaleFactor;

		// aiMatrix4x4 mat(
		//	rightVec.x, rightVec.y, rightVec.z, 0.0f,
		//	-upVec.x, -upVec.y, -upVec.z, 0.0f,
		//	forwardVec.x, forwardVec.y, forwardVec.z, 0.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f);

		aiMatrix4x4 mat(
			rightVec.x, forwardVec.x, -upVec.x, 0.0f,
			rightVec.y, forwardVec.y, -upVec.y, 0.0f,
			rightVec.z, forwardVec.z, -upVec.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
*/

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			for (UINT i = 0; i <= aiTextureType_UNKNOWN; ++i)
			{
				LoadMaterialTextures(material, (aiTextureType)i, scene);
			}
		}

		StaticMesh* newMesh = new StaticMesh(vertices, indices);

		return newMesh;
	}

	Mesh* FBXLoader::ProcessSkinnedMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<VertexSkinned> vertices;
		std::vector<UINT> indices;

		// Load position, normal, uv
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			VertexSkinned vertex;

			// process position
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;

			// process normal
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			// process uv
			vertex.UV.x = mesh->mTextureCoords[0][i].x;
			vertex.UV.y = mesh->mTextureCoords[0][i].y;

			vertex.boneIndices = DirectX::XMUINT4{ 0, 0, 0, 0 };
			vertex.weights = DirectX::XMFLOAT4{ 0.0f, 0.0f, 0.0f, 0.0f };

			vertices.push_back(vertex);
		}

		// TODO : aiProcess_PopulateArmatureData 이걸 써서 Armature 데이터로도 애니메이션 구성해보자
		//		  ㄴ> 지금은 이 플래그를 사용하지 않고 bone과 node를 따로 읽어서 만들어 보고있다.
		//		  ㄴ> mesh->mBones[0]->mArmature->mMeshes 뭐 이런식으로? 바로 노드에 접근하는거같은데..


		// 각각의 버텍스에 영향을 주는 모든 본에 대해서 저장한 다음
		// 버텍스 기준으로 본인의 position과 normal을 다시 계산한다.
		std::unordered_map<int, std::vector<std::pair<float, DirectX::XMMATRIX>>> weightDataPerVertex;	// 버텍스 인덱스, 가중치 데이터(가중치,트랜스폼)

		for (int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* bone = mesh->mBones[i];

			for (int j = 0; j < bone->mNumWeights; j++)
			{
				int vertexIndex = bone->mWeights[j].mVertexId;
				float weight = bone->mWeights[j].mWeight;
				DirectX::XMMATRIX offsetTM = AIMatrix4x4ToXMMatrix(bone->mOffsetMatrix);
				offsetTM = DirectX::XMMatrixTranspose(offsetTM);

				weightDataPerVertex[vertexIndex].push_back({ weight,offsetTM });
			}
		}

		// 지금은 읽은 Vertex 데이터에 직접 Bone의 가중치를 곱해서 Vertex 정보를 갱신해주고있다.
		// 어차피 Bone의 가중치는 바뀌지 않을거니까..?
		for (auto& iter : weightDataPerVertex)
		{
			Vector3 resultPosition = Vector3::Zero;
			Vector3 resultNormal = Vector3::Zero;

			for (auto& weightData : iter.second)
			{
				DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(weightData.second);
				DirectX::XMMATRIX offsetInverse = DirectX::XMMatrixInverse(&det, weightData.second);

				resultPosition += Vector3::Transform(vertices[iter.first].position, weightData.second) * weightData.first;
				resultNormal += Vector3::Transform(vertices[iter.first].normal, DirectX::XMMatrixTranspose(offsetInverse)) * weightData.first;
			}

			vertices[iter.first].position = resultPosition;
			vertices[iter.first].normal = resultNormal;
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

		// UINT metaDataCount = scene->mMetaData->mNumProperties;
		// for (UINT i = 0; i < metaDataCount; ++i)
		// {
		//	const aiString* key;
		//	const aiMetadataEntry* entry;
		//	auto res = scene->mMetaData->Get(i, key, entry);
		//	auto type = entry->mType;
		// }

// 		int upAxis = 0;
// 		scene->mMetaData->Get<int>("UpAxis", upAxis);
// 		int upAxisSign = 1;
// 		scene->mMetaData->Get<int>("UpAxisSign", upAxisSign);
// 		int frontAxis = 0;
// 		scene->mMetaData->Get<int>("FrontAxis", frontAxis);
// 		int frontAxisSign = 1;
// 		scene->mMetaData->Get<int>("FrontAxisSign", frontAxisSign);
// 		int coordAxis = 0;
// 		scene->mMetaData->Get<int>("CoordAxis", coordAxis);
// 		int coordAxisSign = 1;
// 		scene->mMetaData->Get<int>("CoordAxisSign", coordAxisSign);
// 
// 		int originalUpAxis = 0;
// 		scene->mMetaData->Get<int>("OriginalUpAxis", originalUpAxis);
// 		int originalUpAxisSign = 1;
// 		scene->mMetaData->Get<int>("OriginalUpAxisSign", originalUpAxisSign);
// 
// 		float unitScaleFactor = 1.0f;
// 		scene->mMetaData->Get<float>("UnitScaleFactor", unitScaleFactor);
// 		float originalUnitScaleFactor = 1.0f;
// 		scene->mMetaData->Get<float>("OriginalUnitScaleFactor", originalUnitScaleFactor);
// 
// 		aiVector3D upVec = upAxis == 0 ? aiVector3D(upAxisSign, 0, 0) : upAxis == 1 ? aiVector3D(0, upAxisSign, 0) : aiVector3D(0, 0, upAxisSign);
// 		aiVector3D forwardVec = frontAxis == 0 ? aiVector3D(frontAxisSign, 0, 0) : frontAxis == 1 ? aiVector3D(0, frontAxisSign, 0) : aiVector3D(0, 0, frontAxisSign);
// 		aiVector3D rightVec = coordAxis == 0 ? aiVector3D(coordAxisSign, 0, 0) : coordAxis == 1 ? aiVector3D(0, coordAxisSign, 0) : aiVector3D(0, 0, coordAxisSign);
// 
// 		upVec *= unitScaleFactor;
// 		forwardVec *= unitScaleFactor;
// 		rightVec *= unitScaleFactor;
// 
// 		aiMatrix4x4 mat(
// 			rightVec.x, rightVec.y, rightVec.z, 0.0f,
// 			forwardVec.x, forwardVec.y, forwardVec.z, 0.0f,
// 			-upVec.x, -upVec.y, -upVec.z, 0.0f,
// 			0.0f, 0.0f, 0.0f, 1.0f);

		// aiMatrix4x4 mat(
		//	rightVec.x, forwardVec.x, -upVec.x, 0.0f,
		//	rightVec.y, forwardVec.y, -upVec.y, 0.0f,
		//	rightVec.z, forwardVec.z, -upVec.z, 0.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f);

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			for (UINT i = 0; i <= aiTextureType_UNKNOWN; ++i)
			{
				LoadMaterialTextures(material, (aiTextureType)i, scene);
			}
		}

		SkinnedMesh* newMesh = new SkinnedMesh(vertices, indices);

		return newMesh;
	}

	/// 임시 주석
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

	ID3D11ShaderResourceView* FBXLoader::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
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
	}
}

#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "IResource.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class Mesh;
	class StaticMesh;
	class SkinnedMesh;

	struct Model;
	struct StaticModel;
	struct DynamicModel;
	struct Node;
	struct Bone;
	struct NodeAnimation;
	struct Animation;

	struct Model : public IResource
	{
		// TODO : 여기에 가상 함수가 있는게 맞나?
		virtual std::vector<Mesh*> GetMeshes();

		std::string name;
		Node* rootNode;
		std::unordered_map<std::string, Node*> nodeMap;
		ComPtr<ID3D11Buffer> nodeBuffer;
	};

	struct StaticModel : public Model
	{
		virtual std::vector<Mesh*> GetMeshes() override;

		std::vector<StaticMesh*> meshes;
	};

	struct DynamicModel : public Model
	{
		virtual std::vector<Mesh*> GetMeshes() override;

		std::vector<SkinnedMesh*> meshes;
	};

	// structure containing bone information
	struct Bone
	{
		std::string name = "";
		int index = -1;	// position of the bone in final upload array
		Node* bindedNode = nullptr;
		DirectX::XMMATRIX offsetMatrix = DirectX::XMMatrixIdentity();
	};

	// structure to hold node hierarchy
	struct Node
	{
		std::string name = "";
		int index = -1;
		Bone* bindedBone = nullptr;
		DirectX::XMMATRIX transformMatrix = DirectX::XMMatrixIdentity();
		Node* parent = nullptr;
		std::vector<Node*> children = {};

		DirectX::XMMATRIX GetWorldMatrix()
		{
			if (parent)
			{
				return transformMatrix * parent->GetWorldMatrix();
			}
			else
			{
				return transformMatrix;
			}
		}
	};

	// structure containing each node's animation information in one animation
	struct NodeAnimation
	{
		std::string nodeName;

		std::vector<float> positionTimestamps = {};
		std::vector<float> rotationTimestamps = {};
		std::vector<float> scaleTimestamps = {};

		std::vector<DirectX::XMFLOAT3> positions = {};
		std::vector<DirectX::XMFLOAT4> rotations = {};
		std::vector<DirectX::XMFLOAT3> scales = {};
	};

	// structure containing animation information
	struct Animation
	{
		float duration = 0.0f;
		float ticksPerSecond = 1.0f;
		float accumulatedTime = 0.0f;
		bool isLoop = true;
		bool isEnd = false;		// flag for not loop animations
		std::vector<NodeAnimation*> nodeAnimations;
	};
}

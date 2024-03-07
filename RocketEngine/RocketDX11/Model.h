#pragma once
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "IResource.h"
#include "Animation.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class Model : public IResource
	{
	public:
		Model();
		~Model();

		std::string GetName() const { return _name; }
		void SetName(std::string val) { _name = val; }

		Rocket::Core::Node* GetRootNode() const { return _rootNode; }
		void SetRootNode(Node* val) { _rootNode = val; }

		std::unordered_map<std::string, Node*>& GetNodeMap() { return _nodeMap; }

		ID3D11Buffer* GetNodeBuffer() const { return _nodeBuffer.Get(); }
		ID3D11Buffer** GetAddressOfNodeBuffer() { return _nodeBuffer.GetAddressOf(); }

	protected:
		std::string _name;
		Node* _rootNode;
		std::unordered_map<std::string, Node*> _nodeMap;
		ComPtr<ID3D11Buffer> _nodeBuffer;
	};
}


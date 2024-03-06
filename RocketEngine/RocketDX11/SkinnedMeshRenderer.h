#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#include "..\\GraphicsInterface\\ISkinnedMeshRenderer.h"
#include "../GraphicsInterface/GraphicsEnum.h"
#include "SkinnedMesh.h"
#include "Material.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	struct ModelData;
	struct SkinnedModelData;

	class SkinnedMeshRenderer : public Rocket::Core::ISkinnedMeshRenderer
	{
	public:
		SkinnedMeshRenderer();

	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void LoadMesh(const std::string& fileName) override;
		virtual void LoadTexture(std::string fileName) override;

	public:
		void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);

	public:
		void LoadMesh(SkinnedMesh* mesh) { _mesh = mesh; }
		void SetMaterial(Material* val) { _material = val; }
		void SetTexture(Texture* texture);
		void SetVertexShader(VertexShader* shader);
		void SetPixelShader(PixelShader* shader);
		void SetRenderState(ID3D11RasterizerState* renderState);

	private:
		void SetNodeBuffer(Node* node, UINT& index, NodeBufferType* nodeBuffer);

	private:
		// TODO : 이거 상속구조 잘 만들던가.. 어떻게든 해서 ModelData와 SkinnedModelData 잘 나눠보자..ㅠㅠ
		ModelData* _model;
		SkinnedMesh* _mesh;
		std::vector<SkinnedMesh*>* _meshes;
		Material* _material;
		DirectX::XMMATRIX _worldTM;
		bool _isActive;
	};
}

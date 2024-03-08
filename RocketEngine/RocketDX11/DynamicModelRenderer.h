#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#include "..\\GraphicsInterface\\IDynamicModelRenderer.h"
#include "../GraphicsInterface/GraphicsEnum.h"
#include "ModelStruct.h"
#include "Material.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class DynamicModelRenderer : public Rocket::Core::IDynamicModelRenderer
	{
	public:
		DynamicModelRenderer();

	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void LoadModel(const std::string& fileName) override;
		virtual void LoadTexture(std::string fileName) override;

	public:
		void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);

	public:
		void SetMaterial(Material* val) { _material = val; }
		void SetVertexShader(VertexShader* shader);
		void SetPixelShader(PixelShader* shader);
		void SetRenderState(ID3D11RasterizerState* renderState);

	private:
		void SetNodeBuffer(Node* node, NodeBufferType* nodeBuffer);
		void SetBoneBuffer(Node* node, BoneBufferType* boneBuffer);

	private:
		// TODO : 이거 상속구조 잘 만들던가.. 어떻게든 해서 ModelData와 SkinnedModelData 잘 나눠보자..ㅠㅠ
		DynamicModel* _model;
		Material* _material;
		DirectX::XMMATRIX _worldTM;
		bool _isActive;
	};
}

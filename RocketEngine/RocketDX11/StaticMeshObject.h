#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "Mesh.h"
#include "Model.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "..\\HODO3DGraphicsInterface\\IStaticMesh.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class StaticMeshObject : public Rocket::Core::IStaticMesh
	{
	public:
		StaticMeshObject();

	public:
		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void LoadMesh(const std::string& fileName) override;
		virtual void LoadNormalMap(const std::string& fileName) override;
		virtual void LoadDiffuseMap(const std::string& fileName) override;
		virtual void LoadVertexShader(const std::string& fileName) override;
		virtual void LoadPixelShader(const std::string& fileName) override;

	public:
		void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);

	public:
		void SetModel(Model* model);
		void SetVertexShader(VertexShader* shader);
		void SetPixelShader(PixelShader* shader);
		void SetRenderState(ID3D11RasterizerState* renderState) { _renderState = renderState; }

	private:
		Model* _model;
		VertexShader* _vertexShader;
		PixelShader* _pixelShader;
		ID3D11RasterizerState* _renderState;
		DirectX::XMMATRIX _worldTM;
	};
}
	
#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "..\\RocketGraphicsInterface\\IMeshRenderer.h"
#include "Mesh.h"
#include "Material.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class MeshRenderer : public Rocket::Core::IMeshRenderer
	{
	public:
		MeshRenderer();

	public:
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;

	public:
		void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);

	public:
		void SetMesh(Mesh* mesh) { _mesh = mesh; }
		void SetMaterial(Material* val) { _material = val; }
		void SetTexture(Texture* texture);
		void SetVertexShader(VertexShader* shader);
		void SetPixelShader(PixelShader* shader);
		void SetRenderState(ID3D11RasterizerState* renderState);

	private:
		Mesh* _mesh;
		Material* _material;
		DirectX::XMMATRIX _worldTM;
		bool _isActive;
	};
}
	
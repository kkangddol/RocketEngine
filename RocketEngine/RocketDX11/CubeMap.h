#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>
#include <string>

#include "IResource.h"
#include "MathHeader.h"
#include "Texture.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class CubeMap : public IResource
	{
	public:
		CubeMap();
		~CubeMap();

	public:
		void Initialize(ID3D11Device* device);
		void Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj);
		void LoadTexture(const std::string& fileName);
		void SetTexture(ID3D11Resource* texture, ID3D11ShaderResourceView* textureView);

	private:
		void BuildGeometryBuffers(ID3D11Device* device);

	private:
		Texture* _texture;
		ComPtr<ID3D11Buffer> _vertexBuffer;
		ComPtr<ID3D11Buffer> _indexBuffer;
		ComPtr<ID3D11RasterizerState> _cubeMapRenderState;
		int _vertexCount;
		int _indexCount;
	};
}

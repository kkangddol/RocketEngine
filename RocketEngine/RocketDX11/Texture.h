#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>

#include "IResource.h"

using Microsoft::WRL::ComPtr;

namespace Rocket::Core
{
	class Texture : public IResource
	{
	public:
		Texture();
		Texture(const Texture& other);
		~Texture();

	public:
		bool Initialize(ID3D11Device* device, const std::wstring& path);
		void Shutdown();
		ID3D11ShaderResourceView* GetTexture();
		ID3D11ShaderResourceView** GetAddressOfTextureView();

	private:
		ID3D11Resource* _texture;
		ID3D11ShaderResourceView* _textureView;
	};
}

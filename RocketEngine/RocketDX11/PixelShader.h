#pragma once
#include <wrl.h>
#include "ShaderBase.h"

namespace Rocket::Core
{
	class PixelShader : public ShaderBase
	{
	public:
		PixelShader();
		~PixelShader();

	public:
		virtual void Initialize(ID3D11Device* device, const std::wstring& path) override;

	public:
		ID3D11PixelShader* GetPixelShader() const;
		ID3D11Buffer* GetLightBuffer() const;
		ID3D11Buffer** GetAddressOfLightBuffer();

	private:
		void CreateAndReflectShader(ID3D11Device* device, const std::wstring& path);
		void CreateLightBuffer(ID3D11Device* device);			// Shader Reflection 이후 안씀.


	private:
		ComPtr<ID3D11PixelShader> _pixelShader;
		ComPtr<ID3D11Buffer> _lightBuffer;
	};
}

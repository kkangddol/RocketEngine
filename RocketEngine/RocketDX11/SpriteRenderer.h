#pragma once
#include <d3d11.h>
#include <memory>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include <wrl\client.h>

#include "..\\RocketGraphicsInterface\\ISpriteRenderer.h"

namespace Rocket::Core
{
	class SpriteRenderer : public Rocket::Core::ISpriteRenderer
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		virtual void SetImage(const std::string& filePath) override;
		virtual void SetWorldTM(const Matrix& worldTM) override;
		virtual void SetActive(bool isActive) override;
		virtual void SetColor(Color color) override;

		void InitalizeImageRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	public:
		void Render(DirectX::SpriteBatch* spriteBatch);

	private:
		Color _color;
		Matrix _worldTM;

		// 이미지 정보
		float _imageWidth;
		float _imageHeight;

		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _imagerSRV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _textureSRV;
	};
}

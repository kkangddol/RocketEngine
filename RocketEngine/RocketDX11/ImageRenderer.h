#pragma once

#include <d3d11.h>
#include <memory>
//#include <vector>
#include <DXTK\\SpriteBatch.h>
#include <DXTK\\WICTextureLoader.h>
#include <DirectXTex\\DirectXTex.h>
#include <wrl\client.h>

#include "..\\HODO3DGraphicsInterface\\ISketchableImage.h"
#include "..\\HODOmath\\HODOmath.h"



namespace Rocket::Core
{
	class ImageRenderer : public Rocket::Core::ISketchableImage
	{
	public:
		ImageRenderer();
		~ImageRenderer();

		virtual void SetImage(const std::string& filePath) override;

		virtual void SetScreenSpacePosition(float x, float y) override;

		virtual float GetScreenSpacePositionX() override;
		virtual float GetScreenSpacePositionY() override;

		virtual void SetScreenSpace() override;

		virtual void SetWorldSpace() override;

		virtual void SetWorldTM(const HDMath::HDFLOAT4X4& worldTM) override;

		virtual void SetActive(bool isActive) override;

		virtual void ChangeScale(float x, float y) override;

		virtual float GetWidth() override;

		virtual float GetHeight() override;

		void InitalizeImageRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);


	public:
		void Render(DirectX::SpriteBatch* spriteBatch);

	private:
		// 내부 변수들
		std::string _str;
		DirectX::XMVECTOR _color;
		float _xlocation;
		float _ylocation;
		float _scaleX;
		float _scaleY;

		// 이미지 정보
		DirectX::TexMetadata _meataData;
		float _imageWidth;
		float _imageHeight;

		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _imagerSRV;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _textureSRV;

		bool _isTranslated;
	};
}
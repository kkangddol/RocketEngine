#include <locale>
#include <codecvt>
//#include <SimpleMath.h>

#include "SpriteRenderer.h"
#include "ResourceManager.h"

#define FILEPATH "Resources/"

namespace Rocket::Core
{
	SpriteRenderer::SpriteRenderer()
		: _device(),
		_deviceContext(),
		_imageWidth(),
		_imageHeight()
	{
		_color = DirectX::Colors::White;
	}

	SpriteRenderer::~SpriteRenderer()
	{

	}

	void SpriteRenderer::SetImage(const std::string& filePath)
	{
		// TODO : 이거 리소스매니저로 빼야지.. 하..

		std::string basePath = FILEPATH;

		std::string fullPath = basePath + filePath;

		std::wstring wideFilePath;

		// std::wstring_convert 대신 MultiByteToWideChar() 사용
		int requiredSize = MultiByteToWideChar(
			CP_UTF8,             // 문자열의 인코딩 방식 (UTF-8)
			0,                   // 플래그 (여기서는 0 사용)
			fullPath.c_str(),    // 변환할 문자열
			-1,                  // 변환할 문자열의 길이 (-1이면 널 종료 문자까지 변환)
			nullptr,             // 출력 버퍼 (크기 계산을 위해 nullptr 전달)
			0                    // 출력 버퍼의 크기 (0이면 크기 계산만 수행)
		);

		if (requiredSize > 0)
		{
			wideFilePath.resize(requiredSize);
			MultiByteToWideChar(
				CP_UTF8,
				0,
				fullPath.c_str(),
				-1,
				&wideFilePath[0],
				requiredSize
			);
		}

		DirectX::CreateWICTextureFromFile(
			_device,
			_deviceContext,
			wideFilePath.c_str(),
			nullptr,
			_imagerSRV.GetAddressOf());

		D3D11_TEXTURE2D_DESC textureDesc;
		ID3D11Resource* resource;
		_imagerSRV->GetResource(&resource);
		ID3D11Texture2D* texture2D = static_cast<ID3D11Texture2D*>(resource);
		texture2D->GetDesc(&textureDesc);

		_imageWidth = static_cast<float>(textureDesc.Width);
		_imageHeight = static_cast<float>(textureDesc.Height);

	}

	void SpriteRenderer::Render(DirectX::SpriteBatch* spriteBatch)
	{
		Vector3 scale;
		Quaternion quat;
		Vector3 pos;

		_worldTM.Decompose(scale, quat, pos);

		spriteBatch->Begin();

		spriteBatch->Draw(
			_imagerSRV.Get(),
			DirectX::XMFLOAT2(pos.x, pos.y),
			nullptr,
			_color,
			0.0f,										//회전 각도
			DirectX::XMFLOAT2(0.5f, 0.5f),				//  이미지의 원점->0.0f,0.0f이면 좌측상단
			DirectX::XMFLOAT2(scale.x, scale.y));		// 이미지 스케일

		spriteBatch->End();
	}

	void SpriteRenderer::SetColor(Color color)
	{
		_color = color;
	}

	void SpriteRenderer::SetWorldTM(const Matrix& worldTM)
	{
		_worldTM = worldTM;
	}


	void SpriteRenderer::SetActive(bool isActive)
	{
		isActive = 1;
	}

	void SpriteRenderer::InitalizeImageRenderer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		_device = device;
		_deviceContext = deviceContext;
	}
}

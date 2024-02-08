#include "MeshRenderer.h"
#include "GraphicsMacro.h"
#include "GraphicsStruct.h"
#include "ResourceManager.h"
#include "CubeMesh.h"
#include "SphereMesh.h"


namespace Rocket::Core
{
	MeshRenderer::MeshRenderer()
		: _mesh(nullptr),
		_material(nullptr),
		_isActive(true),
		_worldTM(Matrix::Identity)
	{

	}

	void MeshRenderer::SetWorldTM(const Matrix& worldTM)
	{
		_worldTM = worldTM;
	}

	void MeshRenderer::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void MeshRenderer::SetMesh(eMeshType meshType)
	{
		_mesh = ResourceManager::Instance().GetMesh(meshType);
	}

	void MeshRenderer::SetMesh(std::string fileName)
	{
		_meshes = &(ResourceManager::Instance().GetMeshes(fileName));
	}

	void MeshRenderer::Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
	{
		if (!_isActive)
		{
			return;
		}

		// Grid가 쓰는 Shader deviceContext 이용해 연결.
		deviceContext->VSSetShader(_material->GetVertexShader()->GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(_material->GetPixelShader()->GetPixelShader(), nullptr, 0);

		deviceContext->PSSetSamplers(0, 1, _material->GetVertexShader()->GetAddressOfSampleState());

		// 입력 배치 객체 셋팅
		deviceContext->IASetInputLayout(_material->GetVertexShader()->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 상수 버퍼 세팅
		{
			// 버텍스 쉐이더
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(deviceContext->Map(_material->GetVertexShader()->GetMatrixBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			MatrixBufferType* matrixBufferDataPtr = (MatrixBufferType*)mappedResource.pData;

			DirectX::XMMATRIX w = DirectX::XMMatrixTranspose(_worldTM);
			DirectX::XMMATRIX v = DirectX::XMMatrixTranspose(view);
			DirectX::XMMATRIX p = DirectX::XMMatrixTranspose(proj);

			matrixBufferDataPtr->world = w;
			matrixBufferDataPtr->view = v;
			matrixBufferDataPtr->projection = p;

			deviceContext->Unmap(_material->GetVertexShader()->GetMatrixBuffer(), 0);

			unsigned int bufferNumber = 0;

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _material->GetVertexShader()->GetAddressOfMatrixBuffer());

			// 픽셀 쉐이더
			HR(deviceContext->Map(_material->GetPixelShader()->GetLightBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		
			LightBufferType* lightBufferDataPtr = (LightBufferType*)mappedResource.pData;

			lightBufferDataPtr->ambientColor = { 0.15f,0.15f,0.15f,1.0f };
			lightBufferDataPtr->diffuseColor = { 1.0f,1.0f,1.0f,1.0f };
			lightBufferDataPtr->lightDirection = { 1.0f,-1.0f,1.0f };
			lightBufferDataPtr->specularPower = 2.0f;
			lightBufferDataPtr->specularColor = { 0.0f,0.0f,1.0f,1.0f };

			deviceContext->Unmap(_material->GetPixelShader()->GetLightBuffer(), 0);

			bufferNumber = 0;

			deviceContext->PSSetConstantBuffers(bufferNumber, 1, _material->GetPixelShader()->GetAddressOfLightBuffer());
		}

		// 렌더스테이트
		deviceContext->RSSetState(_material->GetRenderState());



		

		/// 그린다

		// 인덱스버퍼와 버텍스버퍼 셋팅
		UINT stride = 0;
		UINT offset = 0;

		/*
		switch (_mesh->GetVertexType())
		{
			case VertexType::COLOR_VERTEX:
				stride = sizeof(ColorVertex);
				break;
			case VertexType::TEXTURE_VERTEX:
				stride = sizeof(TextureVertex);
				break;
			case VertexType::LIGHT_VERTEX:
				stride = sizeof(LightVertex);
				break;
			case VertexType::VERTEX:
				stride = sizeof(Vertex);
				break;
			default:
				break;
		}

		deviceContext->IASetVertexBuffers(0, 1, _mesh->GetAddressOfVertexBuffer(), &stride, &offset);
		deviceContext->IASetIndexBuffer(_mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

		deviceContext->PSSetShaderResources(0, 1, _material->GetTexture()->GetAddressOfTextureView());

		deviceContext->DrawIndexed(_mesh->GetIndexCount(), 0, 0);
		*/

		if(_meshes == nullptr)
		{
			return;
		}

		for (auto& mesh : *_meshes)
		{
			switch (mesh->GetVertexType())
			{
				case VertexType::COLOR_VERTEX:
					stride = sizeof(ColorVertex);
					break;
				case VertexType::TEXTURE_VERTEX:
					stride = sizeof(TextureVertex);
					break;
				case VertexType::LIGHT_VERTEX:
					stride = sizeof(LightVertex);
					break;
				case VertexType::VERTEX:
					stride = sizeof(Vertex);
					break;
				default:
					break;
			}

			deviceContext->IASetVertexBuffers(0, 1, mesh->GetAddressOfVertexBuffer(), &stride, &offset);
			deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			deviceContext->PSSetShaderResources(0, 1, _material->GetTexture()->GetAddressOfTextureView());

			deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}
	}

	void MeshRenderer::SetTexture(Texture* texture)
	{
		assert(_material);
		_material->SetTexture(texture);
	}

	void MeshRenderer::SetVertexShader(VertexShader* shader)
	{
		assert(_material);
		_material->SetVertexShader(shader);
	}

	void MeshRenderer::SetPixelShader(PixelShader* shader)
	{
		assert(_material);
		_material->SetPixelShader(shader);
	}

	void MeshRenderer::SetRenderState(ID3D11RasterizerState* renderState)
	{
		assert(_material);
		_material->SetRenderState(renderState);
	}
}

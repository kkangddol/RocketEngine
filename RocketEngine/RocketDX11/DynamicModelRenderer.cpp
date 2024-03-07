#include "DynamicModelRenderer.h"
#include "GraphicsMacro.h"
#include "GraphicsStruct.h"
#include "ResourceManager.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "Animation.h"

namespace Rocket::Core
{
	DynamicModelRenderer::DynamicModelRenderer()
		: _mesh(nullptr),
		_material(nullptr),
		_isActive(true),
		_worldTM(Matrix::Identity)
	{

	}

	void DynamicModelRenderer::SetWorldTM(const Matrix& worldTM)
	{
		_worldTM = worldTM;
	}

	void DynamicModelRenderer::SetActive(bool isActive)
	{
		_isActive = isActive;
	}

	void DynamicModelRenderer::LoadMesh(const std::string& fileName)
	{
		_model = ResourceManager::Instance().GetModel(fileName);
	}

	void DynamicModelRenderer::LoadTexture(std::string fileName)
	{
		_material->SetTexture(ResourceManager::Instance().GetTexture(fileName));
	}

	void DynamicModelRenderer::Render(ID3D11DeviceContext* deviceContext, const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
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

			// DX에서 HLSL 로 넘어갈때 자동으로 전치가 되서 넘어간다.
			// HLSL 에서도 Row Major 하게 작성하고 싶으므로 미리 전치를 시켜놓는다.
			// 총 전치가 2번되므로 HLSL에서도 Row Major한 Matrix로 사용한다.

			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(_worldTM);
			DirectX::XMMATRIX worldInverse = DirectX::XMMatrixInverse(&det, _worldTM);

			DirectX::XMMATRIX w = DirectX::XMMatrixTranspose(_worldTM);
			DirectX::XMMATRIX wi = DirectX::XMMatrixTranspose(worldInverse);
			DirectX::XMMATRIX v = DirectX::XMMatrixTranspose(view);
			DirectX::XMMATRIX p = DirectX::XMMatrixTranspose(proj);

			matrixBufferDataPtr->world = w;
			matrixBufferDataPtr->worldInverse = wi;
			matrixBufferDataPtr->view = v;
			matrixBufferDataPtr->projection = p;

			deviceContext->Unmap(_material->GetVertexShader()->GetMatrixBuffer(), 0);

			unsigned int bufferNumber = 0;

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, _material->GetVertexShader()->GetAddressOfMatrixBuffer());

			///
			HR(deviceContext->Map(_model->nodeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			NodeBufferType* nodeBufferDataPtr = (NodeBufferType*)mappedResource.pData;

			UINT index = 0;

			SetNodeBuffer(_model->rootNode, index, nodeBufferDataPtr);

			deviceContext->Unmap(_model->nodeBuffer, 0);

			bufferNumber = 2;

			deviceContext->VSSetConstantBuffers(bufferNumber, 1, &(_model->nodeBuffer));
			///

			// 픽셀 쉐이더
			HR(deviceContext->Map(_material->GetPixelShader()->GetLightBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

			LightBufferType* lightBufferDataPtr = (LightBufferType*)mappedResource.pData;

			lightBufferDataPtr->ambientColor = { 0.15f,0.15f,0.15f,1.0f };
			lightBufferDataPtr->diffuseColor = { 1.0f,1.0f,1.0f,1.0f };
			lightBufferDataPtr->lightDirection = { 1.0f,0.0f,0.0f };
			lightBufferDataPtr->specularPower = 2.0f;
			lightBufferDataPtr->specularColor = { 1.0f,1.0f,1.0f,1.0f };

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

		// 		if(_meshes == nullptr)
		// 		{
		// 			return;
		// 		}

		stride = sizeof(VertexSkinned);
		
		for (auto& mesh : _model->meshes)
		{
			deviceContext->IASetVertexBuffers(0, 1, mesh->GetAddressOfVertexBuffer(), &stride, &offset);
			deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

			deviceContext->PSSetShaderResources(0, 1, _material->GetTexture()->GetAddressOfTextureView());

			deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		}
	}

	void DynamicModelRenderer::SetTexture(Texture* texture)
	{
		assert(_material);
		_material->SetTexture(texture);
	}

	void DynamicModelRenderer::SetVertexShader(VertexShader* shader)
	{
		assert(_material);
		_material->SetVertexShader(shader);
	}

	void DynamicModelRenderer::SetPixelShader(PixelShader* shader)
	{
		assert(_material);
		_material->SetPixelShader(shader);
	}

	void DynamicModelRenderer::SetRenderState(ID3D11RasterizerState* renderState)
	{
		assert(_material);
		_material->SetRenderState(renderState);
	}

	void DynamicModelRenderer::SetNodeBuffer(Node* node, UINT& index, NodeBufferType* nodeBuffer)
	{
		// DX에서 HLSL 로 넘어갈때 자동으로 전치가 되서 넘어간다.
		// HLSL 에서도 Row Major 하게 작성하고 싶으므로 미리 전치를 시켜놓는다.
		// 총 전치가 2번되므로 HLSL에서도 Row Major한 Matrix로 사용한다.
		nodeBuffer->transformMatrix[node->id] = DirectX::XMMatrixTranspose(node->GetWorldMatrix());
		index++;
		for (int i = 0; i < node->children.size(); i++)
		{
			SetNodeBuffer(node->children[i], index, nodeBuffer);
		}
	}
}

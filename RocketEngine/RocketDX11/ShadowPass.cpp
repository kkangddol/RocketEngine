#include "ShadowPass.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "DeferredBuffers.h"
#include "ObjectManager.h"
#include "DirectionalLight.h"
#include "IRenderable.h"
#include "MeshRenderer.h"
#include "DynamicModelRenderer.h"

namespace Rocket::Core
{

	ShadowPass::ShadowPass()
	{

	}

	ShadowPass::~ShadowPass()
	{

	}

	void ShadowPass::Initialize(VertexShader* staticMeshVS, PixelShader* staticMeshPS, VertexShader* dynamicModelVS, PixelShader* dynamicModelPS)
	{
		_staticMeshVS = staticMeshVS;
		_staticMeshPS = staticMeshPS;
		_dynamicModelVS = dynamicModelVS;
		_dynamicModelPS = dynamicModelPS;
	}

	void ShadowPass::GenerateShadowMap(ID3D11DeviceContext* deviceContext, DeferredBuffers* g_buffer)
	{
		auto& objMgr = ObjectManager::Instance();

		g_buffer->SetShadowMapRenderTarget(deviceContext);

		// TODO : 라이트별로 물체를 컬링해서.. 그림자를 만들어내는..? 음..
		// 그러면 ShadowMap이 라이트 갯수만큼있어야..?
		for (auto& light : objMgr.GetDirectionalLightList())
		{
			std::vector<MeshRenderer*> staticMeshList;
			staticMeshList.reserve(256);

			std::vector<DynamicModelRenderer*> dynamicModelList;
			dynamicModelList.reserve(256);

			for (auto meshRenderer : objMgr.GetStaticMeshRenderers())
			{
				if (light->FrustumCulling(meshRenderer->GetBoundingBox()))
				{
					staticMeshList.push_back(meshRenderer);
				}
			}

			for (auto dynamicModelRenderer : objMgr.GetDynamicModelRenderers())
			{
				if (light->FrustumCulling(dynamicModelRenderer->GetBoundingBox()))
				{
					dynamicModelList.push_back(dynamicModelRenderer);
				}
			}

			// Draw On ShadowMap
			for (auto& meshRenderer : staticMeshList)
			{
				auto tempVS = meshRenderer->SetVertexShader(_staticMeshVS);
				auto tempPS = meshRenderer->SetPixelShader(_staticMeshPS);
				meshRenderer->Render(deviceContext, light->GetViewMatrix(), light->GetProjectionMatrix());
				meshRenderer->SetVertexShader(tempVS);
				meshRenderer->SetPixelShader(tempPS);
			}

			for (auto& modelRenderer : dynamicModelList)
			{
				auto tempVS = modelRenderer->SetVertexShader(_dynamicModelVS);
				auto tempPS = modelRenderer->SetPixelShader(_dynamicModelPS);
				modelRenderer->Render(deviceContext, light->GetViewMatrix(), light->GetProjectionMatrix());
				modelRenderer->SetVertexShader(tempVS);
				modelRenderer->SetPixelShader(tempPS);
			}
		}
	}

}

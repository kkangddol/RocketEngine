#include "RenderSystem.h"
#include "MeshRendererBase.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include <cassert>
#include "SceneSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "..\\RocketGraphicsInterface\\IRenderable.h"
#include "AnimationController.h"
#include "Animator.h"
#include "State.h"
#include "..\\RocketGraphicsInterface\\RenderConstantData.h"
#include "TimeSystem.h"
#include "PhysicsSystem.h"
#include "DynamicCollider.h"
#include "BoxCollider.h"
#include "..\\RocketMath\\RocketMath.h"
#include "TextBox.h"
#include "SpriteRenderer.h"
#include "..\\RocketGraphicsInterface\\ISketchable.h"
#include "DebugSystem.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include <algorithm>

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG

#pragma comment(lib,"..\\x64\\Debug\\RocketUtil.lib")

using GRAPHICS_CREATE_SIGNATURE = RocketCore::Graphics::IRocketGraphics* (*)(void);
constexpr const char* GRAPHICS_CREATE_NAME = "CreateGraphicsInstance";

#define GRAPHICS_RELEASE_SIGNATURE void(*)(RocketCore::Graphics::IRocketGraphics*)
#define GRAPHICS_RELEASE_NAME "ReleaseGraphicsInstance"

namespace RocketCore
{
	RenderSystem::RenderSystem()
		:_hWnd(), _screenWidth(), _screenHeight(),
		hGraphicsModule(LoadLibrary(GRAPHICSDLL_PATH)),
		_rocketGraphics(),
		_renderMap(),
		_uiMap()
	{
		DWORD error_code = GetLastError();
		assert(hGraphicsModule);

		auto a = GetProcAddress(hGraphicsModule, GRAPHICS_CREATE_NAME);
		error_code = GetLastError();

		_rocketGraphics.reset((reinterpret_cast<GRAPHICS_CREATE_SIGNATURE>(GetProcAddress(hGraphicsModule, GRAPHICS_CREATE_NAME)))());

		//extern "C" __declspec(dllexport) IRocketGraphics * CreateGraphicsInstance();
		//extern "C" __declspec(dllexport) void ReleaseGraphicsInstance(IRocketGraphics * instance);
	}

	void RenderSystem::Initialize(HWND hWnd, int screenWidth, int screenHeight, bool isEditor /*= false*/)
	{
		_hWnd = hWnd;
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_isEditor = isEditor;

		_rocketGraphics->Initialize(static_cast<void*>(hWnd), screenWidth, screenHeight, isEditor);
	}

	void RenderSystem::Finalize()
	{
		reinterpret_cast<GRAPHICS_RELEASE_SIGNATURE>(GetProcAddress(hGraphicsModule, GRAPHICS_RELEASE_NAME))(_rocketGraphics.release());
		FreeLibrary(hGraphicsModule);
	}

	void RenderSystem::MakeRenderableAll()
	{
		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
		{
			if (sceneIter.second == nullptr)
			{
				return;
			}

			for (auto& object : sceneIter.second->GetOriginalList())
			{
				// 각각의 객체가 본인들을 그리는 것이 아닌
				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
				// object->GetRenderData();

				MeshRendererBase* renderer = object->GetComponent<RocketEngine::MeshRenderer>();

				if (!renderer)
				{
					renderer = object->GetComponent<RocketEngine::SkinnedMeshRenderer>();

					if (!renderer)
					{
						continue;
					}
				}

				if (_renderMap.find(renderer) == _renderMap.end())
				{
					if (dynamic_cast<RocketEngine::MeshRenderer*>(renderer))
					{
// 						RocketCore::Graphics::IRenderable* renderableObj = _rocketGraphics->MakeRenderable(renderer->GetModelData());
// 						_renderMap.insert({ renderer, renderableObj });
					}

					if (dynamic_cast<RocketEngine::SkinnedMeshRenderer*>(renderer))
					{
// 						RocketCore::Graphics::IRenderable* renderableObj = _rocketGraphics->MakeSkinnedRenderable(renderer->GetModelData());
// 						_renderMap.insert({ renderer, renderableObj });
					}
				}
			}
		}
	}

	void RenderSystem::MakeAnimationAll()
	{
		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
		{
			if (sceneIter.second == nullptr)
			{
				return;
			}

			for (auto& object : sceneIter.second->GetOriginalList())
			{
				// 각각의 객체가 본인들을 그리는 것이 아닌
				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
				// object->GetRenderData();

				RocketEngine::AnimationController* animCtr = object->GetComponent<RocketEngine::AnimationController>();

				if (!animCtr)
				{
					continue;
				}

				for (auto& iter : animCtr->GetAnimator()->GetAllStates())
				{
/*					_rocketGraphics->MakeAnim(iter.second->GetAnimDesc(), &(iter.second->GetAnimDataRef()));*/
				}
			}
		}
	}

	void RenderSystem::MakeUIAll()
	{
		for (auto& sceneIter : SceneSystem::Instance().GetAllScenes())
		{
			if (sceneIter.second == nullptr)
			{
				return;
			}

			for (auto& object : sceneIter.second->GetOriginalList())
			{
				// 각각의 객체가 본인들을 그리는 것이 아닌
				// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
				// object->GetRenderData();

				std::vector<UIRenderer*> uiVec = object->GetComponentsDynamic<UIRenderer>();

				for (auto& ui : uiVec)
				{
					if (_uiMap.find(ui) == _uiMap.end())
					{
						RocketEngine::TextBox* tr = nullptr;
						if (tr = dynamic_cast<RocketEngine::TextBox*>(ui))
						{
// 							RocketCore::Graphics::ISketchable* sketchableObj = _rocketGraphics->MakeText(tr->GetFontSize(), tr->GetWidth(), tr->GetHeight(), tr->GetColor());
// 							_uiMap.insert({ ui, sketchableObj });
						}

						RocketEngine::SpriteRenderer* sr = nullptr;
						if (sr = dynamic_cast<RocketEngine::SpriteRenderer*>(ui))
						{
// 							RocketCore::Graphics::ISketchable* sketchableObj = _rocketGraphics->MakeImage(sr->GetPath().c_str(), sr->GetWidth(), sr->GetHeight());
// 							_uiMap.insert({ ui, sketchableObj });
						}
					}
				}
			}
		}
	}

	void RenderSystem::OnResize(int width, int height)
	{
/*		_rocketGraphics->Resize(width, height);*/
	}

	void RenderSystem::DrawProcess()
	{
		UpdateRenderData(); //SetRenderData + 전체 렌더 시작,
		_rocketGraphics->Render();
	}

	void RenderSystem::UpdateConstantData(RocketCore::Graphics::RenderConstantData& data)
	{
		// TODO : 씬 전역적으로 관리하지말고 씬단위로 관리할것 ㄷㄷ;;
		// 지금 라이트가 씬에 따라 가져오지 않고 그냥 전역적으로 갖고온다 세상에..
		// 23.08.23 강석원 인재원.

		for (auto& light : _lights)
		{
			if (!(light->gameObject->IsActive()))
			{
				continue;
			}

			void* temp = nullptr;

			temp = dynamic_cast<RocketEngine::DirectionalLight*>(light);

			if (temp)
			{
				data.dirLightList.push_back(static_cast<RocketEngine::DirectionalLight*>(temp)->GetLightData());
				continue;
			}

			temp = dynamic_cast<RocketEngine::PointLight*>(light);

			if (temp)
			{
				data.pointLightList.push_back(static_cast<RocketEngine::PointLight*>(temp)->GetLightData());
				continue;
			}

			temp = dynamic_cast<RocketEngine::SpotLight*>(light);

			if (temp)
			{
				data.spotLightList.push_back(static_cast<RocketEngine::SpotLight*>(temp)->GetLightData());
				continue;
			}
		}

		data.deltaTime = RocketCore::TimeSystem::Instance().GetDeltaTime();
	}

	void RenderSystem::UpdateRenderData()
	{
		auto mainCam = SceneSystem::Instance().GetCurrentScene()->GetMainCamera();
		_rocketGraphics->UpdateCamera(mainCam->GetCameraData());

		RocketCore::Graphics::RenderConstantData data;
		UpdateConstantData(data);
		_rocketGraphics->UpdateConstantData(data);
	}

	void RenderSystem::DrawCurrentScene()
{
		RocketEngine::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		// 현재 씬이 존재하지 않으면 Return.
		if (currentScene == nullptr)
		{
			return;
		}

		/// 현재씬이 존재한다면 진행될 동작들.
		_rocketGraphics->UpdateCamera(currentScene->GetMainCamera()->GetCameraData());

		for (auto& object : currentScene->GetOriginalList())
		{ 
			// 각각의 객체가 본인들을 그리는 것이 아닌
			// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
			// object->GetRenderData();

			if (!object->IsActive())
			{
				continue;
			}

			MeshRendererBase* renderer = object->GetComponentDynamic<MeshRendererBase>();

			if (!renderer)
			{
				continue;
			}

			// 위험하지만 우선 부하때문에 주석처리..?
// 			if (_renderMap.find(renderer) == _renderMap.end())
// 			{
// 				RocketCore::Graphics::IRenderable* renderableObj = _rocketGraphics->MakeRenderable(renderer->GetModelData());
// 				_renderMap.insert({ renderer, renderableObj });
// 			}

		}
	}

	void RenderSystem::DrawCurrentUI()
	{
		RocketEngine::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		// 현재 씬이 존재하지 않으면 Return.
		if (currentScene == nullptr)
		{
			return;
		}

		std::vector<UIRenderer*> UIVec;

		for (auto& object : currentScene->GetOriginalList())
		{
			// 각각의 객체가 본인들을 그리는 것이 아닌
			// RenderSystem에서 각각의 객체의 정보를 바탕으로 Graphics에게 그리라고 시키는 것
			// object->GetRenderData();

			if (!object->IsActive())
			{
				continue;
			}

			std::vector<UIRenderer*> renderers = object->GetComponentsDynamic<UIRenderer>();


			for (auto& renderer : renderers)
			{
				// button일때.. button은 sketchable을 만들지 않는다.
				if (dynamic_cast<RocketEngine::Button*>(renderer))
				{
					continue;
				}

				UIVec.push_back(renderer);


			}
		}

		std::sort(UIVec.begin(), UIVec.end(),
			[](UIRenderer* lhs, UIRenderer* rhs) 
			{
				return lhs->GetSortOrder() < rhs->GetSortOrder();
			});

		for (auto& renderer : UIVec)
		{
			RocketCore::Graphics::ISketchable* temp = _uiMap.at(renderer);
			temp->SetData(renderer->GetSketchData().c_str());
			temp->SetFillRatio(renderer->GetFillRatio());
 			_uiMap.at(renderer)->Render(renderer->Get2DWorldTM());
		}
		
	}

// 	void RenderSystem::DrawDebugBox()
// 	{
// 		if (!(DebugSystem::Instance().IsDebugMode() || _isEditor))
// 		{
// 			return;
// 		}
// 
// 		for (auto& box : DebugSystem::Instance().GetBoxQueue())
// 		{
// 			//_rocketGraphics->RenderCube(box.worldTM, box.widthHeightDepth.x, box.widthHeightDepth.y, box.widthHeightDepth.z, { box.isWire,box.color.r,box.color.g,box.color.b,box.color.a });
// 		}
// 
// 		DebugSystem::Instance().FlushBoxQueue();
// 	}

// 	void RenderSystem::DrawDebugText()
// 	{
// 		if (!(DebugSystem::Instance().IsDebugMode()))
// 		{
// 			return;
// 		}
// 
// 		DebugSystem::Instance().DrawDebugText({ 50.0f,50.0f }, "FPS : " + std::to_string(TimeSystem::Instance().GetFrameRate()), 20.0f);
// 
// 		for (auto& text : DebugSystem::Instance().GetTextQueue())
// 		{
// 			//_rocketGraphics->RenderDebugText(text.centerPos.x, text.centerPos.y, text.content.c_str(), text.size);
// 		}
// 
// 		DebugSystem::Instance().FlushTextQueue();
// 	}
// 
// 	void RenderSystem::DrawDebugLine()
// 	{
// 		if (!(DebugSystem::Instance().IsDebugMode()))
// 		{
// 			return;
// 		}
// 
// 		for (auto& line : DebugSystem::Instance().GetLineQueue())
// 		{
// 			//_rocketGraphics->RenderLine(line.beginPoint, line.endPoint, line.color);
// 		}
// 
// 		DebugSystem::Instance().FlushLineQueue();
// 	}
// 
// 	void RenderSystem::DrawDebug2DBox()
// 	{
// 		if (!(DebugSystem::Instance().IsDebugMode()))
// 		{
// 			return;
// 		}
// 
// 		for (auto& box2D : DebugSystem::Instance().Get2DBoxQueue())
// 		{
// 			//_rocketGraphics->Render2DBox(box2D.LT, box2D.RB, box2D.color);
// 		}
// 
// 		DebugSystem::Instance().Flush2DBoxQueue();
// 	}
// 
// 	void RenderSystem::RenderPhysicsDebug()
// 	{
// 		PhysicsSystem& temp = PhysicsSystem::Instance();
// 		for (auto& rigid : temp.GetRigidDynamicsList())
// 		{
// 			RocketEngine::DynamicCollider* col = static_cast<RocketEngine::DynamicCollider*>(rigid->userData);
// 			RocketEngine::BoxCollider* temp = dynamic_cast<RocketEngine::BoxCollider*>(col);
// 			RocketEngine::RMFLOAT4X4 worldTM = col->gameObject->transform.GetWorldTM();
// 			if (temp)
// 			{
// 				//_rocketGraphics->RenderCube(worldTM, temp->GetWidth(), temp->GetHeight(), temp->GetDepth(), { true,1.0f,0.0f,0.0f,1.0f });
// 			}
// 			else
// 			{
// 				//_rocketGraphics->RenderCube(worldTM, { true,1.0f,0.0f,0.0f,1.0f });
// 			}
// 		}
// 
// 		for (auto& rigid : temp.GetRigidStaticsList())
// 		{
// 			RocketEngine::StaticCollider* col = static_cast<RocketEngine::StaticCollider*>(rigid->userData);
// 			RocketEngine::PlaneCollider* temp = dynamic_cast<RocketEngine::PlaneCollider*>(col);
// 			RocketEngine::RMFLOAT4X4 worldTM = col->gameObject->transform.GetWorldTM();
// 			if (temp)
// 			{
// 				//_rocketGraphics->RenderCube(worldTM, 0.01f, 100.0f, 100.0f, { false,0.0f,1.0f,0.0f,1.0f });
// 			}
// 			else
// 			{
// 			//	_rocketGraphics->RenderCube(worldTM, { true,1.0f,0.0f,0.0f,1.0f });
// 			}
// 		}
// 	}



	int RenderSystem::GetScreenWidth() const
	{
		return _screenWidth;
	}

	int RenderSystem::GetScreenHeight() const
	{
		return _screenHeight;
	}

	void RenderSystem::AddLight(RocketEngine::Light* light)
	{
		_lights.push_back(light);
	}

}

#include <algorithm>
#include <cassert>

#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "ResourceSystem.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"
#include "DynamicModelRenderer.h"
#include "MeshRenderer.h"
#include "../RocketCommon/RawModelStruct.h"

namespace Rocket::Core
{
	ObjectSystem::ObjectSystem()
	{

	}

	void ObjectSystem::Initialize()
	{
		_enableList.clear();
		_disableList.clear();
	}

	void ObjectSystem::Finalize()
	{
		Rocket::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		for (auto& staticObj : _staticObjList)
		{
			staticObj->Finalize();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			staticComponent->Finalize();
		}

		for (auto& iter : SceneSystem::Instance().GetAllScenes())
		{
			for (auto& gameObject : iter.second->GetOriginalList())
			{
				gameObject->Finalize();
			}
		}
	}

	void ObjectSystem::StartCurrentScene()
	{
		Rocket::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();
		//assert(currentScene);

		if (currentScene == nullptr)
		{
			return;
		}

		for (auto& staticObj : _staticObjList)
		{
			staticObj->Start();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			if (!staticComponent->_isStarted)
			{
				staticComponent->Start();
				staticComponent->_isStarted = true;
			}
		}

		currentScene->Start();
	}

	void ObjectSystem::UpdateCurrentScene()
	{
		Rocket::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		if (currentScene == nullptr)
		{
			return;
		}

		for (auto& staticObj : _staticObjList)
		{
			staticObj->Update();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			staticComponent->Update();
		}

		currentScene->Update();
	}

	void ObjectSystem::LateUpdateCurrentScene()
	{
		Rocket::Scene* currentScene = SceneSystem::Instance().GetCurrentScene();

		if (currentScene == nullptr)
		{
			return;
		}

		for (auto& staticObj : _staticObjList)
		{
			staticObj->LateUpdate();
		}

		for (auto& staticComponent : _staticComponentList)
		{
			staticComponent->LateUpdate();
		}

		currentScene->LateUpdate();
	}

	void ObjectSystem::UpdateScene(Rocket::Scene* scene)
	{

	}

	void ObjectSystem::FlushEnable()
	{
		for (auto obj : _enableList)
		{
			obj->FlushEnable();
		}

		_enableList.clear();
	}

	void ObjectSystem::FlushDisable()
	{
		for (auto obj : _disableList)
		{
			obj->FlushDisable();
		}

		_disableList.clear();
	}

	void ObjectSystem::AddEnable(Rocket::GameObject* obj)
	{
		_enableList.push_back(obj);
	}

	void ObjectSystem::AddDisable(Rocket::GameObject* obj)
	{
		_disableList.push_back(obj);
	}

	Rocket::GameObject* ObjectSystem::CreateObject(const std::string& objName)
	{
		Rocket::GameObject* gameObject = new Rocket::GameObject(objName);
		return gameObject;
	}

	Rocket::GameObject* ObjectSystem::CreateStaticObject(const std::string& objName)
	{
		Rocket::GameObject* gameObject = new Rocket::GameObject(objName);
		_staticObjList.push_back(gameObject);
		return gameObject;
	}

	Rocket::GameObject* ObjectSystem::CreateModelObject(const std::string& fileName)
	{
		RawModel* rawModel = ResourceSystem::Instance().GetModel(fileName);
		Rocket::GameObject* resultGameObject;

		if (0 < rawModel->animations.size())
		{
			// 애니메이션이 있다면 (dynamic)
			resultGameObject = CreateGameObjectFromRawNode(rawModel->rootNode, true);
		}
		else
		{
			// 애니메이션이 없다면 (static)
			resultGameObject = CreateGameObjectFromRawNode(rawModel->rootNode, false);
		}

	}

	Rocket::GameObject* ObjectSystem::CreateGameObjectFromRawNode(RawNode* node, bool isDynamic)
	{
		Rocket::GameObject* gameObject = new Rocket::GameObject(node->name);

		Vector3 position;
		Quaternion rotation;
		Vector3 scale;

		node->transformMatrix.Decompose(scale, rotation, position);
		gameObject->transform.SetLocalPosition(position);
		gameObject->transform.SetLocalRotation(rotation);
		gameObject->transform.SetLocalScale(scale);


		// TODO : 내꺼는 지금 Model 기준으로 Render 하는데 여기서는 Mesh 기준으로..해야..되는건가..?
		//			Static한 Model은 Node마다 메쉬가 다를텐데..
		//			Static한 Model은 node마다 meshRenderer를 달아주고..
		//			Dynamic한 Model은 하나의 node에다가 ModelRenderer를 달아주고..?
		if (isDynamic)
		{
			if (0 < node->meshes.size())
			{
				gameObject->AddComponent<Rocket::DynamicModelRenderer>();
				// TODO : 여기서 모델이나 메쉬 로드해주기 전에 리소스매니저에 리소스가 있는지?
				// 없을 것이기 때문에 앞 단의 ResourceSystem에서 읽은 데이터를 넘겨주면서 로드해두라고 해야함.
				// 240319 1714 현재작업중.
			}
		}
		else
		{
			for (auto& rawMesh : node->meshes)
			{
				gameObject->AddComponent<Rocket::MeshRenderer>();
				// TODO : Mesh의 이름을 기준으로 넣어줘보자.. 이름이 겹치는 경우가 있으려나? 세상에 너무 슬플거같다.
			}
		}


		for (auto& child : node->children)
		{
			Rocket::GameObject* childObject = CreateGameObjectFromRawNode(child, isDynamic);
			childObject->transform.SetParent(gameObject);
		}

		return gameObject;
	}

	void ObjectSystem::AddStaticComponent(Rocket::Component* component)
	{
		if (std::find(_staticComponentList.begin(), _staticComponentList.end(), component) != _staticComponentList.end())
		{
			return;
		}

		_staticComponentList.push_back(component);
	}
}

#include "RocketAPI.h"

#include <windows.h>

#include "ObjectSystem.h"
#include "SceneSystem.h"
#include "TimeSystem.h"
#include "TweenSystem.h"
#include "InputSystem.h"
#include "RenderSystem.h"
#include "DebugSystem.h"
#include "PhysicsSystem.h"
#include "DataSystem.h"
#include "SoundSystem.h"

constexpr const char* DEFAULT_IMAGE_PATH = "..\\GraphicsResources\\Images\\luffy.jpg";

namespace RocketEngine
{

	EngineProcess* CreateLauncher()
	{
		return new EngineProcess();
	}

	void ReleaseLauncher(IEngineProcess* instance)
	{
		delete instance;
	}

	void LoadSceneFromJSON(std::string sceneDataPath)
	{
		RocketCore::DataSystem::Instance().SetAllDataToScene(sceneDataPath);
	}

	GameObject* CreateObject(const std::string& objName)
	{
		return RocketCore::SceneSystem::Instance().GetCurrentScene()->CreateObject(objName);
	}

	GameObject* CreateStaticObject(const std::string& objName)
	{
		return RocketCore::ObjectSystem::Instance().CreateStaticObject(objName);

	}

	ROCKET_API Scene* CreateScene(const std::string& sceneName, bool withNoCam /*= false*/)
	{
		return RocketCore::SceneSystem::Instance().CreateScene(sceneName, withNoCam);
	}

	Tween* CreateTween()
	{
		return &(RocketCore::TweenSystem::Instance().CreateTween());
	}

	GameObject* CreateButton(const std::string& objName)
	{
		GameObject* button = RocketCore::SceneSystem::Instance().GetCurrentScene()->CreateObject(objName);
		button->AddComponent<Button>();
		SpriteRenderer* buttonRenderer = button->AddComponent<SpriteRenderer>();
		buttonRenderer->SetPath(DEFAULT_IMAGE_PATH);
		return button;
	}

	GameObject* CreateImageBox(const std::string& objName)
	{
		GameObject* imageBox = RocketCore::SceneSystem::Instance().GetCurrentScene()->CreateObject(objName);
		SpriteRenderer* imageRenderer = imageBox->AddComponent<SpriteRenderer>();
		imageRenderer->SetPath(DEFAULT_IMAGE_PATH);
		return imageBox;
	}

	GameObject* CreateTextBox(const std::string& objName)
	{
		GameObject* textBox = RocketCore::SceneSystem::Instance().GetCurrentScene()->CreateObject(objName);
		SpriteRenderer* imageRenderer = textBox->AddComponent<SpriteRenderer>();
		TextBox* textRenderer = textBox->AddComponent<TextBox>();

		imageRenderer->SetPath(DEFAULT_IMAGE_PATH);
		textRenderer->SetText("기본 텍스트 입니다.");
		textRenderer->SetSortOrder(1);

		return textBox;
	}

	Camera* GetMainCamera()
	{
		return RocketCore::SceneSystem::Instance().GetCurrentScene()->GetMainCamera();
	}

	void SetMainCamera(Camera* camera)
	{
		RocketCore::SceneSystem::Instance().GetCurrentScene()->SetMainCamera(camera);
	}

	void RocketDestroyWindow()
	{
		DestroyWindow(RocketCore::RenderSystem::Instance()._hWnd);
	}

	bool LoadScene(const std::string& sceneName)
	{
		return RocketCore::SceneSystem::Instance().LoadScene(sceneName);
	}

	float GetDeltaTime()
	{
		return RocketCore::TimeSystem::GetDeltaTime();
	}

	bool GetKeyDown(int keyCode)
	{
		return RocketCore::InputSystem::Instance().GetKeyDown(keyCode);
	}

	bool GetKeyUp(int keyCode)
	{
		return RocketCore::InputSystem::Instance().GetKeyUp(keyCode);
	}

	bool GetKey(int keyCode)
	{
		return RocketCore::InputSystem::Instance().GetKey(keyCode);
	}

	Vector2 GetMousePosition()
	{
		return RocketCore::InputSystem::Instance().GetMousePosition();
	}

	Vector2 GetMouseDelta()
	{
		return RocketCore::InputSystem::Instance().GetMouseDelta();
	}

	ROCKET_API Vector2 MouseDeltaSmooth()
	{
		return RocketCore::InputSystem::Instance().MouseDeltaSmooth();
	}

	void AddStaticComponent(Component* component)
	{
		RocketCore::ObjectSystem::Instance().AddStaticComponent(component);
	}

	float GetMouseSensitivity()
	{
		return RocketCore::InputSystem::Instance().GetMouseSensitivity();
	}

	void SetMouseSensitivity(float sensitivity)
	{
		RocketCore::InputSystem::Instance().SetMouseSensitivity(sensitivity);
	}

	GameObject* FindObjectByName(std::string name)
	{
		return RocketCore::SceneSystem::Instance().GetCurrentScene()->FindObjectByName(name);
	}

	ROCKET_API Collider* ShootRay(Vector3 origin, Vector3 direction, float length /*= 100.0f*/, int* type /*= nullptr*/)
	{
		// type이 1이면 rigidStatic, 2면 rigidDynamic
		return RocketCore::PhysicsSystem::Instance().RayCast(origin, direction, length, type);
	}

// 	void ToggleDebugMode()
// 	{
// 		RocketCore::DebugSystem& instance = RocketCore::DebugSystem::Instance();
// 		instance.SetDebugMode(!instance.IsDebugMode());
// 	}

	float GetGroupVolume(eSoundGroup group)
	{
		return RocketCore::SoundSystem::Instance().GetGroupVolume(group);
	}

	void SetGroupVolume(eSoundGroup group, float volume)
	{
		RocketCore::SoundSystem::Instance().SetGroupVolume(group, volume);
	}

	void AddAudio(const std::string& audioPath, eSoundGroup group)
	{
		RocketCore::SoundSystem::Instance().CreateSound(audioPath, group);
	}

	void PlayAudioOnce(const std::string& audioPath, float volume)
	{
		if (RocketCore::SoundSystem::Instance().IsSoundPlaying(audioPath))
		{
			return;
		}

		RocketCore::SoundSystem::Instance().PlayOnce(audioPath);
	}

	ROCKET_API void PlayAudioOnceNested(const std::string& audioPath, float volume)
	{
		RocketCore::SoundSystem::Instance().PlayOnce(audioPath);
	}

	ROCKET_API void PlayAudioRepeat(const std::string& audioPath, float volume)
	{
		if (RocketCore::SoundSystem::Instance().IsSoundPlaying(audioPath))
		{
			return;
		}

		RocketCore::SoundSystem::Instance().RepeatPlayback(audioPath);
	}

	ROCKET_API void PlayEnd(std::string soundPath)
	{
		RocketCore::SoundSystem::Instance().PlayEnd(soundPath);
	}

	ROCKET_API void PlayAllEnd()
	{
		RocketCore::SoundSystem::Instance().PlayAllEnd();
	}

	void DrawDebugText(Vector2 centerPos, std::string content, float size)
	{
		RocketCore::DebugSystem::Instance().DrawDebugText(centerPos, content, size);
	}

	void DrawDebugBox(Matrix worldTM, Vector3 widthHeightDepth /*= { 1.0f,1.0f,1.0f }*/, bool isWire /*= true*/, Vector4 color /*= { 1.0f,0.0f,0.0f,0.0f }*/)
	{
		RocketCore::DebugSystem::Instance().DrawDebugBox(worldTM, widthHeightDepth, isWire, color);
	}

	void DrawDebugLine(Vector3 beginPoint, Vector3 endPoint, Vector4 color /*= { 1.0f,0.0f,0.0f,1.0f }*/)
	{
		RocketCore::DebugSystem::Instance().DrawDebugLine(beginPoint, endPoint, color);
	}

	void DrawDebug2DBox(Vector2 LT, Vector2 RB, Vector4 color /*= { 1.0f,0.0f,0.0f,1.0f }*/)
	{
		RocketCore::DebugSystem::Instance().DrawDebug2DBox(LT, RB, color);
	}


}

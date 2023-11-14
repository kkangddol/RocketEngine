#pragma once

namespace RocketEngine
{
	class Scene;
}

class TestScene
{
public:
	TestScene();

public:
	void Initialize();

public:
	RocketEngine::Scene* scene;

private:
	//RocketAPILoader& _rocket;
};

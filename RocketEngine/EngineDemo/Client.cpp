#include "Client.h"
#include "RocketAPILoader.h"

#include "TestScene.h"

void Client::Initialize()
{
	TestScene testScene;
	testScene.Initialize();

	RocketEngine::LoadScene("TEST");
}

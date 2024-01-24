#include "Client.h"
#include "..\\RocketEngine\\RocketAPI.h"

#include "TestScene.h"

void Client::Initialize()
{
	TestScene testScene;
	testScene.Initialize();

	RocketEngine::LoadScene("TEST");
}

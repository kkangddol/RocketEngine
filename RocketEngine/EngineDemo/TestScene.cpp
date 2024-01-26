#include "TestScene.h"
#include "..\\RocketEngine\\RocketAPI.h"
#include "DebugCameraMove.h"
#include <cassert>
#include <cstdlib>
#include <ctime>

TestScene::TestScene()
	: scene()
{ 

}

void TestScene::Initialize()
{
	scene = Rocket::CreateScene("TEST");
	assert(scene);

	auto camObj = scene->GetMainCamera()->gameObject;
	camObj->AddComponent<DebugCameraMove>();	
	scene->GetMainCamera()->GetCamera().SetAsMainCamera();

	auto cube = scene->CreateObject("Cube");
	auto meshRenderer = cube->AddComponent<Rocket::MeshRenderer>();
	meshRenderer->SetMesh(Rocket::eMeshType::SPHERE);
}

#include "TestScene.h"
#include "..\\RocketEngine\\RocketAPI.h"
#include "DebugCameraMove.h"
#include "PlayerController.h"

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

	auto player = scene->CreateObject("Sphere");
	auto meshRenderer = player->AddComponent<Rocket::MeshRenderer>();
	meshRenderer->SetMesh(Rocket::eMeshType::SPHERE);
	player->AddComponent<PlayerController>();

	auto text = scene->CreateObject("text");
	text->AddComponent<Rocket::TextBox>();
	text->GetComponent<Rocket::TextBox>()->SetText("Hello World");
}

﻿#include "TestScene.h"
#include "..\\RocketEngine\\RocketAPI.h"
#include "DebugCameraMove.h"
#include "PlayerController.h"
#include "PBRController.h"

TestScene::TestScene()
	: scene()
{ 

}

void TestScene::Initialize()
{
	scene = Rocket::CreateScene("TEST");
	assert(scene);

	/// 카메라
	auto camObj = scene->GetMainCamera()->gameObject;
	camObj->AddComponent<DebugCameraMove>();	
	scene->GetMainCamera()->SetAsMainCamera();

	/// 라이트
	auto lightObj = scene->CreateObject("light");
	auto lightComp = lightObj->AddComponent<Rocket::DirectionalLight>();
	lightComp->SetSpecularPower(4.0f);
	lightObj->transform.Rotate(45.0f, 45.0f, 0.0f);

	/// PBR 테스트
	auto PBRTest = scene->CreateObject("PBRTest");
	PBRTest->AddComponent<PBRController>();
	PBRTest->transform.SetPosition(0.0f, 0.0f, -10.0f);
	auto PBRRenderer = PBRTest->AddComponent<Rocket::MeshRenderer>();
	PBRRenderer->SetMesh(Rocket::eMeshType::SPHERE);
	PBRRenderer->SetBaseColorTexture("T_WEP_Basic_009_D.png");

	/// 스킨드 메쉬 테스트
	auto skinnedTest1 = scene->CreateModelObject("Rob02.fbx");
	skinnedTest1->transform.Translate(0.0f, 0.0f, 3.0f);

	auto dmr1 = skinnedTest1->GetComponentsFromAll<Rocket::DynamicModelRenderer>();
	for (auto& m : dmr1)
	{
		m->SetBaseColorTexture("Rob02Yellow_AlbedoTransparency.png");
		m->SetNormalTexture("Rob02_Normal.dds");
		m->SetMetallicTexture("Rob02White_MetallicSmoothness.dds");
		m->SetRoughnessTexture("Rob02White_Roughness.png");
	}

	auto skinnedTest2 = scene->CreateModelObject("Dying.fbx");
	skinnedTest2->transform.SetScale(0.005f, 0.005f, 0.005f);
	skinnedTest2->transform.Translate(-3.0f, 0.0f, 0.0f);
	
	auto skinnedTest3 = scene->CreateModelObject("A_TP_CH_Sprint_F.fbx");
	skinnedTest3->transform.SetScale(0.05f, 0.05f, 0.05f);
	skinnedTest3->transform.Translate(3.0f, 0.0f, 0.0f);

	auto dmr3 = skinnedTest3->GetComponentsFromAll<Rocket::DynamicModelRenderer>();
	for (auto& m : dmr3)
	{
		m->SetBaseColorTexture("T_TP_CH_Camo_001_006_D.png");
	}

	/// 스태틱 메쉬 테스트
	auto staticTest = scene->CreateModelObject("SK_TP_CH_Default.fbx");
	//staticTest->transform.SetPosition(1.0f, 0.0f, 0.0f);

	auto mr = staticTest->GetComponentsFromAll<Rocket::MeshRenderer>();
	for (auto& m : mr)
	{
		m->SetBaseColorTexture("T_TP_CH_Camo_006_003_D.png");
	}

	/// 노드구조대로 게임오브젝트 생성한 것 테스트
	auto hand = scene->FindObjectByName("hand_r");
	staticTest->transform.SetParent(hand,false);
	staticTest->transform.SetScale(1.0f, 1.0f, 1.0f);


	/// 스태틱 메쉬 계층구조 테스트
	auto hierarchyTest = scene->CreateModelObject("SM_Box_Cargo.fbx");
	auto hierarchyRenderer = hierarchyTest->GetComponentsFromAll<Rocket::MeshRenderer>();
	for(auto& m : hierarchyRenderer)
	{
		m->SetBaseColorTexture("T_TP_CH_Props_D.png");
	}

	/// 텍스트 테스트
	auto text = scene->CreateObject("text");
	text->AddComponent<Rocket::TextBox>();
	text->GetComponent<Rocket::TextBox>()->SetText("Hello World\nBABO");
	text->transform.Translate(1400.0f, 0.0f, 0.0f);

	/// 스프라이트 테스트
	auto sprite = scene->CreateObject("sprite");
	sprite->AddComponent<Rocket::SpriteRenderer>();
	sprite->transform.Translate(1400.0f, 700.0f, 0.0f);

	/// 부하테스트
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			/// 스킨드 메쉬 부하 테스트
// 			auto skinnedTest3 = scene->CreateModelObject("A_TP_CH_Sprint_F.fbx");
// 			skinnedTest3->transform.SetScale(0.05f, 0.05f, 0.05f);
// 			skinnedTest3->transform.Translate(5 + i, 0.0f, 5 + j);
// 
// 			auto dmr3 = skinnedTest3->GetComponentsFromAll<Rocket::DynamicModelRenderer>();
// 			for (auto& m : dmr3)
// 			{
// 				m->SetTexture("T_TP_CH_Camo_001_006_D.png");
// 			}

			/// 스태틱 메쉬 부하 테스트
// 			auto staticTest = scene->CreateModelObject("SK_TP_CH_Default.fbx");
// 			staticTest->transform.SetPosition(5 + i, 0.0f, 5 + j);
// 
// 			auto mr = staticTest->GetComponentsFromAll<Rocket::MeshRenderer>();
// 			for (auto& m : mr)
// 			{
// 				m->SetTexture("T_TP_CH_Camo_006_003_D.png");
// 			}
		}
	}
}

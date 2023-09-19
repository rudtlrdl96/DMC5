#include "PrecompileHeader.h"
#include "TestStageLevel.h"
#include "NetworkManager.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "FreeCameraActor.h"

#include "JudgementCut.h"
#include "ColorEffect.h"
#include "FXAA_Effect.h"
#include "ZoomEffect.h"

#include "Plane.h"
#include "TestObject.h"
#include "PlayerActor_Nero.h"

#include "ShaderTestActor.h"
#include "FreeCameraActor.h"
#include "ReflectionProbe.h"

#include <GameEngineCore/GameEngineGUI.h>
#include <GameEngineCore/BloomEffect.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

TestStageLevel* TestStageLevel::Inst = nullptr;

TestStageLevel::TestStageLevel()
{
	Inst = this;
}

TestStageLevel::~TestStageLevel()
{
	Inst = nullptr;
}

void TestStageLevel::Start()
{
	CreateActor<FreeCameraActor>();

	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();

	GetCamera(0)->GetDeferredLightTarget()->CreateEffect<BloomEffect>();

	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();

	StageBaseLevel::Start();

	if (false == GameEngineInput::IsKey("RefTestKey"))
	{
		GameEngineInput::CreateKey("RefTestKey", VK_SPACE);
	}

	TestLight = CreateActor<GameEngineLight>();
	TestLight->SetName("Directional Light");
	TestLight->SetLightType(LightType::Directional);
	TestLight->LightDataValue.LightPower = 1.0f;
	TestLight->IsShadowLight = true;
	TestLight->LightDataValue.ShadowTargetSizeX = 16384;
	TestLight->LightDataValue.ShadowTargetSizeY = 16384;
	TestLight->ShadowRange.x = 16384;
	TestLight->ShadowRange.y = 16384;
	TestLight->GetTransform()->SetWorldPosition(float4(0, 4000, 0));
	TestLight->GetTransform()->SetLocalRotation(float4(45, 0, 0));
	TestLight->LightDataValue.LightColor = float4(0.5f, 0.5f, 0.5f);

	float4 F = TestLight->GetTransform()->GetWorldForwardVector();
}

void TestStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);

	if (true == GameEngineInput::IsDown("RefTestKey"))
	{
		std::shared_ptr<GameEngineActor> TempActor = CreateActor<GameEngineActor>();
		TempActor->GetTransform()->SetLocalPosition(float4(-500, 300, 0));

		std::shared_ptr<ReflectionProbe> Reflect = TempActor->CreateComponent<ReflectionProbe>();
		Reflect->Init("TestReflection", float4(256, 256));

		ReflectionTextureSetting(Reflect->GetReflectionCubeTexture(), TempActor->GetTransform()->GetWorldPosition(), float4(10000, 10000, 10000));

		GetDirectionalLight()->BakeShadow(GetMainCamera());	
		TestLight->BakeShadow(GetMainCamera());
	}
	//GameEngineCore::GetFrameRate();
}

void TestStageLevel::LevelChangeStart()
{
	//�ӽ� �ҷ�����, ���� ���� ����
	StageBaseLevel::LevelChangeStart();
	{
		GameEngineDirectory Dir = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			}
		);
		std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".fbx" });
		for (GameEngineFile& File : FBXFiles)
		{
			if (nullptr == GameEngineFBXMesh::Find(File.GetFileName()))
			{
				GameEngineFBXMesh::Load(File.GetFullPath());
			}
		}
	}

	{
		GameEngineDirectory Dir = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestObj"
			}
		);
		std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".fbx" });
		for (GameEngineFile& File : FBXFiles)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}
	
	SetCamera({ 0,0,-500 });
	CreateStage(AllStageDatas[0]);

	std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
	Nero->SinglePlayLoad();
	Nero->SetWorldPosition({ 0, 100, 0 });

	{

		std::shared_ptr<ShaderTestActor> Nero = CreateActor<ShaderTestActor>();
		//Nero->SinglePlayLoad();
		Nero->GetTransform()->SetWorldPosition({ 0, 100, 0 });
	}

	// �׽�Ʈ�� �ڵ�
	GetDirectionalLight()->GetTransform()->SetWorldPosition(float4(0, 4000, 0));
	GetDirectionalLight()->GetTransform()->SetWorldRotation(float4(45, 180, 0));

	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "AllRenderTarget", GetLevel()->GetMainCamera()->GetCamAllRenderTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "LightRenderTarget", GetLevel()->GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "MainCameraForwardTarget", GetLevel()->GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "DeferredTarget", GetLevel()->GetMainCamera()->GetCamDeferrdTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(4, "ShadowTarget1", GetLevel()->GetDirectionalLight()->GetShadowTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(5, "ShadowTarget2", TestLight->GetShadowTarget());
}
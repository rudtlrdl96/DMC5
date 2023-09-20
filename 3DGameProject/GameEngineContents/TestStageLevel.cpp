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
}

void TestStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);
}

void TestStageLevel::LevelChangeStart()
{
	//임시 불러오기, 추후 수정 예정
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
	Nero->SetUserControllType();
	Nero->SetWorldPosition({ 0, 100, 0 });

	// 테스트용 코드
	GetDirectionalLight()->GetTransform()->SetWorldPosition(float4(0, 4000, 0));
	GetDirectionalLight()->GetTransform()->SetWorldRotation(float4(45, 180, 0));

	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "AllRenderTarget", GetLevel()->GetMainCamera()->GetCamAllRenderTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "LightRenderTarget", GetLevel()->GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "MainCameraForwardTarget", GetLevel()->GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "DeferredTarget", GetLevel()->GetMainCamera()->GetCamDeferrdTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(4, "ShadowTarget0", GetLevel()->GetDirectionalLight()->GetShadowTarget());
}
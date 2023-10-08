#include "PrecompileHeader.h"
#include "TestStageLevel.h"
#include "NetworkManager.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "FreeCameraActor.h"

#include "JudgementCut.h"
#include "ColorEffect.h"
#include "FXAA_Effect.h"
#include "ZoomEffect.h"
#include "DistortionEffect.h"

#include "Plane.h"
#include "TestObject.h"
#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"

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

	{
		std::shared_ptr<DistortionEffect> Distortion = GetCamera(0)->GetCamTarget()->CreateEffect<DistortionEffect>();
		Distortion->SetMaskTexture(GetCamera(0)->GetCamAlphaTarget(), 1);	
		Distortion->SetDistortionValue(8, 4.5);
	}

	StageBaseLevel::Start();

	if (false == GameEngineInput::IsKey("BakeTestKey"))
	{
		GameEngineInput::CreateKey("BakeTestKey", VK_SPACE);
	}

	//CreateActor<ShaderTestActor>()->GetTransform()->SetWorldPosition(float4(0, 100, 0));

}

void TestStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);

	if (true == GameEngineInput::IsDown("BakeTestKey"))
	{
		GetDirectionalLight()->BakeShadow(GetMainCamera());
	}
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

	if (true)
	{
		std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
		Nero->SetUserControllType();
		Nero->SetWorldPosition({ 0, 100, 0 });
	}
	else
	{
		std::shared_ptr<PlayerActor_Vergil> Vergil = CreateActor<PlayerActor_Vergil>();
		Vergil->SetUserControllType();
		Vergil->SetWorldPosition({ 0, 100, 0 });
	}

	// 테스트용 코드
	GetDirectionalLight()->GetTransform()->SetWorldPosition(float4(0, 4000, 0));
	GetDirectionalLight()->GetTransform()->SetWorldRotation(float4(45, 0, 0));
	GetDirectionalLight()->SetLightColor(float4(0.4f, 0.4f, 0.4f));

	std::shared_ptr<GameEngineLight> SpotLight = CreateSpotLight(float4(0, 400, 0), ShadowTextureScale::S_512, 2000, 90);
	SpotLight->GetTransform()->SetLocalRotation(float4(90, 0, 0));
	SpotLight->SetLightPower(3.0f);
	SpotLight->DynamicShadowOn();


	std::shared_ptr<EffectRenderer> NewEffect = CreateActor<GameEngineActor>()->CreateComponent<EffectRenderer>();
	NewEffect->GetTransform()->SetLocalPosition(float4(0, 300, 0));
	NewEffect->GetTransform()->SetLocalScale(float4(300, 300, 100));

	NewEffect->RectInit("Effect_2D");
	NewEffect->SetTexture("DiffuseTexture", "Effect_Texture_03.tga");
	NewEffect->SetDistortionTexture();

	//CreateActor<ShaderTestActor>()->GetTransform()->SetLocalPosition(float4(0, 200, 0));

	std::shared_ptr<GameEngineLight> PointLight = CreatePointLight(float4(0, 400, 0), ShadowTextureScale::S_512, 1024);
	PointLight->DynamicShadowOn();

	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "Forward Target", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "Deferred Target", GetMainCamera()->GetCamDeferrdTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "Light Target", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "Alpha Target", GetMainCamera()->GetCamAlphaTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(4, "Last Target", GetMainCamera()->GetCamTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(5, "Bake Shadow", GetDirectionalLight()->GetBakeTarget(0));
	GameEngineCoreWindow::AddDebugRenderTarget(6, "Last Shadow", GetDirectionalLight()->GetShadowTarget());
}
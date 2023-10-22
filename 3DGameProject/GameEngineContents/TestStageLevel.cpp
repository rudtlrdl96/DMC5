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
#include "EventZone.h"
#include "RedSeal.h"
#include "EnemySpawner.h"

#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "Item_RedOrb.h"
#include "ShaderTestActor.h"
#include "FreeCameraActor.h"
#include "ReflectionProbe.h"
#include "BWColorEffect.h"

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
}

void TestStageLevel::Start()
{
	CreateActor<FreeCameraActor>();


	GetCamera(0)->GetCamTarget()->CreateEffect<ColorEffect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();

	Bloom = GetCamera(0)->GetDeferredLightTarget()->CreateEffect<BloomEffect>();

	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();
	BWEffect = GetCamera(0)->GetCamTarget()->CreateEffect<BWColorEffect>();

	{
		std::shared_ptr<DistortionEffect> Distortion = GetCamera(0)->GetCamTarget()->CreateEffect<DistortionEffect>();
		Distortion->SetMaskTexture(GetCamera(0)->GetCamAlphaTarget(), 1);	
		Distortion->SetDistortionValue(10, 10);
		Distortion->SetMaxPixelValue(100, 100);
	}

	{
		std::shared_ptr<DistortionEffect> Distortion = GetCamera(0)->GetCamTarget()->CreateEffect<DistortionEffect>();
		Distortion->SetMaskTexture(GetCamera(100)->GetCamAlphaTarget(), 1);
		Distortion->SetDistortionValue(10, 10);
		Distortion->SetMaxPixelValue(100, 100);
	}
		
	StageBaseLevel::Start();

	if (false == GameEngineInput::IsKey("BakeTestKey"))
	{
		GameEngineInput::CreateKey("BakeTestKey", VK_PRIOR);
	}
}

std::shared_ptr<GameEngineLight> PointLight = nullptr;

void TestStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);

	if (true == GameEngineInput::IsDown("BakeTestKey"))
	{
		const std::list<std::shared_ptr<GameEngineLight>>& AllLightRef = GetAllLightRef();
		
		for (std::shared_ptr<GameEngineLight> Ref: AllLightRef)
		{
			Ref->BakeShadow(GetMainCamera());
		}

		//GetDirectionalLight()->BakeShadow(GetMainCamera());
		//BWEffect->BWColorEffectSwitch();

		//if (nullptr != PointLight)
		//{
		//	PointLight->Death();
		//	PointLight = nullptr;
		//}
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
				"Map", "Location2"
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
		Nero->SetWorldPosition({ 8615.f, 0.f, 5060.f });
		Nero->GetTransform()->SetWorldPosition({ 8615.f, 0.f, 5060.f });
	}
	else
	{
		std::shared_ptr<PlayerActor_Vergil> Vergil = CreateActor<PlayerActor_Vergil>();
		Vergil->SetUserControllType();
		Vergil->SetWorldPosition({ 8615.f, 0, 5060.f });
	}

	// 테스트용 코드
	GetDirectionalLight()->GetTransform()->SetWorldPosition(float4(0, 30000, 0));
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 45.f,45.f,45.f });
	GetDirectionalLight()->SetLightPower(0.5f);
	GetDirectionalLight()->SetLightColor({ 0.85f,0.85f,1.f });
	//GetDirectionalLight()->SetLightColor(float4(0.4f, 0.4f, 0.4f));

	//std::shared_ptr<GameEngineLight> SpotLight = CreateSpotLight(float4(0, 400, 0), ShadowTextureScale::S_512, 2000, 90);
	//SpotLight->GetTransform()->SetLocalRotation(float4(90, 0, 0));
	//SpotLight->SetLightPower(3.0f);
	//SpotLight->DynamicShadowOn();

	if (nullptr == GameEngineTexture::Find("DistortionSample_00.jpg"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Texture", "DistortionTexture"
			});

		GameEngineTexture::Load(Path + "\\DistortionSample_00.jpg");
		GameEngineTexture::Load(Path + "\\DistortionSample_01.jpg");
		GameEngineTexture::Load(Path + "\\DistortionSample_02.jpg");
	}

	//{
	//	std::shared_ptr<EffectRenderer> NewEffect = CreateActor<GameEngineActor>()->CreateComponent<EffectRenderer>();
	//	NewEffect->RectInit("Effect_2D");
	//
	//	NewEffect->GetTransform()->SetLocalPosition(float4(-300, 300, 0));
	//	NewEffect->GetTransform()->SetLocalScale(float4(300, 300, 100));
	//	NewEffect->SetTexture("DiffuseTexture", "Effect_Texture_03.tga");
	//	NewEffect->SetDistortionTexture("DistortionSample_00.jpg");	
	//}
	//
	//{
	//	std::shared_ptr<EffectRenderer> NewEffect = CreateActor<GameEngineActor>()->CreateComponent<EffectRenderer>();
	//	NewEffect->RectInit("Effect_2D");
	//
	//	NewEffect->GetTransform()->SetLocalPosition(float4(0, 300, 0));
	//	NewEffect->GetTransform()->SetLocalScale(float4(300, 300, 300));
	//	NewEffect->SetTexture("DiffuseTexture", "Effect_Texture_03.tga");
	//	NewEffect->SetDistortionTexture("DistortionSample_01.jpg");
	//}
	//
	//{
	//	std::shared_ptr<EffectRenderer> NewEffect = CreateActor<GameEngineActor>()->CreateComponent<EffectRenderer>();
	//	NewEffect->RectInit("Effect_2D");
	//
	//	NewEffect->GetTransform()->SetLocalPosition(float4(300, 300, 0));
	//	NewEffect->GetTransform()->SetLocalScale(float4(300, 300, 100));
	//	NewEffect->SetTexture("DiffuseTexture", "Effect_Texture_03.tga");
	//	NewEffect->SetDistortionTexture("DistortionSample_02.jpg");
	//}


	CreateActor<ShaderTestActor>()->GetTransform()->SetWorldPosition(float4(0, 400, 0));

	//PointLight = CreatePointLight(float4(0, 300, 0), ShadowTextureScale::S_512, 1024);
	//PointLight->SetLightPower(2.0f);
	//PointLight->DynamicShadowOn();

	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "Forward Target", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "Deferred Target", GetMainCamera()->GetCamDeferrdTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "Light Target", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "Alpha Target", GetMainCamera()->GetCamAlphaTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(4, "Last Target", GetMainCamera()->GetCamTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(5, "Bake Shadow", GetDirectionalLight()->GetBakeTarget(0));
	GameEngineCoreWindow::AddDebugRenderTarget(6, "Last Shadow", GetDirectionalLight()->GetShadowTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(7, "Bloom Debug A", Bloom->DebugTargetA);
	//GameEngineCoreWindow::AddDebugRenderTarget(8, "Bloom Debug B", Bloom->DebugTargetB);
}
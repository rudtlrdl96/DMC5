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
#include "MotionBlurEffect.h"
#include "FadeEffect.h"

#include "Plane.h"
#include "TestObject.h"
#include "EventZone.h"
#include "RedSeal.h"
#include "EnemySpawner.h"
#include "Player_MirageBlade.h"
#include "Enemy_HellCaina.h"
#include "Enemy_Empusa.h"
#include "Enemy_Qliphoth.h"

#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "Item_RedOrb.h"
#include "ShaderTestActor.h"
#include "FreeCameraActor.h"
#include "ReflectionProbe.h"
#include "BWColorEffect.h"
#include "PlayerCamera.h"
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
	BaseLevel::SetNetLevelType(Net_LevelType::TestStageLevel);

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
	
	{
		std::shared_ptr<FadeEffect> Fade = GetCamera(0)->GetCamTarget()->CreateEffect<FadeEffect>();
	}

	{
		std::shared_ptr<MotionBlurEffect> Blur = GetCamera(0)->GetCamTarget()->CreateEffect<MotionBlurEffect>();
		Blur->SetCamPosTarget(GetMainCamera()->GetCamPosTarget());
		Blur->SetCamMaskTarget(GetMainCamera()->GetCamMaskTarget());
		Blur->SetCam(GetMainCamera());
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
	// 테스트용 코드
	GetDirectionalLight()->GetTransform()->SetWorldPosition(float4(0, 5000, 0));
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 45.f, 45.f, 45.f });
	GetDirectionalLight()->SetLightPower(1.f);
	GetDirectionalLight()->SetLightColor({ 0.5f,0.5,1.f });

	//임시 불러오기, 추후 수정 예정
	StageBaseLevel::LevelChangeStart();

	SetCamera({ 0,0,-500 });

	BGMPlayer::BGMLoad();
	if (true)
	{
		MyPlayer = CreateActor<PlayerActor_Nero>();
		MyPlayer->SetUserControllType();
		MyPlayer->SetWorldPosition({ 8615.f, 0.f, 5060.f });
		MyPlayer->GetTransform()->SetWorldPosition({ 8615.f, 0.f, 5060.f });
		NetworkManager::LinkNetwork(MyPlayer.get(), this);
	}
	else
	{
		MyPlayer = CreateActor<PlayerActor_Vergil>();
		MyPlayer->SetUserControllType();
		MyPlayer->SetWorldPosition({ 8615.f, 0, 5060.f });
		NetworkManager::LinkNetwork(MyPlayer.get(), this);
	}
	CreateStage(Location2_StageDatas[0]);

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


	//CreateActor<ShaderTestActor>()->GetTransform()->SetWorldPosition(float4(0, 400, 0));

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
	GameEngineCoreWindow::AddDebugRenderTarget(7, "Mask Target", GetMainCamera()->GetCamMaskTarget());
	//GameEngineCoreWindow::AddDebugRenderTarget(7, "Bloom Debug A", Bloom->DebugTargetA);
	//GameEngineCoreWindow::AddDebugRenderTarget(8, "Bloom Debug B", Bloom->DebugTargetB);

	InitPool();
	CreateEventZone();
}

void TestStageLevel::InitPool()
{
	//Passive컨트롤 용 네로 오브젝트 풀링
	Poolable<PlayerActor_Nero>::CreatePool(this, static_cast<int>(ActorOrder::Player), 1,
		[](std::shared_ptr<PlayerActor_Nero> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});

	//Passive컨트롤 용 버질 오브젝트 풀링
	Poolable<PlayerActor_Vergil>::CreatePool(this, static_cast<int>(ActorOrder::Player), 1,
		[](std::shared_ptr<PlayerActor_Vergil> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});

	//Passive컨트롤 용 미자리 블레이드 오브젝트 풀링
	Poolable<Player_MirageBlade>::CreatePool(this, static_cast<int>(ActorOrder::Player), 8);

	//Enemy_HellCaina
	Poolable<Enemy_HellCaina>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 6,
		[this](std::shared_ptr<Enemy_HellCaina> _ActorPtr)
	{
		if (true == NetworkManager::IsClient())
		{
			_ActorPtr->SetControll(NetControllType::PassiveControll);
		}
		else
		{
			//_ActorPtr->SetControll(NetControllType::ActiveControll);
			NetworkManager::LinkNetwork(_ActorPtr.get(), this);
		}
	});

	//Enemy_Empusa
	Poolable<Enemy_Empusa>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 7,
		[this](std::shared_ptr<Enemy_Empusa> _ActorPtr)
	{
		if (true == NetworkManager::IsClient())
		{
			_ActorPtr->SetControll(NetControllType::PassiveControll);
		}
		else
		{
			//_ActorPtr->SetControll(NetControllType::ActiveControll);
			NetworkManager::LinkNetwork(_ActorPtr.get(), this);
		}
	});

	//Enemy_Qliphoth
	Poolable<Enemy_Qliphoth>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 2,
		[this](std::shared_ptr<Enemy_Qliphoth> _ActorPtr)
		{
			if (true == NetworkManager::IsClient())
			{
				_ActorPtr->SetControll(NetControllType::PassiveControll);
			}
			else
			{
				NetworkManager::LinkNetwork(_ActorPtr.get(), this);
			}
		});
}

void TestStageLevel::CreateEventZone()
{
	std::shared_ptr<EventZone> NewEvent = CreateActor<EventZone>();
	NewEvent->SetName("ClipothEventZone");
	NewEvent->GetTransform()->SetWorldPosition({ 4000, 275, -6122 });
	NewEvent->GetTransform()->SetWorldScale({ 2500, 500, 3000 });
	NewEvent->SetEvent([=]
		{
			GameEngineSound::Play("Qliphoth_6.wav");
			PlayerCamera::Shake(5.0f, 2.5f, 0.1f, 0.8f);
			std::vector<float4> EnemyPos =
			{
				{ 4424, 105, -4619 }, { 3458, 84, -5820 }
			};
			std::vector<float> EnemyRot = { -99, 20 };
			for (int i = 0; i < 2; i++)
			{
				TimeEvent.AddEvent(i * 0.8f, [=](GameEngineTimeEvent::TimeEvent& Event, GameEngineTimeEvent* Manager)
				{
					std::shared_ptr<Enemy_Qliphoth> Empusa = Poolable<Enemy_Qliphoth>::PopFromPool(this, static_cast<int>(ActorOrder::Enemy));
					Empusa->GetTransform()->SetWorldPosition(EnemyPos[i]);
					Empusa->GetTransform()->SetWorldRotation(float4::UP * EnemyRot[i]);
				});
			}
			NewEvent->Off();
		});
}

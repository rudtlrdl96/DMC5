#include "PrecompileHeader.h"
#include "Location2_Level.h"
#include "NetworkManager.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "FreeCameraActor.h"
#include "EnemySpawner.h"

#include "JudgementCut.h"
#include "ColorEffect.h"
#include "FXAA_Effect.h"
#include "ZoomEffect.h"
#include "DistortionEffect.h"

#include "Plane.h"
#include "TestObject.h"
#include "PlayerActor_Nero.h"
#include "PlayerActor_Vergil.h"
#include "Player_MirageBlade.h"
#include "Enemy_HellCaina.h"
#include "Enemy_Empusa.h"

#include "ShaderTestActor.h"
#include "FreeCameraActor.h"
#include "ReflectionProbe.h"

#include <GameEngineCore/GameEngineGUI.h>
#include <GameEngineCore/BloomEffect.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "Location2_EnemySpawner0.h"

Location2_Level* Location2_Level::Inst = nullptr;

Location2_Level::Location2_Level()
{
	Inst = this;
}

Location2_Level::~Location2_Level()
{

}

void Location2_Level::Start()
{
	BaseLevel::Start();
	BaseLevel::SetNetLevelType(Net_LevelType::Location2);

	CreateActor<FreeCameraActor>();

	GetCamera(0)->GetCamTarget()->CreateEffect<ColorEffect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();

	GetCamera(0)->GetDeferredLightTarget()->CreateEffect<BloomEffect>();

	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	GetCamera(0)->GetCamTarget()->CreateEffect<ZoomEffect>();

	{
		std::shared_ptr<DistortionEffect> Distortion = GetCamera(0)->GetCamTarget()->CreateEffect<DistortionEffect>();
		Distortion->SetMaskTexture(GetCamera(0)->GetCamAlphaTarget(), 2);
		Distortion->SetDistortionValue(10, 10);
		Distortion->SetMaxPixelValue(100, 100);
	}

	StageBaseLevel::Start();

	if (false == GameEngineInput::IsKey("BakeTestKey"))
	{
		GameEngineInput::CreateKey("BakeTestKey", VK_SPACE);
	}

	//FightEvent 생성
	FightEvents.push_back([this]()
		{
			std::shared_ptr<EnemySpawner> Spawner = GetLevel()->CreateActor<Location2_EnemySpawner0>();
			RedSealWallOn();
		});

	FightEvents.push_back([this]()
		{
			//std::shared_ptr<EnemySpawner> Spawner = GetLevel()->CreateActor<Location2_EnemySpawner1>();
			RedSealWallOn();
		});

	GetDirectionalLight()->GetTransform()->SetWorldPosition(float4(0, 10000, 0));
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 45.f,45.f,45.f });
	GetDirectionalLight()->SetLightPower(1.f);
	GetDirectionalLight()->SetLightColor({ 0.5f,0.5,1.f });
}

void Location2_Level::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);
}

void Location2_Level::LevelChangeStart()
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

	if (false)
	{
		std::shared_ptr<PlayerActor_Nero> Nero = CreateActor<PlayerActor_Nero>();
		Nero->SetUserControllType();
		Nero->SetWorldPosition(PlayerStartPos);
	}
	else
	{
		std::shared_ptr<PlayerActor_Vergil> Vergil = CreateActor<PlayerActor_Vergil>();
		Vergil->SetUserControllType();
		Vergil->SetWorldPosition(PlayerStartPos);
	}

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

	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "Forward Target", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "Deferred Target", GetMainCamera()->GetCamDeferrdTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "Light Target", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "Alpha Target", GetMainCamera()->GetCamAlphaTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(4, "Last Target", GetMainCamera()->GetCamTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(5, "Bake Shadow", GetDirectionalLight()->GetBakeTarget(0));
	GameEngineCoreWindow::AddDebugRenderTarget(6, "Last Shadow", GetDirectionalLight()->GetShadowTarget());

	InitPool();
}


void Location2_Level::InitPool()
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
	Poolable<Enemy_HellCaina>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 3,
		[](std::shared_ptr<Enemy_HellCaina> _ActorPtr)
	{
		if (true == NetworkManager::IsClient())
		{
			_ActorPtr->SetControll(NetControllType::PassiveControll);
		}
		else
		{
			_ActorPtr->SetControll(NetControllType::ActiveControll);
		}
	});

	//Enemy_Empusa
	Poolable<Enemy_Empusa>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 5,
		[](std::shared_ptr<Enemy_Empusa> _ActorPtr)
	{
		if (true == NetworkManager::IsClient())
		{
			_ActorPtr->SetControll(NetControllType::PassiveControll);
		}
		else
		{
			_ActorPtr->SetControll(NetControllType::ActiveControll);
		}
	});
}

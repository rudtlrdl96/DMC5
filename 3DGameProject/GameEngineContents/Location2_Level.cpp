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
		Distortion->SetMaskTexture(GetCamera(0)->GetCamAlphaTarget(), 1);
		Distortion->SetDistortionValue(10, 10);
		Distortion->SetMaxPixelValue(100, 100);
	}

	StageBaseLevel::Start();

	if (false == GameEngineInput::IsKey("BakeTestKey"))
	{
		GameEngineInput::CreateKey("BakeTestKey", VK_SPACE);
	}

	//FightEvent ����
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

	GetDirectionalLight()->GetTransform()->SetWorldPosition(float4(0, 5000, 0));
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 90.f,0.f,0.f });
	GetDirectionalLight()->SetLightPower(0.25f);
	GetDirectionalLight()->SetLightColor({ 0.85f,0.85f,1.f });
}

void Location2_Level::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);
}

void Location2_Level::LevelChangeStart()
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
	//Passive��Ʈ�� �� �׷� ������Ʈ Ǯ��
	Poolable<PlayerActor_Nero>::CreatePool(this, static_cast<int>(ActorOrder::Player), 1,
		[](std::shared_ptr<PlayerActor_Nero> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});

	//Passive��Ʈ�� �� ���� ������Ʈ Ǯ��
	Poolable<PlayerActor_Vergil>::CreatePool(this, static_cast<int>(ActorOrder::Player), 1,
		[](std::shared_ptr<PlayerActor_Vergil> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});

	//Passive��Ʈ�� �� ���ڸ� ���̵� ������Ʈ Ǯ��
	Poolable<Player_MirageBlade>::CreatePool(this, static_cast<int>(ActorOrder::Player), 8);

	//Monster_��ī�̳�
	Poolable<Enemy_HellCaina>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 3,
		[](std::shared_ptr<Enemy_HellCaina> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});

	//Monster_��ī�̳�
	Poolable<Enemy_Empusa>::CreatePool(this, static_cast<int>(ActorOrder::Enemy), 5,
		[](std::shared_ptr<Enemy_Empusa> _ActorPtr)
	{
		_ActorPtr->SetControll(NetControllType::PassiveControll);
	});
}

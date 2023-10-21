#include "PrecompileHeader.h"
#include "RedSeal.h"
#include "EffectRenderer.h"
#include "StageBaseLevel.h"
#include "FXSystem.h"
#include <GameEngineCore/PhysXBoxComponent.h>

bool IsEditLevel = false;

RedSeal::RedSeal()
{

}

RedSeal::~RedSeal()
{

}

void RedSeal::WallOn()
{
	State = RedSealState::On;
}

void RedSeal::WallOff()
{
	State = RedSealState::Off;
}

void RedSeal::Start()
{
	IsEditLevel = GetLevel()->DynamicThis<StageBaseLevel>()->IsEditLevel;
	if (!IsEditLevel)
	{
		PhysXBox = CreateComponent<PhysXBoxComponent>();
		physx::PxVec3 PxScale = { 3000.f, 3000.f, 100.f };
		PhysXBox->SetObstacleObject();
		PhysXBox->SetPositionSetFromParentFlag(true);
		PhysXBox->CreatePhysXActors(PxScale);

		GetLevel()->DynamicThis<StageBaseLevel>()->PushBackRedSealWall(DynamicThis<RedSeal>());

		EffectSystem = CreateComponent<FXSystem>();

		if (FXData::Find("RedSealEffect_Start.effect") == nullptr)
		{
			GameEngineDirectory NewDir = GameEngineDirectory();
			NewDir.MoveParentToDirectory("ContentResources");
			NewDir.Move("ContentResources");
			NewDir.Move("Effect");
			NewDir.Move("Map");

			std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".effect" });
			for (GameEngineFile File : Files)
			{
				if (nullptr == FXData::Find(File.GetFileName()))
				{
					FXData::Load(File.GetFullPath());
				}
			}
		}

		EffectSystem->CreateFX(FXData::Find("RedSealEffect_Start.effect"));
		EffectSystem->CreateFX(FXData::Find("RedSealEffect_End.effect"));
		EffectSystem->SetDistortionTextureName("");

		OffStateInit();
		OnStateInit();

		RedSealFSM.ChangeState(static_cast<int>(State));
	}
	else
	{
		//FBXMesh = CreateComponent<GameEngineFBXRenderer>();
		//FBXMesh->SetFBXMesh("sm7001_seal_02.fbx", "FBX");
	}
	
}

void RedSeal::Update(float _DeltaTime)
{
	if (IsEditLevel)
	{
		return;
	}
	RedSealFSM.Update(_DeltaTime);
}

void RedSeal::OffStateInit()
{
	RedSealFSM.CreateState(
		{
			.StateValue = static_cast<int>(RedSealState::Off),
			.Start = [this]()
			{
				EffectSystem->ChangeFX("RedSealEffect_End.effect");
				EffectSystem->Play();
				PhysXBox->Off();
			},
			.Update = [this](float _DeltaTime)
			{
				if (State == RedSealState::On)
				{
					RedSealFSM.ChangeState(static_cast<int>(State));
				}
			},
			.End = [this]()
			{
			}
		});
}

void RedSeal::OnStateInit()
{
	RedSealFSM.CreateState(
		{
			.StateValue = static_cast<int>(RedSealState::On),
			.Start = [this]()
			{
				EffectSystem->ChangeFX("RedSealEffect_Start.effect");
				EffectSystem->Play();
				PhysXBox->On();
		},
			.Update = [this](float _DeltaTime)
			{
				if (State == RedSealState::Off)
				{
					RedSealFSM.ChangeState(static_cast<int>(State));
				}
			},
			.End = [this]()
			{
			}
		});
}


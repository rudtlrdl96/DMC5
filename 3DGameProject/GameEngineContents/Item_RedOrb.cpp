#include "PrecompileHeader.h"
#include "Item_RedOrb.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "EffectRenderer.h"
#include "FXSystem.h"
#include "PlayerHPUI.h"
#include "RedOrbUI.h"
Item_RedOrb::Item_RedOrb()
{
}

Item_RedOrb::~Item_RedOrb()
{
}


void Item_RedOrb::SetWait()
{
	FSM.ChangeState(RedOrb_Wait);
}

void Item_RedOrb::Take(GameEngineTransform* _Transform)
{
	Col->Off();
	Player = _Transform;
	IsTake = true;
	//Col->Off();
	//FBXMesh->Off();
	//Effect->PlayFX("RedOrb_Disappear.effect");

	//GetLevel()->TimeEvent.AddEvent(0.5f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager) {
	//	Off();
	//	FBXMesh->On();
	//	Col->On();
	//	});
}

void Item_RedOrb::Start()
{
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Map");
		NewDir.Move("RedOrb");

		if (nullptr == GameEngineFBXMesh::Find("orb000_0_low.meshout.FBX"))
		{
			GameEngineFBXMesh::Load(NewDir.GetPlusFileName("orb000_0_low.meshout.FBX").GetFullPath());
		}
	}

	Effect = CreateComponent<FXSystem>();
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Effect");
		NewDir.Move("RedOrb");

		if (nullptr == FXData::Find("RedOrb_Disappear.effect"))
		{
			FXData::Load(NewDir.GetPlusFileName("RedOrb_Disappear.effect").GetFullPath());
		}
		Effect->CreateFX(FXData::Find("RedOrb_Disappear.effect"));
	}

	FBXMesh = CreateComponent<GameEngineFBXRenderer>();
	FBXMesh->SetFBXMesh("orb000_0_low.meshout.FBX", "FBX");

	Col = CreateComponent<GameEngineCollision>(CollisionOrder::RedOrb);
	Col->GetTransform()->SetLocalScale({ 800, 800, 800 });

	FSM.CreateState({ .StateValue = RedOrb_Wait,
		.Start = [=]
		{
			FBXMesh->GetTransform()->SetLocalScale({0.0f, 0.0f, 0.0f});
			Off();
		},
		.Update = [=](float _DeltaTime)
		{
			if (true == IsUpdate())
			{
				FSM.ChangeState(RedOrb_Bursts);
			}
		},
		.End = [=]
		{
		}
		});

	FSM.CreateState({ .StateValue = RedOrb_Bursts,
	.Start = [=]
	{
		Col->On();
		FBXMesh->On();
		Effect->PlayFX("RedOrb_Disappear.effect");
		float Random = GameEngineRandom::MainRandom.RandomFloat(0.3f, 1);
		Force = GetTransform()->GetLocalForwardVector() * 1000 * Random;
		GetLevel()->RayCast(GetTransform()->GetWorldPosition(), float4::DOWN, GroundPos);
		FloorHeight = GroundPos.y + 5;
		Gravity = 350;

		ResetLiveTime();
	},
	.Update = [=](float _DeltaTime)
	{
		FBXMesh->GetTransform()->SetLocalScale(float4::ONE * std::clamp(GetLiveTime() * 7.0f, 0.0f, 3.0f));

		Force = float4::LerpClamp(Force, float4::ZERO, 0.05f);

		if (GetTransform()->GetLocalPosition().y < FloorHeight)
		{
			Gravity = 0;
			float4 Pos = GetTransform()->GetLocalPosition();
			Pos.y = FloorHeight;
			GetTransform()->SetLocalPosition(Pos);
			FSM.ChangeState(RedOrb_Bound);
			return;
		}
		Gravity = std::max<float>(-500, Gravity - 800 * _DeltaTime);
		GetTransform()->AddLocalPosition((Force + float4::UP * Gravity) * _DeltaTime);
		GetTransform()->AddLocalRotation(float4::UP * 60 * _DeltaTime);
		},
	.End = [=]
	{
		FBXMesh->GetTransform()->SetLocalScale(float4::ONE * 3);
	}
		});


	FSM.CreateState({ .StateValue = RedOrb_Bound,
	.Start = [=]
	{
		Gravity = 50;
		BoundCount = 0;
	},
	.Update = [=](float _DeltaTime)
	{
		if (true == IsTake)
		{
			FSM.ChangeState(RedOrb_SuckIn);
		}
		Force = float4::LerpClamp(Force, float4::ZERO, 0.05f);

		if (GetTransform()->GetLocalPosition().y < FloorHeight)
		{
			if (5 < ++BoundCount)
			{
				FSM.ChangeState(RedOrb_Idle);
				GetTransform()->AddLocalPosition(float4::DOWN* GetTransform()->GetLocalPosition().y);
				return;
			}
			Gravity = 50;
			float4 Pos = GetTransform()->GetLocalPosition();
			Pos.y = FloorHeight;
			GetTransform()->SetLocalPosition(Pos);
		}
		Gravity = std::max<float>(-500, Gravity - 300 * _DeltaTime);
		GetTransform()->AddLocalPosition((Force + float4::UP * Gravity) * _DeltaTime);
		GetTransform()->AddLocalRotation(float4::UP * 60 * _DeltaTime);
		},
	.End = [=]
	{
	}
		});

	FSM.CreateState({ .StateValue = RedOrb_Idle,
	.Start = [=]
	{
		FBXMesh->GetTransform()->SetLocalScale(float4::ONE * 3.0f);
	},
	.Update = [=](float _DeltaTime)
	{
		if (true == IsTake)
		{
			FSM.ChangeState(RedOrb_SuckIn);
		}
		GetTransform()->AddLocalRotation(float4::UP * 60 * _DeltaTime);
	},
	.End = [=]
	{
	}
	});

	FSM.CreateState({ .StateValue = RedOrb_SuckIn,
	.Start = [=]
	{
		StartPos = GetTransform()->GetWorldPosition();
		ResetLiveTime();
	},
	.Update = [=](float _DeltaTime)
	{
		if (1.0f < GetLiveTime() * 3.0f)
		{
			FSM.ChangeState(RedOrb_Disappear);
			return;
		}
		GetTransform()->SetWorldPosition(float4::LerpClamp (StartPos, Player->GetWorldPosition(), GetLiveTime() * 3.0f));
		GetTransform()->AddLocalRotation(float4::UP * 60 * _DeltaTime);
	},
	.End = [=]
	{
	}
	});
	
	FSM.CreateState({ .StateValue = RedOrb_Disappear,
	.Start = [=]
	{
		FBXMesh->Off();
		Effect->PlayFX("RedOrb_Disappear.effect");
		RedOrbUI::AddRedOrb(100);
		ResetLiveTime();
	},
	.Update = [=](float _DeltaTime)
	{
		if (1 < GetLiveTime())
		{
			Death();
		}
	},
	.End = [=]
	{
	}
	});

	FSM.ChangeState(RedOrb_Idle);
}

void Item_RedOrb::Update(float _DeltaTime)
{
	FSM.Update(_DeltaTime);
}
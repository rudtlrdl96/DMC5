#include "PrecompileHeader.h"
#include "PlayerActor_Vergil.h"
#include "NetworkManager.h"
#include "Player_MirageBlade.h"
#include "PlayerController.h"
bool PlayerActor_Vergil::Input_GunCheck()
{
	if (Controller->GetGunUp())
	{
		FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Shoot);
	}
	return false;
}

void PlayerActor_Vergil::CreateMirageBlade()
{
	AllMirageBlades.resize(8);
	for (int i = 0; i < 8; i++)
	{
		AllMirageBlades[i] = NetworkManager::CreateNetworkActor<Player_MirageBlade>(GetLevel());
		if (nullptr == AllMirageBlades[i])
		{
			AllMirageBlades[i] = GetLevel()->CreateActor<Player_MirageBlade>(ActorOrder::Player);
		}
		AllMirageBlades[i]->Off();
	}
	static int BladesIndex = 0;
	static GameEngineTimeEvent TimeEvent;

	FSM_MirageBlade.CreateState({ .StateValue = FSM_State_MirageBlade::MirageBlade_Idle,
	.Start = [=] {

	},
	.Update = [=](float _DeltaTime) {
		if (true == Input_GunCheck()) { return; }
		TimeEvent.Update(_DeltaTime);
	},
	.End = [=] {

	}
		});

	static bool IsDelay = false;
	FSM_MirageBlade.CreateState({ .StateValue = FSM_State_MirageBlade::MirageBlade_Shoot,
	.Start = [=] {
		IsDelay = true;
		std::shared_ptr<Player_MirageBlade> CurBlade = AllMirageBlades[BladesIndex];
		CurBlade->Reset();
		CurBlade->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
		CurBlade->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());


		CurBlade->On();
		CurBlade->Shoot();
		if (BladesIndex % 2 == 0)
		{
			CurBlade->GetTransform()->AddWorldPosition(GetTransform()->GetWorldLeftVector() * 75 + GetTransform()->GetWorldBackVector() * 50 + float4::UP * 100);
		}
		else
		{
			CurBlade->GetTransform()->AddWorldPosition(GetTransform()->GetWorldRightVector() * 75 + GetTransform()->GetWorldBackVector() * 50 + float4::UP * 100);
		}
		if (AllMirageBlades.size() == ++BladesIndex)
		{
			BladesIndex = 0;
		}
		TimeEvent.AddEvent(0.1f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
			{
				IsDelay = false;
			});

		if (nullptr != LockOnEnemyTransform)
		{
			CurBlade->SetTarget(LockOnEnemyTransform);
			CurBlade->LookTarget();
		}


	},
	.Update = [=](float _DeltaTime) {
		TimeEvent.Update(_DeltaTime);
		if (false == IsDelay)
		{
			FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Idle);
		}
		},
	.End = [=] {

	}
		});

	FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Idle);
}
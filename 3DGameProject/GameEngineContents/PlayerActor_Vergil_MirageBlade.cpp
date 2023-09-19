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
		AllMirageBlades[i] = NetworkManager::CreateNetworkActor<Player_MirageBlade>(GetLevel(), ActorOrder::Player);
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
			float4 LocalForward = CurBlade->GetTransform()->GetWorldForwardVector();
			LocalForward.y = 0;
			LocalForward.Normalize();
			float4 LookDir = LockOnEnemyTransform->GetWorldPosition() - CurBlade->GetTransform()->GetWorldPosition();
			LookDir.y = 0;
			LookDir.Normalize();
			float Dot = float4::DotProduct3D(LocalForward, LookDir);
			float Angle = acosf(Dot) * GameEngineMath::RadToDeg;
			float4 Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);
			if (Cross.y < 0.0f)
			{
				CurBlade->GetTransform()->AddWorldRotation({ 0, -Angle, 0 });
			}
			else
			{
 				CurBlade->GetTransform()->AddWorldRotation({ 0, Angle, 0 });
			}

			LocalForward = CurBlade->GetTransform()->GetWorldForwardVector();
			LocalForward.Normalize();
			LookDir = LockOnEnemyTransform->GetWorldPosition() - CurBlade->GetTransform()->GetWorldPosition();
			LookDir.y += 100;
			LookDir.Normalize();

			Dot = float4::DotProduct3D(LocalForward, LookDir);
			Angle = acosf(Dot) * GameEngineMath::RadToDeg;
			Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);
			if (Cross.y < 0.0f)
			{
				CurBlade->GetTransform()->AddWorldRotation({ Angle, 0, 0 });
			}
			else
			{
				CurBlade->GetTransform()->AddWorldRotation({ Angle, 0, 0 });
			}

			return;
			/*float4 LocalForward = CurBlade->GetTransform()->GetWorldForwardVector().NormalizeReturn();
			float4 LookDir = LockOnEnemyTransform->GetWorldPosition() - CurBlade->GetTransform()->GetWorldPosition();
			LookDir.Normalize();
			float Dot = float4::DotProduct3D(LocalForward, LookDir);
			if (1.0f < Dot || LocalForward == LookDir)
			{
				return;
			}
			if (Dot < -1.0f)
			{
				CurBlade->GetTransform()->AddWorldRotation({180, 0, 0});
				return;
			}
			float Angle = acosf(Dot) * GameEngineMath::RadToDeg;
			float4 Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);

			if (std::isnan(Angle))
			{
				return;
			}
			CurBlade->GetTransform()->AddWorldRotation({ Angle, 0, 0 });
			return;
			if (Cross.x < 0.0f)
			{
				CurBlade->GetTransform()->AddWorldRotation({ Angle, 0, 0 });
			}
			else
			{
				CurBlade->GetTransform()->AddWorldRotation({ -Angle, 0, 0 });
			}
			return;*/

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
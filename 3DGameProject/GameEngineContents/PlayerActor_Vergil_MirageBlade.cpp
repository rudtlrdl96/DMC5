#include "PrecompileHeader.h"
#include "PlayerActor_Vergil.h"
#include "NetworkManager.h"
#include "Player_MirageBlade.h"
#include "PlayerController.h"
#include "AttackCollision.h"
#include "PlayerHPUI.h"
#include "UI_DTGauge.h"
void PlayerActor_Vergil::CreateMirageBlade()
{
	GameEngineTransform* SpiralPivot = CreateComponent<GameEngineComponent>()->GetTransform();
	SpiralPivot->SetWorldRotation(float4::ZERO);
	AllMirageBlades.resize(8);
	for (int i = 0; i < 8; i++)
	{
		AllMirageBlades[i] = GetLevel()->CreateActor<Player_MirageBlade>(ActorOrder::Player);
		AllMirageBlades[i]->GetTransform()->SetParent(SpiralPivot);
		AllMirageBlades[i]->Off();
		NetworkManager::LinkNetwork(AllMirageBlades[i].get());
	}
	static int BladesIndex = 0;

	// Idle
	FSM_MirageBlade.CreateState({ .StateValue = FSM_State_MirageBlade::MirageBlade_Idle,
	.Start = [=] {

	},
	.Update = [=](float _DeltaTime) {
		if (true == Controller->GetIsGunCharge() && true == IsSpiralBlade && 1.0f < DTGauge)
		{
			DTGauge -= 1.0f;
			HUD->GetDtUI()->ActivateDtUI(DTGauge);
			FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Spiral);
			return;
		}
		if (true == Controller->GetIsBackFrontGun() && nullptr != LockOnEnemyTransform && true == IsHeavyRainBlade && 1.0f < DTGauge)
		{
			DTGauge -= 1.0f;
			HUD->GetDtUI()->ActivateDtUI(DTGauge);
			FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_HeavyRain);
			return;
		}
		if (true == Controller->GetIsFrontGun() && nullptr != LockOnEnemyTransform && true == IsLesteringBlade && 1.0f < DTGauge)
		{
			DTGauge -= 1.0f;
			HUD->GetDtUI()->ActivateDtUI(DTGauge);
			FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Blistering);
			return;
		}
		if (true == Controller->GetIsBackGun() && nullptr != LockOnEnemyTransform && true == IsStormBlade && 1.0f < DTGauge)
		{
			DTGauge -= 1.0f;
			HUD->GetDtUI()->ActivateDtUI(DTGauge);
			FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Storm);
			return;
		}
		if (true == Controller->GetIsGunUp())
		{
			FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Shoot);
			return;
		}
	},
	.End = [=] {

	}
		});

	static bool IsDelay = false;

	// Shoot
	FSM_MirageBlade.CreateState({ .StateValue = FSM_State_MirageBlade::MirageBlade_Shoot,
	.Start = [=] {
		Sound.Play("Mirage_", 1);
		IsDelay = true;
		std::shared_ptr<Player_MirageBlade> CurBlade = AllMirageBlades[BladesIndex];
		CurBlade->Reset();
		CurBlade->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
		CurBlade->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());
		CurBlade->On();


		// 블레이드 인덱스값이 짝수 홀수에 따라 좌우 위치 변경
		if (BladesIndex % 2 == 0)
		{
			CurBlade->GetTransform()->AddWorldPosition(GetTransform()->GetWorldLeftVector() * 75 + GetTransform()->GetWorldBackVector() * 50 + float4::UP * 100);
		}
		else
		{
			CurBlade->GetTransform()->AddWorldPosition(GetTransform()->GetWorldRightVector() * 75 + GetTransform()->GetWorldBackVector() * 50 + float4::UP * 100);
		}
		// 인덱스 증가.
		if (AllMirageBlades.size() == ++BladesIndex)
		{
			BladesIndex = 0;
		}

		// 공격 딜레이
		TimeEvent.AddEvent(0.1f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
			{
				IsDelay = false;
			});

		// 록온한 적이 있다면 타겟 지정
		if (nullptr != LockOnEnemyTransform)
		{
			CurBlade->SetTarget(LockOnEnemyTransform);
			CurBlade->LookTarget();
		}
		CurBlade->Shoot(9, DamageType::VergilLight);

	},
	.Update = [=](float _DeltaTime) {
		if (false == IsDelay)
		{
			FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Idle);
		}
		},
	.End = [=] {

	}
		});

	// Spiral
	FSM_MirageBlade.CreateState({ .StateValue = FSM_State_MirageBlade::MirageBlade_Spiral,
	.Start = [=] {
		Sound.Play("Mirage_", 3);
		SpiralPivot->SetLocalPosition(float4::ZERO);
		SpiralPivot->SetWorldRotation(float4::ZERO);
		for (int i = 0; i < 8; i++)
		{
			AllMirageBlades[i]->Reset();
			AllMirageBlades[i]->On();
			AllMirageBlades[i]->GetTransform()->SetLocalRotation({0, 45.0f * i, 0});
			AllMirageBlades[i]->GetTransform()->SetLocalPosition({0, 50, 0});
			AllMirageBlades[i]->GetTransform()->AddLocalPosition(AllMirageBlades[i]->GetTransform()->GetWorldForwardVector() * 150);
			AllMirageBlades[i]->SetSpiral();
		}
		AllMirageBlades[0]->Collision->GetTransform()->SetParent(GetTransform());
		AllMirageBlades[0]->Collision->GetTransform()->SetLocalPosition({0, 50, 0});
		AllMirageBlades[0]->Collision->GetTransform()->SetLocalRotation(float4::ZERO);
		AllMirageBlades[0]->Collision->GetTransform()->SetLocalScale({300, 50, 300});
		AllMirageBlades[0]->Collision->On();
		AllMirageBlades[0]->Collision->SetAttackData(DamageType::VergilLight, 9, [=]
			{
				AllMirageBlades[0]->Collision->Off();
				TimeEvent.AddEvent(0.2f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
					{
						if (true == AllMirageBlades[0]->IsSpiralStop) { return; }
						AllMirageBlades[0]->Collision->On();
					});
			});
	},
	.Update = [=](float _DeltaTime) {
		AllMirageBlades[0]->Collision->GetTransform()->AddLocalRotation(float4::UP * 360 * _DeltaTime);
		SpiralPivot->AddWorldRotation(float4::UP * 360 * _DeltaTime);
		if (Controller->GetIsGunDown())
		{
			FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_SpiralShoot);
		}
	},
	.End = [=] {

	}
		});

	// Spiral Shoot
	FSM_MirageBlade.CreateState({ .StateValue = FSM_State_MirageBlade::MirageBlade_SpiralShoot,
	.Start = [=] {
		Sound.Play("Mirage_", 0);
		IsDelay = false;
		AllMirageBlades[0]->Collision->GetTransform()->SetParent(AllMirageBlades[0]->GetTransform());
		AllMirageBlades[0]->Collision->GetTransform()->SetLocalPosition({ 0, 0, -50 });
		AllMirageBlades[0]->Collision->GetTransform()->SetLocalRotation(float4::ZERO);
		AllMirageBlades[0]->Collision->GetTransform()->SetLocalScale({ 25, 25, 150 });
		for (int i = 0; i < 8; i++)
		{
			AllMirageBlades[i]->SpiralStop();
		}
	},
	.Update = [=](float _DeltaTime) {
		if (true == IsDelay)
		{
			return;
		}
		if (Controller->GetIsGunUp())
		{
			Sound.Play("Mirage_", 2);
			for (int i = 0; i < 8; i++)
			{
				AllMirageBlades[i]->GetTransform()->SetWorldPosition(AllMirageBlades[i]->GetTransform()->GetWorldPosition());
				AllMirageBlades[i]->GetTransform()->SetWorldRotation(AllMirageBlades[i]->GetTransform()->GetWorldRotation());
				AllMirageBlades[i]->Shoot(20, DamageType::VergilLight);
			}
			TimeEvent.AddEvent(1.0f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
			{
				FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Idle);
			});
			IsDelay = true;
		}
	},
	.End = [=] {

	}
		});

	// Storm
	FSM_MirageBlade.CreateState({ .StateValue = FSM_State_MirageBlade::MirageBlade_Storm,
	.Start = [=] {
		Sound.Play("Mirage_", 3);
		IsDelay = false;
		SpiralPivot->SetWorldPosition(LockOnEnemyTransform->GetWorldPosition());
		SpiralPivot->SetWorldRotation(float4::ZERO);
		for (int i = 0; i < 8; i++)
		{
			AllMirageBlades[i]->Reset();
			AllMirageBlades[i]->On();
			AllMirageBlades[i]->GetTransform()->SetLocalRotation({0, 45.0f * i, 0});
			AllMirageBlades[i]->GetTransform()->SetLocalPosition({0, 50, 0});
			AllMirageBlades[i]->GetTransform()->AddLocalPosition(AllMirageBlades[i]->GetTransform()->GetWorldBackVector() * 150);
		}
	},
	.Update = [=](float _DeltaTime) {
		if (nullptr != LockOnEnemyTransform)
		{
			SpiralPivot->SetWorldPosition(LockOnEnemyTransform->GetWorldPosition());
		}
		if (true == IsDelay)
		{
			return;
		}
		SpiralPivot->AddWorldRotation(float4::UP * 360 * _DeltaTime);
		if (Controller->GetIsGunUp())
		{
			Sound.Play("Mirage_", 0);

			TimeEvent.AddEvent(0.4f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Sound.Play("Mirage_", 2);
					for (int i = 0; i < 8; i++)
					{
						AllMirageBlades[i]->GetTransform()->SetWorldPosition(AllMirageBlades[i]->GetTransform()->GetWorldPosition());
						AllMirageBlades[i]->GetTransform()->SetWorldRotation(AllMirageBlades[i]->GetTransform()->GetWorldRotation());
						AllMirageBlades[i]->Shoot(50, DamageType::Air);
					}
				});
			TimeEvent.AddEvent(1.0f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Idle);
				});
			IsDelay = true;
		}
	},
	.End = [=] {

	}
		});

	// Blistering
	static const std::vector<float4> BlisteringPos = {
		{ -35, 120, 0 },
		{ 35, 120, 0 },
		{ -40, 105, 0 },
		{ 40, 105, 0 },
		{ -45, 90, 0 },
		{ 45, 90, 0 },
		{ -50, 75, 0 },
		{ 50, 75, 0 },
	};
	FSM_MirageBlade.CreateState({ .StateValue = FSM_State_MirageBlade::MirageBlade_Blistering,
	.Start = [=] {
		Sound.Play("Mirage_", 3);
		IsDelay = false;
		SpiralPivot->SetLocalPosition(float4::ZERO);
		SpiralPivot->SetWorldRotation(float4::ZERO);
		for (int i = 0; i < 8; i++)
		{
			AllMirageBlades[i]->Reset();
			AllMirageBlades[i]->On();
			AllMirageBlades[i]->GetTransform()->SetLocalPosition(BlisteringPos[i]);
			AllMirageBlades[i]->SetTarget(LockOnEnemyTransform);
			AllMirageBlades[i]->LookTarget();
		}

	},
	.Update = [=](float _DeltaTime) {
		if (true == IsDelay)
		{
			for (int i = 0; i < 8; i++)
			{
				AllMirageBlades[i]->LookTarget();
			}
			return;
		}
		if (LockOnEnemyTransform != nullptr)
		{
			for (int i = 0; i < 8; i++)
			{
				AllMirageBlades[i]->SetTarget(LockOnEnemyTransform);
				AllMirageBlades[i]->LookTarget();
			}
			float4 LocalForward = SpiralPivot->GetWorldForwardVector();
			LocalForward.y = 0;
			LocalForward.Normalize();
			float4 LookDir = LockOnEnemyTransform->GetWorldPosition() - SpiralPivot->GetWorldPosition();
			LookDir.y = 0;
			LookDir.Normalize();
			float Dot = float4::DotProduct3D(LocalForward, LookDir);
			float Angle = acosf(Dot) * GameEngineMath::RadToDeg;
			float4 Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);
			if (false == std::isnan(Angle))
			{
				if (Cross.y < 0.0f)
				{
					SpiralPivot->AddWorldRotation({ 0, -Angle, 0 });
				}
				else
				{
					SpiralPivot->AddWorldRotation({ 0, Angle, 0 });
				}
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				AllMirageBlades[i]->SetTarget(nullptr);
				AllMirageBlades[i]->GetTransform()->SetWorldRotation(GetTransform()->GetWorldRotation());
			}
			SpiralPivot->SetWorldRotation(GetTransform()->GetWorldRotation());
		}
		if (Controller->GetIsGunUp())
		{
			for (int i = 0; i < 8; i++)
			{
				TimeEvent.AddEvent(0.05f * i, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					Sound.PlaySetVolume("Mirage_", 0, 0.5f);
					AllMirageBlades[i]->GetTransform()->SetWorldPosition(AllMirageBlades[i]->GetTransform()->GetWorldPosition());
					AllMirageBlades[i]->GetTransform()->SetWorldRotation(AllMirageBlades[i]->GetTransform()->GetWorldRotation());
					AllMirageBlades[i]->Shoot(94, DamageType::VergilLight);
				});

			}
			TimeEvent.AddEvent(1.5f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Idle);
				});
			IsDelay = true;
		}
		},
	.End = [=] {

	}
		});

	// HeavyRain
	static const std::vector<float4> HeavyRainPos = {
		{ 0, 652, 2 },
		{ 121, 524, 50 },
		{ -53, 600, -45 },
		{ 42, 576, -20 },
		{ -115, 632, 20 },
		{ 80, 680, 75 },
		{ -132, 550, -74 },
		{ 20, 610, -100 },
	};

	FSM_MirageBlade.CreateState({ .StateValue = FSM_State_MirageBlade::MirageBlade_HeavyRain,
	.Start = [=] {
		Sound.Play("Mirage_", 3);
		IsDelay = false;
		float4 EnemyPos = LockOnEnemyTransform->GetWorldPosition();
		for (int i = 0; i < 8; i++)
		{
			AllMirageBlades[i]->Reset();
			AllMirageBlades[i]->On();
			AllMirageBlades[i]->GetTransform()->SetWorldPosition(EnemyPos + HeavyRainPos[i]);
			AllMirageBlades[i]->GetTransform()->SetWorldRotation({90, 0, 0});
		}
	},
	.Update = [=](float _DeltaTime) {
		if (true == IsDelay)
		{
			return;
		}
		if (Controller->GetIsGunUp())
		{
			Sound.Play("Mirage_", 0);
			for (int i = 0; i < 8; i++)
			{
				TimeEvent.AddEvent(0.03f * i, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					AllMirageBlades[i]->GetTransform()->SetWorldPosition(AllMirageBlades[i]->GetTransform()->GetWorldPosition());
					AllMirageBlades[i]->GetTransform()->SetWorldRotation(AllMirageBlades[i]->GetTransform()->GetWorldRotation());
					AllMirageBlades[i]->Shoot(390, DamageType::VergilLight);
				});
			}
			TimeEvent.AddEvent(1.4f, [=](GameEngineTimeEvent::TimeEvent& _Event, GameEngineTimeEvent* _Manager)
				{
					FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Idle);
				});
			IsDelay = true;
		}
	},
	.End = [=] {

	}
		});
	FSM_MirageBlade.ChangeState(FSM_State_MirageBlade::MirageBlade_Idle);
}
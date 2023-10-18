#include "PrecompileHeader.h"
#include "CavaliereAngelo.h"

#include <cmath>

#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineBase/GameEngineRandom.h>

#include "BasePlayerActor.h"
#include "NetworkManager.h"
#include "AnimationEvent.h"
#include "AttackCollision.h"
#include "FXSystem.h"
#include "Cavaliere_Electric.h"

CavaliereAngelo::CavaliereAngelo() 
{
}

CavaliereAngelo::~CavaliereAngelo() 
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////   Actor Init   ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CavaliereAngelo::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::CavaliereAngelo;
	EnemyHP = 10000;
	HPServerStack = 0;
	HPClientStack = 0;
}

void CavaliereAngelo::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("em5501.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Character", "Enemy", "CavaliereAngelo", "mesh"
			},
			"em5501.FBX"
		);
		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("em5501.fbx", "AniFBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("em5501.fbx", "AniFBX");
	}
	break;
	default:
		break;
	}

	if (nullptr == GameEngineTexture::Find("PaperBurnNoise.jpg"))
	{
		GameEngineTexture::Load(GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Texture", "BurnNoiseTexture"
			}, "PaperBurnNoise.jpg"));
	}

	EnemyRenderer->SetTexture("PaperBurnTexture", "PaperBurnNoise.jpg");
}

void CavaliereAngelo::EnemyAnimationLoad()
{
	//Animation���� ��θ� ã�Ƽ� ���animation���� �ε�
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("CavaliereAngelo");
	NewDir.Move("Animation");

	AnimationEvent::LoadAll
	(
		{
			.Dir = NewDir.GetFullPath().c_str(),
			.Renderer = EnemyRenderer,
			.RendererLocalPos = { 0.0f, -45.0f, 0.0f },
			.Objects = {(GameEngineObject*)MonsterAttackCollision.get()},
			.CallBacks_void =
			{
			},
			.CallBacks_int =
			{
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)
			},
			.CallBacks_float4 =
			{
			}
		}
	);

	// ����Ʈ �ý��� ����
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Effect");
		NewDir.Move("CavalierAngelo");
		std::vector<GameEngineFile> FXFiles = NewDir.GetAllFile({ ".Effect" });

		EffectRenderer_0 = CreateComponent<FXSystem>();
		EffectRenderer_1 = CreateComponent<FXSystem>();
		for (size_t i = 0; i < FXFiles.size(); i++)
		{
			EffectRenderer_0->CreateFX(FXData::Load(FXFiles[i].GetFullPath()));
			EffectRenderer_1->CreateFX(FXData::Find(FXFiles[i].GetFileName()));
		}
	}
}

void CavaliereAngelo::Start()
{
	BaseEnemyActor::Start();

	//PhysX(�浹)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({ 100, 200, 100 });
	PhysXCapsule->GetDynamic()->setMass(200.0f);
	BindPhysicsWithNet(PhysXCapsule);

	// ������ ũ�� ����
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -45.0f, 0.0f });

	// �ݸ��� �ɼ�, ũ�� ����
	MonsterAttackCollision->SetAttackData(DamageType::Light, 0);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);
	MonsterCollision->GetTransform()->SetLocalScale({ 150, 300, 150 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 150, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 1000, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	ParryCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Null);
	ParryCollision->SetColType(ColType::OBBBOX3D);
	ParryCollision->GetTransform()->SetLocalScale({ 220, 220, 400 });
	ParryCollision->GetTransform()->SetLocalPosition({ 0, 150, 220 });
	//ParryCollision->GetTransform()->SetLocalScale({ 220, 220, 220 });
	//ParryCollision->GetTransform()->SetLocalPosition({ 0, 150, 240 });

	Electric = GetLevel()->CreateActor<Cavaliere_Electric>();
	Electric->GetTransform()->SetParent(GetTransform());
	Electric->Off();

	// �⺻ ����
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	MonsterAttackCollision->SetAttackData(DamageType::Heavy, 200);

	// �� ������Ʈ Ÿ�� ����
	SetNetObjectType(Net_ActorType::CavaliereAngelo);

	LinkData_UpdatePacket<bool>(IsCharge);
	LinkData_UpdatePacket<bool>(IsBurn);
	LinkData_UpdatePacket<bool>(IsStun);
	LinkData_UpdatePacket<int>(ChargeDamageStack);
	LinkData_UpdatePacket<int>(EnemyHP);
	LinkData_UpdatePacket<int>(HPServerStack);
	LinkData_UpdatePacket<int>(HPClientStack);

	SetDamagedNetCallBack<BasePlayerActor>([this](BasePlayerActor* _Attacker) {
		DamageData Datas = _Attacker->GetAttackCollision()->GetDamage();
		MinusEnemyHP(Datas.DamageValue);
		HPClientStackPlus(Datas.DamageValue);

		if (600 < HPClientStack)
		{
			HPServerStack = 0;
			HPClientStack = 0;
			Player = _Attacker;
		}

		if (DamageType::VergilLight == Datas.DamageTypeValue)
		{
			IsVergilLight = true;
		}

		if (DamageType::Buster == Datas.DamageTypeValue)
		{
			IsStun = false;
			SetTimeScale(0.4f);
			GetLevel()->TimeEvent.AddEvent(.316f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					StartRenderShaking(8);
				});
			GetLevel()->TimeEvent.AddEvent(.683f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					StartRenderShaking(8);
				});
			GetLevel()->TimeEvent.AddEvent(1.13f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					StartRenderShaking(8);
				});
			GetLevel()->TimeEvent.AddEvent(1.4f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					StartRenderShaking(8);
				});
			GetLevel()->TimeEvent.AddEvent(1.6f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					StartRenderShaking(8);
				});
			GetLevel()->TimeEvent.AddEvent(1.93f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					StartRenderShaking(8);
					SetTimeScale(1.0f);
				});
		}

		if (EnemyHP < 0)
		{
			DeathValue = true;
		}

		});
}

void CavaliereAngelo::Update(float _DeltaTime)
{
	// �̱�, ����, Ŭ���̾�Ʈ ������� ����ؼ� üũ�Ǿ�� �ϴ� �Լ���
	{
		_DeltaTime *= GetTimeScale();
		EnemyRenderer->SetTimeScale(GetTimeScale());
		EffectRenderer_0->TimeScale = GetTimeScale();
		EffectRenderer_1->TimeScale = GetTimeScale();

		//if (GetTimeScale() == 0.0f)
		//{
		//	MonsterAttackCollision->Off();
		//	PhysXCapsule->Off();
		//}
		//else if (false == PhysXCapsule->IsUpdate())
		//{
		//	PhysXCapsule->On();
		//}

		RendererBurn(_DeltaTime);
		RenderShake(_DeltaTime);
		ParryCheck();
	}

	// �̱� �÷����� �� ����
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		DeathCheck();
		RecognizeCollisionCheck(_DeltaTime);
		DamageCollisionCheck(_DeltaTime);
		EnemyFSM.Update(_DeltaTime);
	}
	else
	{
		// ���� �÷����� �� ����
		if (NetControllType::ActiveControll == GetControllType())
		{
			DeathCheck();
			RecognizeCollisionCheck(_DeltaTime);
			DamageCollisionCheck(_DeltaTime);
			EnemyFSM.Update(_DeltaTime);
		}
		// Ŭ���̾�Ʈ �÷����� �� ����
		else
		{
			Update_NetworkTrans(_DeltaTime);
			DamageCollisionCheck_Client(_DeltaTime);
			EnemyFSM.Update(_DeltaTime);
			MonsterAttackCollision->Off();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// ������, ��Ʈ ���� ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CavaliereAngelo::DeathCheck()
{
	if (EnemyHP <= 0)
	{
		DeathValue = true;
	}

	if (true == DeathValue && false == DeathSettig)
	{
		DeathSettig = true;
		MonsterCollision->Off();
		ParryCollision->Off();
		RN_MonsterCollision->Off();
		PhysXCapsule->Off();

		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Daeth_Front);
	}
}

void CavaliereAngelo::PlayerChase(float _DeltaTime)
{
	RotationCheck();

	if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
	{
		std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
		bool Iscontact = PlayerContactCheck_Normal(Collision.get());

		if (true == Iscontact)
		{
			IsRecognize = true;
		}
	}
	else
	{
		IsRecognize = false;
	}

	if (EnemyRotation::Right_180 == EnemyRotationValue)
	{
		if (false == IsRecognize)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Right);
		}
		else
		{
			IsRecognize = false;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Right);
		}
	}
	else if (EnemyRotation::Left_180 == EnemyRotationValue)
	{
		if (false == IsRecognize)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Left);
		}
		else
		{
			IsRecognize = false;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Left);
		}
	}
	else
	{
		if (false == IsRecognize)
		{
			if (true == IsPowerUp)
			{
				IsPowerUp = false;
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Start);
			}
			else
			{
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Start);
			}
		}
		else
		{
			IsRecognize = false;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01);
		}
	}

	return;
}

void CavaliereAngelo::AttackCalculation()
{
	AttackDirectCheck();

	if (true == AnimationTurnStart)
	{
		AnimationTurnStart = false;
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		EnemyHitDirValue = EnemyHitDirect::Forward;
	}

	PushDirectSetting();
}

void CavaliereAngelo::DamageCollisionCheck(float _DeltaTime)
{
	if (true == DeathValue)
	{
		return;
	}

	AttackDelayCheck += _DeltaTime;

	float FrameTime = (1.0f / 60.0f) * 5.0f;

	if (FrameTime > AttackDelayCheck)
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Col = MonsterCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	PlayerAttackCheck(AttackCol.get());
	MonsterAttackCollision->Off();
	DamageData Data = AttackCol->GetDamage();

	if (DamageType::VergilLight == Data.DamageTypeValue)
	{
		IsVergilLight = true;
		Data.DamageTypeValue = DamageType::Light;
	}

	StartRenderShaking(6);
	MinusEnemyHP(Data.DamageValue);
	HPSeverStackPlus(Data.DamageValue);

	if (NetControllType::ActiveControll == GetControllType())
	{
		if (600 < HPServerStack)
		{
			HPServerStack = 0;
			HPClientStack = 0;

			std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
			size_t Playersize = Players.size();

			int PlayerID = Player->GetNetObjectID();

			for (size_t i = 0; i < Playersize; i++)
			{
				int PlayersID = Players[i]->GetNetObjectID();

				if (ServerPlayerID == PlayersID)
				{
					Player = Players[i];
				}
			}
		}
	}

	if (EnemyHP <= 0)
	{
		DeathValue = true;
		return;
	}

	if (true == IsCharge)
	{
		ChargeDamageStack += Data.DamageValue;
	}

	if (ChargeDamageStack >= 650)
	{
		ChargeDamageStack = 0;
		RotationCheck();
		AllDirectSetting();
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Start);
	}

	if (DamageType::Buster == Data.DamageTypeValue)
	{
		IsStun = false;
		SetTimeScale(0.4f);
		GetLevel()->TimeEvent.AddEvent(.316f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				StartRenderShaking(8);
			});
		GetLevel()->TimeEvent.AddEvent(.683f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				StartRenderShaking(8);
			});
		GetLevel()->TimeEvent.AddEvent(1.13f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				StartRenderShaking(8);
			});
		GetLevel()->TimeEvent.AddEvent(1.4f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				StartRenderShaking(8);
			});
		GetLevel()->TimeEvent.AddEvent(1.6f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				StartRenderShaking(8);
			});
		GetLevel()->TimeEvent.AddEvent(1.7f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				SetTimeScale(0.3f);
			});
		GetLevel()->TimeEvent.AddEvent(2.5f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				StartRenderShaking(8);
				SetTimeScale(0.0f);
			});
		GetLevel()->TimeEvent.AddEvent(3.5f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				SetTimeScale(1.0f);
			});
	}

	IsVergilLight = false;
	AttackDelayCheck = 0.0f;
}

void CavaliereAngelo::DamageCollisionCheck_Client(float _DeltaTime)
{
	if (true == DeathValue)
	{
		return;
	}

	AttackDelayCheck += _DeltaTime;

	float FrameTime = (1.0f / 60.0f) * 5.0f;

	if (FrameTime > AttackDelayCheck)
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Col = MonsterCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	PlayerAttackCheck(AttackCol.get());
	DamageData Data = AttackCol->GetDamage();

	if (DamageType::VergilLight == Data.DamageTypeValue)
	{
		IsVergilLight = true;
		Data.DamageTypeValue = DamageType::Light;
	}

	StartRenderShaking(6);
	AttackDelayCheck = 0.0f;
}

void CavaliereAngelo::RecognizeCollisionCheck(float _DeltaTime)
{
	if (true == IsRecognize || false == RN_MonsterCollision->IsUpdate() || true == DeathValue)
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
	if (nullptr == Collision) { IsRecognize = false;  return; }

	bool Iscontact = PlayerContactCheck(_DeltaTime, Collision.get());

	if (true == Iscontact)
	{
		IsRecognize = true;
	}
}

void CavaliereAngelo::ParryCheck()
{
	if (false == IsParryCheck || true == ParryOkay || true == DeathValue)
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Col = ParryCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	if (false == AttackCol->GetIsParryAttack()) { return; }
	AttackCol->ParryEvent();
	ParryOkay = true;
}

void CavaliereAngelo::ParryTime()
{
	SetTimeScale(0.2f);
	GetLevel()->TimeEvent.AddEvent(0.4f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			SetTimeScale(0.4f);
		});
	GetLevel()->TimeEvent.AddEvent(0.6f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			SetTimeScale(0.6f);
		});
	GetLevel()->TimeEvent.AddEvent(0.8f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			SetTimeScale(0.8f);
		});
	GetLevel()->TimeEvent.AddEvent(1.f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			SetTimeScale(1.f);
		});
}

void CavaliereAngelo::BossTurn()
{
	//RotationCheck();

	float4 CrossResult = MonsterAndPlayerCross();
	float RotationValue = 0.0f;
	float4 EnemyPosition = GetTransform()->GetWorldPosition();
	float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();

	float4 EnemyForWardVector = GetTransform()->GetWorldForwardVector();
	EnemyForWardVector.Normalize();

	PlayerPosition.y = 0.0f;
	EnemyPosition.y = 0.0f;

	float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
	float4 RotationDirectNormal = ToPlayerVector.NormalizeReturn();
	RotationValue = float4::GetAngleVectorToVectorDeg(EnemyForWardVector, RotationDirectNormal);

	if (-1.0f <= RotationValue && 1.0f >= RotationValue)
	{
		AllDirectSetting_Normal();
		return;
	}

	if (true == isnan(RotationValue))
	{
		AllDirectSetting_Normal();
		return;
	}

	if (CrossResult.y < 0)
	{
		if (RotationValue >= 0)
		{
			RotationValue = -RotationValue;
		}
	}
	else if (CrossResult.y > 0)
	{
		if (RotationValue < 0)
		{
			RotationValue = -RotationValue;
		}
	}

	float4 CurRot = GetTransform()->GetWorldRotation();

	if (CurRot.y <= 0.0f)
	{
		CurRot.y += 360.f;
	}

	float4 Value = float4{ 0.0f, RotationValue, 0.0f };

	float4 GoalRot = CurRot + Value;

	if (GoalRot.y <= 0.0f)
	{
		CurRot.y += 360.f;
		GoalRot = CurRot + Value;
	}

	CurRot.x = 0.0f;
	CurRot.z = 0.0f;
	GoalRot.x = 0.0f;
	GoalRot.z = 0.0f;

	PhysXCapsule->SetWorldRotation(GoalRot);
	AllDirectSetting_Normal();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////     FSM     ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CavaliereAngelo::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

void CavaliereAngelo::ChangeState_Client(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, Player);
}

void CavaliereAngelo::EnemyCreateFSM()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Idle     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ����ڼ��� ������
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_defense-Idle");
	},
	.Update = [=](float _DeltaTime) {

	if (3 <= ColliderStack)
	{
		IsRecognize = false;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider);
		return;
	}

	WaitTime += _DeltaTime;

	if (WaitTime >= 0.2f)
	{
		PlayerChase(_DeltaTime);
		return;
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Special     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ���� ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Start,
	.Start = [=] {
	EffectRenderer_0->PlayFX("Cavalier_Charge_1.effect");
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_start");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});

	// ���� ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Loop,
	.Start = [=] {
	EffectRenderer_0->PlayFX("Cavalier_Charge_2.effect");
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_loop");
	IsCharge = true;
	},
	.Update = [=](float _DeltaTime) {

	WaitTime += _DeltaTime;

	if (WaitTime > 6.0f)
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_End);
		return;
	}

	},
	.End = [=] {
	IsCharge = false;
	ChargeDamageStack = 0;
	WaitTime = 0.0f;
	}
		});

	// ���� ���� ��, 100������ on, 105������ off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_End,
	.Start = [=] {
	IsPowerUp = true;
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, 200);
	EffectRenderer_0->PlayFX("Cavalier_Charge_3.effect");
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_end");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	}
		});

	// ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_Start,
	.Start = [=] {
	EffectRenderer_1->PlayFX("Cavalier_Warp.effect");
	EnemyRenderer->ChangeAnimation("em5501_warp_start");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		PhysXCapsule->SetWorldPosition({ 0, 100, 0 });
		PhysXCapsule->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Start);
		return;
	}
	},
	.End = [=] {
	}
		});

	// ���� ��->����ڼ��� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_warp_end");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// ���� ���� Į ���� �� ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider,
	.Start = [=] {
	ColliderStack = 0;
	EffectRenderer_0->PlayFX("Cavalier_Colider.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_collider");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		PhysXCapsule->SetWorldPosition({ 0, 100, 0 });
		PhysXCapsule->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider_To_Dengeki);
		return;
	}
	},
	.End = [=] {
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em5501_Attack_collider_to_Dengeki", 70, [=] {
			RotationCheck();
			AllDirectSetting_Normal();
			SlerpCalculation();
			IsFastCharge = true; });
	}

	// ���� �Ϸ� �� Dengeki �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider_To_Dengeki,
	.Start = [=] {
	EffectRenderer_1->PlayFX("Cavalier_Collider_To_Dengeki.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_collider_to_Dengeki");
	},
	.Update = [=](float _DeltaTime) {

	if (70 < EnemyRenderer->GetCurFrame())
	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsRecognize = false;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop);
		return;
	}
	},
	.End = [=] {
	EffectRenderer_1->Off();
	SlerpTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Move     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ���ִٰ� ����ڼ� ���ϸ鼭 �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Attack_To_Guard_Start,
	.Start = [=] {
	RotationCheck();
	AllDirectSetting_Normal();
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em5501_A-walk_start_L0");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime * 2.0f);
		AllDirectSetting_Normal();
		SetForwardMove(140.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// ����ڼ� �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Start,
	.Start = [=] {
	RotationCheck();
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em5501_guard_to_walk");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime * 2.0f);
		AllDirectSetting_Normal();
		SetForwardMove(140.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// ����ڼ� �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_loop");
	},
	.Update = [=](float _DeltaTime) {

	{
		BossTurn();
		SetForwardMove(220.0f);
	}

	if (false == IsRecognize)
	{
		//ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01);
		//return;
	}
	else
	{
		IsRecognize = false;
		if (3 <= ColliderStack)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01);
		}
		return;
	}
	},
	.End = [=] {
	}
		});

	// ����ڼ� �ȱ� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_End_L0");
	},
	.Update = [=](float _DeltaTime) {

	{
		AllDirectSetting_Normal();
		SetForwardMove(140.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// Dengeki �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Start,
	.Start = [=] {
	RotationCheck();
	AllDirectSetting_Normal();
	SlerpCalculation();
	EffectRenderer_0->PlayFX("Cavalier_Dengeki_Start.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Start");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime * 2.0f);
		AllDirectSetting_Normal();
		SetForwardMove(140.0f);
	}

	if (true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack04);
		return;
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	EffectRenderer_0->Off();
	}
		});

	// Dengeki �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Light, 100);
	WaitTime = 0.2f;
	EffectRenderer_0->PlayFX("Cavalier_Dengeki_Loop.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Loop");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime -= _DeltaTime;
	if (WaitTime < 0)
	{
		if (nullptr == Player)
		{
			Electric->Shoot(GetTransform()->GetLocalForwardVector());
		}
		else
		{
			Electric->Shoot(Player);
		}
		WaitTime = 1.0f;
	}
	{
		BossTurn();
		SetForwardMove(200.0f);
	}

	if (true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_End);
		return;
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsFastCharge = false;
		ColliderStack = 0;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_End);
		return;
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	EffectRenderer_0->Off();
	}
		});

	// Dengeki �ȱ� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_End,
	.Start = [=] {
	EffectRenderer_0->PlayFX("Cavalier_Dengeki_End.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_End");
	},
	.Update = [=](float _DeltaTime) {

	{
		BossTurn();
		SetForwardMove(220.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
		{
			std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
			bool Iscontact = PlayerContactCheck(_DeltaTime, Collision.get());

			if (true == Iscontact)
			{
				IsRecognize = true;
			}
		}
		else
		{
			IsRecognize = false;
		}

		if (false == IsRecognize)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop);
		}
		else
		{
			IsRecognize = false;
			if (3 <= ColliderStack)
			{
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider);
			}
			else
			{
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01);
			}
		}
		return;
	}
	},
	.End = [=] {
		EffectRenderer_0->Off();
	}
		});

	// ����ڼ� ���� �������� 180�� ȸ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_quick_turn_L");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// ����ڼ� ���� ���������� 180�� ȸ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_quick_turn_R");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Attack     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	{
		// Attack1 -> 2 ����
		EnemyRenderer->SetAnimationStartEvent("em5501_Attack01_R", 68, [=] {
			// ColliderStack�� �� �� ��쿡�� 25�� Ȯ���� Idle�� ���� (�׳� return)
			if (3 <= ColliderStack + 1)
			{
				int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);
				if (0 == RandC)
				{
					return;
				}
			}

			// ��ó�� �÷��̾ ������ �ļ�Ÿ�� ����
			if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
			{
				std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
				bool Iscontact = PlayerContactCheck_Normal(Collision.get());

				if (true == Iscontact)
				{
					IsRecognize = true;
				}

				if (true == IsRecognize)
				{
					IsRecognize = false;
					PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
					Event01 = false;
					Normal01 = false;
					ParryStack = 0;
					++ColliderStack;
					SetMoveStop();
					ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02);
				}
				return;
			}
			else
			{
				IsRecognize = false;
			}
			});

		// Attack2 -> 3 ����
		EnemyRenderer->SetAnimationStartEvent("em5501_Attack02", 59, [=] {
			if (3 <= ColliderStack + 1)
			{
				int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);
				if (0 == RandC)
				{
					return;
				}
			}
			if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
			{
				std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
				bool Iscontact = PlayerContactCheck_Normal(Collision.get());

				if (true == Iscontact)
				{
					IsRecognize = true;
				}

				if (true == IsRecognize)
				{
					PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
					Event01 = false;
					Normal01 = false;
					SetMoveStop();
					ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03);
				}
				return;
			}
			else
			{
				IsRecognize = false;
			}
			});

		// Attack3 -> 4 ����
		EnemyRenderer->SetAnimationStartEvent("em5501_Attack03", 37, [=] {
			if (3 <= ColliderStack + 1)
			{
				int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);
				if (0 == RandC)
				{
					return;
				}
			}
			if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
			{
				std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
				bool Iscontact = PlayerContactCheck_Normal(Collision.get());

				if (true == Iscontact)
				{
					IsRecognize = true;
				}

				if (true == IsRecognize)
				{
					PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
					Event01 = false;
					Normal01 = false;
					SetMoveStop();
					ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack04);
				}
				return;
			}
			else
			{
				IsRecognize = false;
			}
			});
	}

	// 01(���ߴ�) ����, ������ ����ڼ�, 66������ on, 69������ off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, 200);
	RotationCheck();
	SlerpCalculation();
	AllDirectSetting_Normal();
	SlerpCalculation();
	SlerpTime = 0.0f;
	EffectRenderer_0->PlayFX("Cavalier_Attack_1.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack01_R");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();

		if (79 > EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}

		if (129 < EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}

		if (EnemyRenderer->GetCurFrame() < 64)
		{
			if (50 < EnemyRenderer->GetCurFrame())
			{
				RotationCheck();
				SlerpCalculation();
				SlerpTime = 0.0f;
				SlerpTurn(_DeltaTime * 10.0f);
			}
			else
			{
				SlerpTurn(_DeltaTime * 1.1f);
			}
		}
	}

	if (false == ParryOkay)
	{
		if (60 <= EnemyRenderer->GetCurFrame() && 69 > EnemyRenderer->GetCurFrame())
		{
			IsParryCheck = true;
		}
		else
		{
			IsParryCheck = false;
		}
	}

	if (true == ParryOkay)
	{
		SetMoveStop();
		MonsterAttackCollision->Off();
		ParryOkay = false;
		IsParryCheck = false;
		++ParryStack;

		if (ParryStack <= 1)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01);
		}
		else if (ParryStack >= 2 && ParryStack < 5)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01);
		}
		else if (ParryStack >= 5)
		{
			ParryStack = 0;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill);
		}
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ParryStack = 0;
		++ColliderStack;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	IsParryCheck = false;
	SlerpTime = 0.0f;
	}
		});

	// 02(�����) ����, ������ ����ڼ�, 44������ on, 47������ off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, 200);
	RotationCheck();
	SlerpCalculation();
	SlerpTime = 0.0f;
	EffectRenderer_1->PlayFX("Cavalier_Attack_2.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack02");
	if (true == Event01)
	{
		Event01 = false;
		EnemyRenderer->SetCurFrame(38);
		EffectRenderer_1->SetCurFrame(38);
	}
	if (true == Normal01)
	{
		Normal01 = false;
		EnemyRenderer->SetCurFrame(38);
		EffectRenderer_1->SetCurFrame(38);
	}
	},
	.Update = [=](float _DeltaTime) {

	{
		AllDirectSetting_Normal();

		if (57 > EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}

		if (117 < EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}
		if (EnemyRenderer->GetCurFrame() < 40)
		{
			if (30 < EnemyRenderer->GetCurFrame())
			{
				RotationCheck();
				SlerpCalculation();
				SlerpTime = 0.0f;
				SlerpTurn(_DeltaTime * 10.0f);
			}
			else
			{
				SlerpTurn(_DeltaTime * 1.36f);
			}
		}
	}

	if (false == ParryOkay)
	{
		if (38 <= EnemyRenderer->GetCurFrame() && 47 > EnemyRenderer->GetCurFrame())
		{
			IsParryCheck = true;
		}
		else
		{
			IsParryCheck = false;
		}
	}

	if (true == ParryOkay)
	{
		SetMoveStop();
		MonsterAttackCollision->Off();
		ParryOkay = false;
		IsParryCheck = false;
		++ParryStack;

		if (ParryStack <= 1)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01);
		}
		else if (ParryStack >= 2 && ParryStack < 5)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01);
		}
		else if (ParryStack >= 5)
		{
			ParryStack = 0;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill);
		}
		return;
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	IsParryCheck = false;
	}
		});

	// 03(���ߴ�) ����, ������ ����ڼ�, 29������ on, 33������ off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, 200);
	RotationCheck();
	SlerpCalculation();
	SlerpTime = 0.0f;
	EffectRenderer_0->PlayFX("Cavalier_Attack_3.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack03");
	if (true == Event01)
	{
		Event01 = false;
		EnemyRenderer->SetCurFrame(23);
		EffectRenderer_0->SetCurFrame(23);
	}
	if (true == Normal01)
	{
		Normal01 = false;
		EnemyRenderer->SetCurFrame(23);
		EffectRenderer_0->SetCurFrame(23);
	}
	},
	.Update = [=](float _DeltaTime) {

	{
		AllDirectSetting_Normal();

		if (43 > EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}

		if (113 < EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}

		if (EnemyRenderer->GetCurFrame() < 28)
		{
			if (20 < EnemyRenderer->GetCurFrame())
			{
				RotationCheck();
				SlerpCalculation();
				SlerpTime = 0.0f;
				SlerpTurn(_DeltaTime * 10.0f);
			}
			else
			{
				SlerpTurn(_DeltaTime * 2.0f);
			}
		}
	}

	if (false == ParryOkay)
	{
		if (23 <= EnemyRenderer->GetCurFrame() && 33 > EnemyRenderer->GetCurFrame())
		{
			IsParryCheck = true;
		}
		else
		{
			IsParryCheck = false;
		}
	}

	if (true == ParryOkay)
	{
		SetMoveStop();
		MonsterAttackCollision->Off();
		ParryOkay = false;
		IsParryCheck = false;
		++ParryStack;

		if (ParryStack <= 1)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01);
		}
		else if (ParryStack >= 2 && ParryStack < 5)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01);
		}
		else if (ParryStack >= 5)
		{
			ParryStack = 0;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill);
		}
		return;
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	IsParryCheck = false;
	}
		});

	// �÷��̾� �и� ���� �� �ٷ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01,
	.Start = [=] {
	ParryTime();
	EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		float4 Direction = MonsterAndPlayerCross();
		RotationCheck();
		AllDirectSetting_Normal();
		SlerpCalculation();
		if (Direction.y > 0)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_03);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_02);
		}
		return;
	}
	},
	.End = [=] {
	}
		});

	// �÷��̾� �и� �� ���� �� �����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01,
	.Start = [=] {
	ParryTime();
	EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01");
		},
	.Update = [=](float _DeltaTime) {

	{
		AllDirectSetting_Normal();

		if (35 > EnemyRenderer->GetCurFrame())
		{
			SetBackMove(100.f);
		}
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		float4 Direction = MonsterAndPlayerCross();
		RotationCheck();
		AllDirectSetting_Normal();
		SlerpCalculation();
		SetMoveStop();
		if (Direction.y > 0)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_03);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_02);
		}
		return;
	}
	},
	.End = [=] {
	}
		});

	// 01 �и� �� �ٷ� 02 ���� ��, 37������ ��ŸƮ (38 ���������� ����)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01_to_02");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Event01 = true;
		SetMoveStop();
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// 01 �и� �� �ٷ� 03 ���� ��, 24 ������ ��ŸƮ (23 ���������� ����)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01_to_04");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Event01 = true;
		SetMoveStop();
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// 01 �и� �� ����, ���� 02 ���� ��, 37������ ��ŸƮ (38 ���������� ����)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_02");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Normal01 = true;
		SetMoveStop();
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// 01 �и� �� ����, ���� 03 ���� ��, 24 ������ ��ŸƮ (23 ���������� ����)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_03");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Normal01 = true;
		SetMoveStop();
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// 04(������� ������) ����, ������ ����ڼ�, 90������ on, 94������ off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack04,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, 200);
	RotationCheck();
	SlerpCalculation();
	SlerpTime = 0.0f;
	SetMoveStop();
	EffectRenderer_1->PlayFX("Cavalier_Attack_4.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack04");
	},
	.Update = [=](float _DeltaTime) {

	if (EnemyRenderer->GetCurFrame() < 90)
	{
		RotationCheck();
		SlerpCalculation();
		SlerpTime = 0.0f;
		SlerpTurn(_DeltaTime * 5.0f);
	}
	if (140 < EnemyRenderer->GetCurFrame())
	{
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (false == ParryOkay)
	{
		if (85 <= EnemyRenderer->GetCurFrame() && 94 > EnemyRenderer->GetCurFrame())
		{
			IsParryCheck = true;
		}
		else
		{
			IsParryCheck = false;
		}
	}

	if (true == ParryOkay)
	{
		SetMoveStop();
		MonsterAttackCollision->Off();
		ParryOkay = false;
		IsParryCheck = false;
		++ParryStack;

		if (ParryStack <= 1)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01);
		}
		else if (ParryStack >= 2 && ParryStack < 5)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01);
		}
		else if (ParryStack >= 5)
		{
			ParryStack = 0;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill);
		}
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		if (true == IsFastCharge)
		{
			IsFastCharge = false;
			ColliderStack = 0;
		}

		if (2 <= ColliderStack)
		{
			ColliderStack = 0;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		}
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	SlerpTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Damage     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// �������(�帱) �°� ���� (������ ���� �� ��), ���� �ڼ� ȸ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill,
	.Start = [=] {
	IsRecognize = false;
	Event01 = false;
	Normal01 = false;
	IsPowerUp = false;
	IsFastCharge = false;
	IsParryCheck = false;
	ParryOkay = false;
	IsStun = true;
	ColliderStack = 0;
	ParryStack = 0;
	ParryTime();
	RotationCheck();
	AllDirectSetting_Normal();
	EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em5501_Damage_Drill");
	},
	.Update = [=](float _DeltaTime) {

	if (85 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(160.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_Start);
		return;
	}
	},
	.End = [=] {
	IsStun = false;
	}
		});

	// ������ ��ø ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Start,
	.Start = [=] {
	IsRecognize = false;
	Event01 = false;
	Normal01 = false;
	IsPowerUp = false;
	IsFastCharge = false;
	IsParryCheck = false;
	ParryOkay = false;
	IsStun = true;
	ParryStack = 0;
	ColliderStack = 0;
	ParryTime();
	RotationCheck();
	AllDirectSetting_Normal();
	EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em5501_stun_start");
	},
	.Update = [=](float _DeltaTime) {

	if (50 < EnemyRenderer->GetCurFrame() && 70 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(120.0f);
	}

	if (100 < EnemyRenderer->GetCurFrame() && 120 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Hukki);
		return;
	}
	},
	.End = [=] {
	IsStun = false;
	}
		});

	// ������ ��ø ���� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Hukki,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_stun_hukki");
	IsSturn = false;
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// �������
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Daeth_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_daeth_front");
	},
	.Update = [=](float _DeltaTime) {

	if (6 < EnemyRenderer->GetCurFrame() && 62 > EnemyRenderer->GetCurFrame())
	{
		float4 CurPos = EnemyRenderer->GetTransform()->GetWorldPosition();
		float4 BackMove = EnemyRenderer->GetTransform()->GetWorldBackVector();
		float4 PlusPos = BackMove * _DeltaTime * 200.0f;
		float4 NewPos = CurPos + PlusPos;
		EnemyRenderer->GetTransform()->SetWorldPosition(NewPos);
	}

	if (150 < EnemyRenderer->GetCurFrame() && 235 > EnemyRenderer->GetCurFrame())
	{
		float4 CurPos = EnemyRenderer->GetTransform()->GetWorldPosition();
		float4 BackMove = EnemyRenderer->GetTransform()->GetWorldBackVector();
		float4 PlusPos = BackMove * _DeltaTime * 150.0f;
		float4 NewPos = CurPos + PlusPos;
		EnemyRenderer->GetTransform()->SetWorldPosition(NewPos);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsBurn = true;
	}
	},
	.End = [=] {
	}
		});

	EnemyFSM.ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
}

void CavaliereAngelo::EnemyCreateFSM_Client()
{
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_defense-Idle");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_end");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_warp_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_warp_end");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_collider");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider_To_Dengeki,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_collider_to_Dengeki");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Attack_To_Guard_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_start_L0");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_to_walk");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_End_L0");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_End");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_quick_turn_L");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_quick_turn_R");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack01_R");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack03");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01");
		},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01_to_02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01_to_04");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_03");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack04,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack04");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Damage_Drill");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_stun_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Hukki,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_stun_hukki");
	IsSturn = false;
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Daeth_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_daeth_front");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
}
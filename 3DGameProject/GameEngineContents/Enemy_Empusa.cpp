#include "PrecompileHeader.h"
#include "Enemy_Empusa.h"

#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineBase/GameEngineRandom.h>

#include "NetworkManager.h"
#include "AnimationEvent.h"
#include "BasePlayerActor.h"
#include "AttackCollision.h"

Enemy_Empusa::Enemy_Empusa() 
{
}

Enemy_Empusa::~Enemy_Empusa() 
{
}

void Enemy_Empusa::Start()
{
	BaseEnemyActor::Start();

	// ������ ũ�� ����
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -50.0f, 0.0f });

	// �ݸ��� �ɼ�, ũ�� ����
	MonsterAttackCollision->SetAttackData(DamageType::Light, 0);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);
	MonsterCollision->GetTransform()->SetLocalScale({ 80, 180, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 50, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 700, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	// �⺻ ����
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	MonsterCollision->Off();
	RN_MonsterCollision->Off();

	// �� ������Ʈ Ÿ�� ����
	SetNetObjectType(Net_ActorType::Empusa);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// ������, ��Ʈ ���� ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_Empusa::PlayerChase(float _DeltaTime)
{
	RotationCheck();

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		AllDirectSetting();
		if (true)
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Start);
		}
		else
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Start);
		}
		break;
	case EnemyRotation::Left:
		AllDirectSetting();
		if (true)
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Start);
		}
		else
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Start);
		}
		break;
	case EnemyRotation::Left_90:
		ChangeState(FSM_State_Empusa::Empusa_Turn_Left_90);
		break;
	case EnemyRotation::Left_180:
		ChangeState(FSM_State_Empusa::Empusa_Turn_Left_180);
		break;
	case EnemyRotation::Right:
		AllDirectSetting();
		if (true)
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Start);
		}
		else
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Start);
		}
		break;
	case EnemyRotation::Right_90:
		ChangeState(FSM_State_Empusa::Empusa_Turn_Right_90);
		break;
	case EnemyRotation::Right_180:
		ChangeState(FSM_State_Empusa::Empusa_Turn_Right_180);
		break;
	default:
		break;
	}
}

void Enemy_Empusa::PlayerAttack(float _DeltaTime)
{
	RotationCheck();
	AllDirectSetting();

	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

	if (0 == RandC)
	{
		ChangeState(FSM_State_Empusa::Empusa_Continuation_Attack);
		return;
	}

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		RandomAttack();
		break;
	case EnemyRotation::Left:
		ChangeState(FSM_State_Empusa::Empusa_Attack_C);
		break;
	case EnemyRotation::Left_90:
		ChangeState(FSM_State_Empusa::Empusa_Attack_C);
		break;
	case EnemyRotation::Left_180:
		ChangeState(FSM_State_Empusa::Empusa_Attack_C);
		break;
	case EnemyRotation::Right:
		ChangeState(FSM_State_Empusa::Empusa_Attack_D);
		break;
	case EnemyRotation::Right_90:
		ChangeState(FSM_State_Empusa::Empusa_Attack_D);
		break;
	case EnemyRotation::Right_180:
		ChangeState(FSM_State_Empusa::Empusa_Attack_D);
		break;
	default:
		break;
	}
}

void Enemy_Empusa::RandomAttack()
{
	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);

	if (0 == RandC)
	{
		ChangeState(FSM_State_Empusa::Empusa_Attack_A);
	}
	else if (1 == RandC)
	{
		ChangeState(FSM_State_Empusa::Empusa_Attack_B);
	}
	else if (2 == RandC)
	{
		ChangeState(FSM_State_Empusa::Empusa_Attack_W);
	}
}

void Enemy_Empusa::DamageCollisionCheck(float _DeltaTime)
{
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

	AttackDirectCheck();

	if (true == AnimationTurnStart)
	{
		AnimationTurnStart = false;
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		EnemyHitDirValue = EnemyHitDirect::Forward;
	}

	PushDirectSetting();
	DamageData Data = AttackCol->GetDamage();

	if (DamageType::VergilLight == Data.DamageTypeValue)
	{
		IsVergilLight = true;
		Data.DamageTypeValue = DamageType::Light;
	}

	switch (Data.DamageTypeValue)
	{
	case DamageType::None:
		return;
		break;
	case DamageType::Light:

		if (true == IsHeavyAttack || true == IsSlamAttack || true == IsBusterAttack)
		{
			return;
		}

		if (true == IsAirAttack)
		{
			StartRenderShaking(8);
			ChangeState(FSM_State_Empusa::Empusa_Air_Damage_Under);
			return;
		}

		if (true == IsCollapse)
		{
			StartRenderShaking(8);
			return;
		}

		switch (EnemyHitDirValue)
		{
		case EnemyHitDirect::Forward:
			ChangeState(FSM_State_Empusa::Empusa_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			ChangeState(FSM_State_Empusa::Empusa_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			ChangeState(FSM_State_Empusa::Empusa_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			ChangeState(FSM_State_Empusa::Empusa_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		IsCollapse = false;
		IsHeavyAttack = true;
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Blown_Back);
		break;
	case DamageType::Air:
		IsCollapse = false;
		IsAirAttack = true;
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Air_Damage);
		break;
	case DamageType::Snatch:
		IsCollapse = false;
		IsAirAttack = true;
		StartMonsterSnatch();
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Snatch);
		break;
	case DamageType::Slam:
		IsCollapse = false;
		IsSlamAttack = true;
		ChangeState(FSM_State_Empusa::Empusa_Slam_Damage);
		break;
	case DamageType::Buster:
		IsCollapse = false;
		IsBusterAttack = true;
		BusterCalculation();
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Buster_Start);
		break;
	case DamageType::Stun:
		break;
	default:
		break;
	}

	IsVergilLight = false;
	AttackDelayCheck = 0.0f;
}

void Enemy_Empusa::RecognizeCollisionCheck(float _DeltaTime)
{
	if (true == IsRecognize || false == RN_MonsterCollision->IsUpdate())
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
	if (nullptr == Collision) { return; }

	PlayerContactCheck(_DeltaTime, Collision.get());
	IsRecognize = true;
}

void Enemy_Empusa::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////   Actor Init   ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_Empusa::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Character", "Enemy", "Empusa", "mesh"
			},
			"em0100.FBX"
		);
		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("em0100.fbx", "AniFBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("em0100.fbx", "AniFBX");
	}
	break;
	default:
		break;
	}

	EnemyRenderer->GetTransform()->SetLocalScale({ 0.8f , 0.8f , 0.8f });
}

void Enemy_Empusa::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::Empusa;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Small;

	EnemyHP = 0;
}

void Enemy_Empusa::EnemyAnimationLoad()
{
	//Animation���� ��θ� ã�Ƽ� ���animation���� �ε�
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("Empusa");
	NewDir.Move("Animation");

	AnimationEvent::LoadAll
	(
		{
			.Dir = NewDir.GetFullPath().c_str(),
			.Renderer = EnemyRenderer,
			.RendererLocalPos = { 0.0f, -50.0f, 0.0f },
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
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////     FSM     ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_Empusa::EnemyCreateFSM()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     ����     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ���� ����_A
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Appear_A,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0100_enter_ground_A");
	},
	.Update = [=](float _DeltaTime) {
	EnemyRenderer->On();
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterCollision->On();
	RN_MonsterCollision->On();
	}
		});
	// ���� ����_B
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Appear_B,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0100_enter_ground_B");
	},
	.Update = [=](float _DeltaTime) {
	EnemyRenderer->On();
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterCollision->On();
	RN_MonsterCollision->On();
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Idle     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Idle,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_biped_idle");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime += _DeltaTime;
	if (0.5f <= WaitTime)
	{
		if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
		{
			IsRecognize = true;
		}
		else
		{
			IsRecognize = false;
		}

		if (false == IsRecognize)
		{
			PlayerChase(_DeltaTime);
		}
		else
		{
			IsRecognize = false;
			PlayerAttack(_DeltaTime);
		}
		return;
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      Move      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ������ �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_start");
	},
	.Update = [=](float _DeltaTime) {
	if (8 < EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(100.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0100_biped_walk_stop", 208, [=] { ++WalkCount; });
	}

	// ������ �ȱ� Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	SetForwardMove(140.0f);
	if (0.5f >= MonsterAndPlayerDotProduct() || true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Stop);
		return;
	}

	if (1 == WalkCount)
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Stop);
		return;
	}
	},
	.End = [=] {
	WalkCount = 0;
	}
		});
	// ������ �ȱ� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (48 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// ������ �ٱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_start");
	},
	.Update = [=](float _DeltaTime) {
	if (8 < EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(140.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ������ �ٱ� Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_loop");
	},
	.Update = [=](float _DeltaTime) {
	RunTime += _DeltaTime;
	SetForwardMove(200.0f);
	if (0.5f >= MonsterAndPlayerDotProduct() || true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Stop);
		return;
	}

	if (2.0f <= RunTime)
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Stop);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});
	// ������ �ٱ� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (48 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(110.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////   Rotation   //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ���� 90�� ȸ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left_90,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_turn_90_left");
	},
	.Update = [=](float _DeltaTime) {

	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, -90.f, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// ������ 90�� ȸ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right_90,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_turn_90_right");
	},
	.Update = [=](float _DeltaTime) {
	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 90.f, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// ���� 180�� ȸ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left_180,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_turn_180_left");
	},
	.Update = [=](float _DeltaTime) {
	if (30 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, -180.f, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ������ 180�� ȸ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right_180,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_turn_180_right");
	},
	.Update = [=](float _DeltaTime) {
	if (30 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     �� ��     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	{
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_A", 135, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_A", 136, [=] { SetAdvance(29000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_B", 67, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_B", 68, [=] { SetAdvance(29000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_W", 135, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_W", 136, [=] { SetAdvance(29000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_C", 87, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_C", 88, [=] { SetAdvance(24000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_D", 87, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_D", 88, [=] { SetAdvance(24000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0100_continuation_attack", 100, [=] { 	RotationCheck(); AllDirectSetting(); });
	}

	// ������ ������ �Ʒ��� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_A,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_A");
	},
	.Update = [=](float _DeltaTime) {
	if (85 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 118)
	{
		SetForwardMove(70.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// �޼� ������ �Ʒ��� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_B,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_B");
	},
	.Update = [=](float _DeltaTime) {
	if (30 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 57)
	{
		SetForwardMove(70.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ��� �������
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_W,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_W");
	},
	.Update = [=](float _DeltaTime) {
	if (285 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 320)
	{
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ������ Ⱦ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_C,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0100_attack_C");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// �޼� Ⱦ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_D,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0100_attack_D");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// ��������
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Continuation_Attack,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_continuation_attack");
	},
	.Update = [=](float _DeltaTime) {
	if (100 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 300)
	{
		SetForwardMove(150.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      �ǰ�      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////�����
	// ���� ����� �ǰ�
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Front,
	.Start = [=] {
	if (true == IsVergilLight)
	{
		SetPush(10000.0f);
	}
	else
	{
		SetPush(26000.0f);
	}

	EnemyRenderer->ChangeAnimation("em0100_angledamage_front", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// �ĸ� ����� �ǰ�
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Back,
	.Start = [=] {
	if (true == IsVergilLight)
	{
		SetPush(10000.0f);
	}
	else
	{
		SetPush(26000.0f);
	}
	EnemyRenderer->ChangeAnimation("em0100_angledamage_back", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ���� ����� �ǰ�
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Left,
	.Start = [=] {
	if (true == IsVergilLight)
	{
		SetPush(10000.0f);
	}
	else
	{
		SetPush(26000.0f);
	}
	EnemyRenderer->ChangeAnimation("em0100_angledamage_left", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ���� ����� �ǰ� 
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Right,
	.Start = [=] {
	if (true == IsVergilLight)
	{
		SetPush(10000.0f);
	}
	else
	{
		SetPush(26000.0f);
	}
	EnemyRenderer->ChangeAnimation("em0100_angledamage_right", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////������
	// ������ �°� ���ư�
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Blown_Back,
	.Start = [=] {
	SetPush(50000.0f);
	SetAir(42000.0f);
	EnemyRenderer->ChangeAnimation("em0100_blown_back_start", true);
	},
	.Update = [=](float _DeltaTime) {
	FallCheckDelayTime += _DeltaTime;
	if (true == FloorCheck(FallDistance) && 0.2f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_Empusa::Empusa_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});
	/////////////////////////�����ǰ�
	// ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Air_Damage,
	.Start = [=] {
	PhysXCapsule->SetAirState(110000.0f);
	EnemyRenderer->ChangeAnimation("em0100_air_damage", true);
	},
	.Update = [=](float _DeltaTime) {

	FallCheckDelayTime += _DeltaTime;

	if (true == FloorCheck(FallDistance) && 0.5f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_Empusa::Empusa_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0100_air_damage_under", 1, [=] {
			if (true == IsVergilLight)
				SetAir(10000.0f);
			else
				SetAir(30000.0f);
			});
	}

	// ���� ���¿��� ����� ������
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Air_Damage_Under,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_air_damage_under", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_Empusa::Empusa_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ������ �ǰ�, �����ǰ� ������ ���� ������
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Blown_Back_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0100_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Downward_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////���ư��ٰ� ���� ����
	// ���ư��ٰ� ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Blown_Wall_Back,
	.Start = [=] {

	EnemyRenderer->ChangeAnimation("em0100_blown_wall_back");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	}
	});

	/////////////////////////����

	{
		EnemyRenderer->SetAnimationStartEvent("em0100_slam_damage_start", 1, [=] { SetAir(-120000.0f); });
	}

	// ���� �ǰ� start
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_start");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd() || true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_Empusa::Empusa_Slam_Damage_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ���� �ǰ� ���� loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_fall_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_Empusa::Empusa_Slam_Damage_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ���� �ǰ� �� ���� �΋H��
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Downward_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////����ġ
	// ����ġ start
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_snatch_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Air_Damage_Under);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ����ġ end
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Snatch_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_snatch_end");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Air_Damage_Under);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////����
	// ���� start
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_stun_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Stun_Revive);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ���� end
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_stun_revive");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      ������      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////// ������ �Ѿ����� ����
	// ������ �Ѿ���
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Down,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_downward_down_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// ������ �Ѿ��� ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Down_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_downward_down_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// ������ �������� �� �Ͼ�� ���
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Getup,
	.Start = [=] {
	PhysXCapsule->AddWorldRotation({ 0.0f, -20.0f, 0.0f });
	EnemyRenderer->ChangeAnimation("em0100_downward_down_standup");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	IsCollapse = false;
	}
		});
	// ������ �Ѿ��� ���¿��� Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Death,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_downward_die");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// ������ �����ִ� ���¿��� ����� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_downward_damage", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Downward_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	// ������ �������鼭 ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Death_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_death_back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// �ڷ� �������鼭 ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Death_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_death_front");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      ����Ʈ      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// em0100_Buster_Start, ����Ʈ ��Ʈ ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_hold_pose");
	},
	.Update = [=](float _DeltaTime) {
	SetMoveStop();
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		BusterEnd();
		SetAir(-120000.0f);
		ChangeState(FSM_State_Empusa::Empusa_Buster_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// ����Ʈ ��Ʈ ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Loop,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_Empusa::Empusa_Buster_Finish);
		return;
	}
	},
	.End = [=] {
	}
		});
	// em0100_Buster_Finish, ����Ʈ ��Ʈ ���� ������
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Finish,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Downward_Getup);
		return;
	}
	},
	.End = [=] {

	}
		});

	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);

	if (0 == RandC)
	{
		EnemyFSM.ChangeState(FSM_State_Empusa::Empusa_Appear_A);
		IsAppearA = true;
	}
	else
	{
		EnemyFSM.ChangeState(FSM_State_Empusa::Empusa_Appear_B);
		IsAppearA = false;
	}
}

void Enemy_Empusa::EnemyCreateFSM_Client()
{
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Appear_A,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_enter_ground_A");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Appear_B,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_enter_ground_B");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_idle");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left_90,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_turn_90_left");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right_90,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_turn_90_right");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left_180,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_turn_180_left");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right_180,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_turn_180_right");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_A,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_A");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_B,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_B");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_C,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_C");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_D,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_D");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_W,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_W");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Continuation_Attack,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_continuation_attack");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_front", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_back", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_left", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_right", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Blown_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_blown_back_start", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Air_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_air_damage", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Air_Damage_Under,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_air_damage_under", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Blown_Back_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Blown_Wall_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_blown_wall_back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_fall_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_snatch_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Snatch_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_snatch_end");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_stun_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_stun_revive");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Down,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_downward_down_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Down_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_downward_down_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_downward_down_standup");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Death,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_downward_die");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_downward_damage", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Death_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_death_back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Death_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_death_front");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_hold_pose");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Loop,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Finish,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {

	}
		});

	if (IsAppearA == true)
	{
		EnemyFSM.ChangeState(FSM_State_Empusa::Empusa_Appear_A);
	}
	else
	{
		EnemyFSM.ChangeState(FSM_State_Empusa::Empusa_Appear_B);
	}
}
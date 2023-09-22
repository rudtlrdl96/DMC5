#include "PrecompileHeader.h"
#include "Enemy_HellCaina.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineBase/GameEngineRandom.h>

#include "NetworkManager.h"
#include "AnimationEvent.h"
#include "BasePlayerActor.h"
#include "AttackCollision.h"

Enemy_HellCaina::Enemy_HellCaina()
{
}

Enemy_HellCaina::~Enemy_HellCaina()
{
}

void Enemy_HellCaina::Start()
{
	BaseEnemyActor::Start();

	// 랜더러 크기 설정
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -45.0f, 0.0f });

	// 콜리전 옵션, 크기 설정
	MonsterAttackCollision->SetAttackData(DamageType::Light, 0);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);
	MonsterCollision->GetTransform()->SetLocalScale({ 80, 180, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 50, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 700, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });
	//RN_MonsterCollision->Off();

	// 기본 세팅
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	// 넷 오브젝트 타입 설정
	SetNetObjectType(Net_ActorType::HellCaina);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// 움직임, 히트 관련 ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_HellCaina::PlayerChase(float _DeltaTime)
{
	RotationCheck();

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		AllDirectSetting();
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Start);
		break;
	case EnemyRotation::Left:
		AllDirectSetting();
		ChangeState(FSM_State_HellCaina::HellCaina_Navi_Turn_Left);
		break;
	case EnemyRotation::Left_180:
		ChangeState(FSM_State_HellCaina::HellCaina_Turn_Left_180);
		break;
	case EnemyRotation::Right:
		AllDirectSetting();
		ChangeState(FSM_State_HellCaina::HellCaina_Navi_Turn_Right);
		break;
	case EnemyRotation::Right_180:
		ChangeState(FSM_State_HellCaina::HellCaina_Turn_Right_180);
		break;
	default:
		break;
	}
}

void Enemy_HellCaina::PlayerAttack(float _DeltaTime)
{
	RotationCheck();
	AllDirectSetting();

	if (EnemyRotation::Left == EnemyRotationValue)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Right_Start);
	}
	else if (EnemyRotation::Right == EnemyRotationValue)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Left_Start);
	}
	else if (EnemyRotation::Left_180 == EnemyRotationValue || EnemyRotation::Right_180 == EnemyRotationValue)
	{
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Attack_Turn);
	}
	else
	{
		RandomAttack();
	}
}

void Enemy_HellCaina::RandomAttack()
{
	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);

	if (0 == RandC)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Attack_DownUp);
	}
	else
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Attack_UpDown);
	}
}

void Enemy_HellCaina::DamageCollisionCheck(float _DeltaTime)
{
	AttackDelayCheck += _DeltaTime;

	float FrameTime = (1.0f / 60.0f) * 5.0f;

	if (FrameTime > AttackDelayCheck)
	{
		return;
	}

	if (nullptr == MonsterCollision)
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Col = MonsterCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

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

	switch (Data.DamageTypeValue)
	{
	case DamageType::None:
		return;
		break;
	case DamageType::Light:

		if (true == IsHeavyAttack || true == IsSlamAttack)
		{
			return;
		}

		if (true == IsAirAttack)
		{
			StartRenderShaking(8);
			ChangeState(FSM_State_HellCaina::HellCaina_Air_Damage_Under);
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
			ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Medium:
		break;
	case DamageType::Heavy:
		IsCollapse = false;
		IsHeavyAttack = true;
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back);
		break;
	case DamageType::Air:
		IsCollapse = false;
		IsAirAttack = true;
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Up);
		break;
	case DamageType::Snatch:
		IsCollapse = false;
		IsAirAttack = true;
		StartMonsterSnatch();
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Snatch);
		break;
	case DamageType::Slam:
		IsCollapse = false;
		IsSlamAttack = true;
		ChangeState(FSM_State_HellCaina::HellCaina_Slam_Damage);
		break;
	case DamageType::Buster:
		break;
	case DamageType::Stun:
		break;
	default:
		break;
	}

	AttackDelayCheck = 0.0f;
}

void Enemy_HellCaina::RecognizeCollisionCheck(float _DeltaTime)
{
	if (true == IsRecognize || false == RN_MonsterCollision->IsUpdate())
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Col = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
	if (nullptr == Col) { return; }

	IsRecognize = true;
}

void Enemy_HellCaina::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////   Actor Init   ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_HellCaina::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("em0000.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Character", "Enemy", "HellCaina", "mesh"
			},
			"em0000.FBX"
		);
		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("em0000.fbx", "AniFBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("em0000.fbx", "AniFBX");
	}
	break;
	default:
		break;
	}

	EnemyRenderer->GetTransform()->SetLocalScale({ 0.8f , 0.8f , 0.8f });
}

void Enemy_HellCaina::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::HellCaina;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Small;

	EnemyHP = 0;
	RN_Range = float4::ZERO;
	RN_Player = false;
	MoveSpeed = 50.0f;
}

void Enemy_HellCaina::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("HellCaina");
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
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////     FSM     ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_HellCaina::EnemyCreateFSM()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     등장     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 최초 등장_02
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Appear_02,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_appear_02");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 최초 등장_10, 이건 랜더 직접 옮겨야함
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Appear_10,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_appear_10");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Idle     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Idle,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_Idle_01");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime += _DeltaTime;
	if (0.7f <= WaitTime)
	{
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
	// 앞으로 함성 위협
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Menace_Front,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_menace_01");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 위로 함성 위협
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Menace_Up,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_menace_02");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		RotationCheck();
		AllDirectSetting();
		ChangeState(FSM_State_HellCaina::HellCaina_Attack_Dash);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      Move      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 앞으로 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_start");
	},
	.Update = [=](float _DeltaTime) {
	if (8 < EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(100.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 4);

		if (0 == RandC)
		{
			ChangeState(FSM_State_HellCaina::HellCaina_Menace_Up);
		}
		else
		{
			ChangeState(FSM_State_HellCaina::HellCaina_Walk_Loop);
		}
		return;
	}
	},
	.End = [=] {
	}
		});
	// 앞으로 걷기 Loop

	{
		EnemyRenderer->SetAnimationStartEvent("em0000_walk_loop", 208, [=] { ++WalkCount; });
	}

	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	SetForwardMove(140.0f);

	if (0.5f >= MonsterAndPlayerDotProduct() || true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Stop);
		return;
	}

	if (1 == WalkCount)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Stop);
		return;
	}
	},
	.End = [=] {
	WalkCount = 0;
	}
		});
	// 앞으로 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (48 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 오른쪽 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_start");
	},
	.Update = [=](float _DeltaTime) {
	if (5 < EnemyRenderer->GetCurFrame())
	{
		SetRightMove(35.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Right_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 오른쪽 걷기 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_loop");
	},
	.Update = [=](float _DeltaTime) {
	SetRightMove(70.0f);
	AttackDelayTime += _DeltaTime;
	if (true == IsRecognize && 1.2f <= AttackDelayTime)
	{
		IsRecognize = false;

		if (EnemyRotation::Left_180 == EnemyRotationValue || EnemyRotation::Right_180 == EnemyRotationValue)
		{
			RotationCheck();
			AllDirectSetting();
			ChangeState(FSM_State_HellCaina::HellCaina_Attack_Turn);
		}
		else
		{
			RotationCheck();
			AllDirectSetting();
			RandomAttack();
		}
	}
	else if (false == IsRecognize && 1.8f <= AttackDelayTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Right_Stop);
		return;
	}
	},
	.End = [=] {
	AttackDelayTime = 0.0f;
	WalkCount = 0;
	}
		});
	// 오른쪽 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (30 > EnemyRenderer->GetCurFrame())
	{
		SetRightMove(20.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 왼쪽 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_start");
	},
	.Update = [=](float _DeltaTime) {
	if (5 < EnemyRenderer->GetCurFrame())
	{
		SetLeftMove(35.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Left_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 왼쪽 걷기 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_loop");
	},
	.Update = [=](float _DeltaTime) {
	SetLeftMove(70.0f);
	AttackDelayTime += _DeltaTime;
	if (true == IsRecognize && 1.2f <= AttackDelayTime)
	{
		IsRecognize = false;

		if (EnemyRotation::Left_180 == EnemyRotationValue || EnemyRotation::Right_180 == EnemyRotationValue)
		{
			RotationCheck();
			AllDirectSetting();
			ChangeState(FSM_State_HellCaina::HellCaina_Attack_Turn);
		}
		else
		{
			RotationCheck();
			AllDirectSetting();
			RandomAttack();
		}
	}
	else if (false == IsRecognize && 1.8f <= AttackDelayTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Left_Stop);
		return;
	}
	},
	.End = [=] {
	AttackDelayTime = 0.0f;
	WalkCount = 0;
	}
		});
	// 왼쪽 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (30 > EnemyRenderer->GetCurFrame())
	{
		SetLeftMove(20.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////   Rotation   //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 왼쪽 슬러프 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Navi_Turn_Left,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0000_navi_turn_left_90");
	},
	.Update = [=](float _DeltaTime) {

	SlerpTurn(_DeltaTime);

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// 오른쪽 슬러프 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Navi_Turn_Right,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0000_navi_turn_right_90");
	},
	.Update = [=](float _DeltaTime) {

	SlerpTurn(_DeltaTime);

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// 왼쪽 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Left_180,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_turn_left_180");
	},
	.Update = [=](float _DeltaTime) {
	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, -180.f, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 왼쪽 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Right_180,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_turn_right_180");
	},
	.Update = [=](float _DeltaTime) {
	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     공 격     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	{
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_01", 62, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_01", 63, [=] { SetAdvance(33000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_02", 57, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_02", 58, [=] { SetAdvance(33000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_01_turn", 70, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_01_turn", 71, [=] { SetThrowback(-33000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_atackhard", 116, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_atackhard", 117, [=] { SetAdvance(34000.0f); });
	}

	// 아래에서 위로 횡베기
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_DownUp,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_attack_01");
	},
	.Update = [=](float _DeltaTime) {
	if (EnemyRenderer->GetCurFrame() <= 61)
	{
		SetForwardMove(90.0f);
	}
	if (138 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 178)
	{
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd()) 
	{ 
		ChangeState(FSM_State_HellCaina::HellCaina_Idle); 
		return; 
	}
	},
	.End = [=] {
	}
		});
	// 위에서 아래로 횡베기
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_UpDown,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_attack_02");
	},
	.Update = [=](float _DeltaTime) {
	if (5 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 55)
	{
		SetForwardMove(80.0f);
	}
	if (138 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 160)
	{
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 뒤에 있으면 턴해서 공격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_Turn,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_attack_01_turn");
	},
	.Update = [=](float _DeltaTime) {
	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (10 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 65)
	{
		SetForwardMove(80.0f);
	}
	if (138 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 160)
	{
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 돌진공격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_Dash,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_attack_atackhard");
	},
	.Update = [=](float _DeltaTime) {
	if (10 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 115)
	{
		SetForwardMove(400.0f);
	}
	if (345 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 390)
	{
		SetForwardMove(60.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      피격      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////약공격
	// 정면 약공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front,
	.Start = [=] {
	SetPush(30000.0f);
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_front_01", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 후면 약공격 피격 (뒤로돈다)
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Back,
	.Start = [=] {
	SetPush(30000.0f);
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_back_01", true);
	},
	.Update = [=](float _DeltaTime) {
	if (90 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.0f, 180.0f, 0.0f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 좌측 약공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Left,
	.Start = [=] {
	SetPush(30000.0f);
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_left_01", true);
	},
	.Update = [=](float _DeltaTime) {
	if (90 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.0f, -90.0f, 0.0f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 우측 약공격 피격 
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Right,
	.Start = [=] {
	SetPush(30000.0f);
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_right_01", true);
	},
	.Update = [=](float _DeltaTime) {
	if (90 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.0f, 90.0f, 0.0f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////중공격
	// 정면 중공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Transdamage_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_transdamage_front", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 후면 중공격 피격 (맞고 뒤돌아봄)
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Transdamage_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_transdamage_back", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////강공격
	// 강공격 맞고 날아감
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back,
	.Start = [=] {
	SetPush(50000.0f);
	SetAir(42000.0f);
	EnemyRenderer->ChangeAnimation("em0000_blown_back", true);
	},
	.Update = [=](float _DeltaTime) {
	FallCheckDelayTime += _DeltaTime;
	if (true == FloorCheck(FallDistance) && 0.2f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});
	/////////////////////////에어피격
	// 띄우기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Up,
	.Start = [=] {
	PhysXCapsule->SetAirState(110000.0f);
	EnemyRenderer->ChangeAnimation("em0000_blown_up", true);
	},
	.Update = [=](float _DeltaTime) {

	FallCheckDelayTime += _DeltaTime;

	if (true == FloorCheck(FallDistance) && 0.5f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0000_air_damage_under", 1, [=] { SetAir(30000.0f); });
	}

	// 에어 상태에서 약공격 맞을때
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Air_Damage_Under,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_air_damage_under", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 강공격 피격, 에어피격 끝나고 땅에 닿을때
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0000_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////날아가다가 벽에 박음
	// 날아가다가 벽에 박음
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Wall_Back,
	.Start = [=] {

	EnemyRenderer->ChangeAnimation("em0000_blown_wall_back");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Wall_Back_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 날아가다가 벽에 박은 후 엎어짐
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Wall_Back_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_wall_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////슬램
	
	{
		EnemyRenderer->SetAnimationStartEvent("em0000_slam_damage", 1, [=] { SetAir(-75000.0f); });
	}
	
	// 슬램 피격 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd() || true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Slam_Damage_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 공중 loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Slam_Damage_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 후 땅에 부딫힘
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0000_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스내치
	// 스내치 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_snatch");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Air_Damage_Under);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 스내치 end
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Snatch_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_snatch_end");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Air_Damage_Under);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스턴
	// 스턴 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_stun");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Stun_Revive);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 스턴 end
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_stun_revive");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      쓰러짐      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////// 앞으로 넘어지는 종류
	// 앞으로 넘어짐
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Down,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_down");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 앞으로 엎어졌을 때 일어나는 모션
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_getup");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	IsCollapse = false;
	}
	});
	// 앞으로 넘어진 상태에서 Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Death,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_death");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 앞으로 쓰러지면서 죽음
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Death_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_death_back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 앞으로 누워있는 상태에서 약공격 맞음
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Damage_Gun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_damage_gun", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
	});

	///////////////////////////// 뒤로 넘어지는 종류
	//뒤로 넘어짐
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Lie_Down,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_lie_down");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 뒤로 넘어졌을 때 일어나는 모션
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Lie_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_lie_getup");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	IsCollapse = false;
	}
		});
	// 뒤로 넘어진 상태에서 Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Lie_Death,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_lie_death");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 뒤로 쓰러지면서 죽음
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Death_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_death_front");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 뒤로 누워있는 상태에서 약공격 맞음
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Lie_Damage_Gun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_lie_damage_gun", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	EnemyFSM.ChangeState(FSM_State_HellCaina::HellCaina_Idle);
}

void Enemy_HellCaina::EnemyCreateFSM_Client()
{
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Appear_02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_appear_02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Appear_10,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_appear_10");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_Idle_01");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Menace_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_menace_01");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Menace_Up,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_menace_02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Navi_Turn_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_navi_turn_left_90");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Navi_Turn_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_navi_turn_right_90");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Left_180,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_turn_left_180");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Right_180,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_turn_right_180");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_DownUp,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_attack_01");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_UpDown,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_attack_02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_Turn,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_attack_01_turn");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_Dash,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_attack_atackhard");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_front_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_back_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_left_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_right_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Transdamage_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_transdamage_front", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Transdamage_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_transdamage_back", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_back", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Up,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_up", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Air_Damage_Under,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_air_damage_under", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Wall_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_wall_back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Wall_Back_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_wall_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_snatch");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Snatch_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_snatch_end");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_stun");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_stun_revive");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Down,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_down");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_getup");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Death,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_death");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Death_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_death_back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Damage_Gun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_damage_gun", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Lie_Down,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_lie_down");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Lie_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_lie_getup");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Lie_Death,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_lie_death");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Death_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_death_front");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM_Client.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Lie_Damage_Gun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_lie_damage_gun", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM_Client.ChangeState(FSM_State_HellCaina::HellCaina_Idle);
}
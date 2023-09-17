#include "PrecompileHeader.h"
#include "Enemy_HellCaina.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>

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
	if (false == GameEngineInput::IsKey("MonsterTest"))
	{
		GameEngineInput::CreateKey("MonsterTest", 'M');
		GameEngineInput::CreateKey("MonsterTest2", 'N');
		GameEngineInput::CreateKey("MonsterTest3", 'B');
		GameEngineInput::CreateKey("MonsterTest4", 'V');
	}

	//Render생성
	EnemyRenderer = CreateComponent<GameEngineFBXRenderer>();
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -45.0f, 0.0f });
	//PhysX(충돌)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({ 90, 60, 90 });
	PhysXCapsule->GetDynamic()->setMass(80.0f);
	PhysXCapsule->GetDynamic()->setLinearDamping(1.0f);
	PhysXCapsule->SetWorldPosition({ 0, 100, 0 });

	MonsterAttackCollision = CreateComponent<AttackCollision>(CollisionOrder::EnemyAttack);
	MonsterAttackCollision->SetAttackData(DamageType::Light, 0);
	MonsterAttackCollision->SetColType(ColType::AABBBOX3D);

	MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	MonsterCollision->GetTransform()->SetLocalScale({ 80, 180, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 50, 0 });
	MonsterCollision->SetColType(ColType::AABBBOX3D);

	EnemyMeshLoad();
	EnemyTypeLoad();
	EnemyAnimationLoad();
	EnemyCreateFSM();

	FallDistance = 55.0f;
}
void Enemy_HellCaina::Update(float _DeltaTime)
{
	//PhysXCapsule->SetLinearVelocityZero();
	CollisionDelayTimeCheck(_DeltaTime);
	DamageCollisionCheck();
	EnemyFSM.Update(_DeltaTime);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// 움직임, 히트 관련 ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_HellCaina::PlayerChase(float _DeltaTime)
{
	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Start);
		break;
	case EnemyRotation::Left:
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Start);
		break;
	case EnemyRotation::Left_90:
		ChangeState(FSM_State_HellCaina::HellCaina_Turn_Left_90);
		break;
	case EnemyRotation::Left_180:
		ChangeState(FSM_State_HellCaina::HellCaina_Turn_Left_180);
		break;
	case EnemyRotation::Right:
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Start);
		break;
	case EnemyRotation::Right_90:
		ChangeState(FSM_State_HellCaina::HellCaina_Turn_Right_90);
		break;
	case EnemyRotation::Right_180:
		ChangeState(FSM_State_HellCaina::HellCaina_Turn_Right_180);
		break;
	default:
		break;
	}
}

void Enemy_HellCaina::CollisionDelayTimeCheck(float _DeltaTime)
{
	AttackDelayCheck += _DeltaTime;

	if (0.05f <= AttackDelayCheck)
	{
		AttackCheck = true;
	}
}

void Enemy_HellCaina::DamageCollisionCheck()
{
	if (false == AttackCheck)
	{
		return;
	}

	{
		AttackDelayCheck = 0.0f;
		AttackCheck = false;
	}

	std::shared_ptr<GameEngineCollision> Col = MonsterCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	switch (AttackCol->GetDamageType())
	{
	case DamageType::None:
		return;
		break;
	case DamageType::Light:
		ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front);
		break;
	case DamageType::Medium:
		break;
	case DamageType::Heavy:
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back);
		break;
	case DamageType::Air:
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Up);
		break;
	case DamageType::Snatch:
		break;
	case DamageType::Slam:
		break;
	case DamageType::Buster:
		break;
	case DamageType::Stun:
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////     FSM     ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_HellCaina::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

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
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_Idle_01");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime += _DeltaTime;
	if (2.0f <= WaitTime)
	{
		CheckHeadingRotationValue();
		PlayerChase(_DeltaTime);
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
	PhysXCapsule->SetLinearVelocityZero();
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
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_menace_02");
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
	/////////////////////////////////      Move      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 앞으로 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Start,
	.Start = [=] {
	PhysXCapsule->AddWorldRotation({ 0, RotationValue, 0 });
	MoveDir = GetTransform()->GetWorldForwardVector().NormalizeReturn();
	MoveDir.RotationYDeg(RotationValue); // 한프레임 차이나서 꺾어줘야함
	EnemyRenderer->ChangeAnimation("em0000_walk_start");
	},
	.Update = [=](float _DeltaTime) {
	PhysXCapsule->SetMove(MoveDir * 70.0f);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Loop);
		return;
	}
	},
	.End = [=] {
	}
	});
	// 앞으로 걷기 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	WalkTime += _DeltaTime;
	PhysXCapsule->SetMove(MoveDir * 100.0f);
	if (2.f <= WalkTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Stop);
		return;
	}
	},
	.End = [=] {
	WalkTime = 0.0f;
	}
	});
	// 앞으로 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_stop");
	},
	.Update = [=](float _DeltaTime) {
	PhysXCapsule->SetMove(MoveDir * 50.0f);
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
	PhysXCapsule->AddWorldRotation({ 0, RotationValue, 0 });
	MoveDir = GetTransform()->GetWorldRightVector().NormalizeReturn();
	MoveDir.RotationYDeg(RotationValue); // 한프레임 차이나서 꺾어줘야함
	EnemyRenderer->ChangeAnimation("em0000_walk_right_start");
	},
	.Update = [=](float _DeltaTime) {
	PhysXCapsule->SetMove(MoveDir * 70.0f);
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
	WalkTime += _DeltaTime;
	PhysXCapsule->SetMove(MoveDir * 100.0f);
	if (2.f <= WalkTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Right_Stop);
		return;
	}
	},
	.End = [=] {
	WalkTime = 0.0f;
	}
	});
	// 오른쪽 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_stop");
	},
	.Update = [=](float _DeltaTime) {
	PhysXCapsule->SetMove(MoveDir * 50.0f);
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
	PhysXCapsule->AddWorldRotation({ 0, RotationValue, 0 });
	MoveDir = GetTransform()->GetWorldLeftVector().NormalizeReturn();
	MoveDir.RotationYDeg(RotationValue); // 한프레임 차이나서 꺾어줘야함
	EnemyRenderer->ChangeAnimation("em0000_walk_left_start");
	},
	.Update = [=](float _DeltaTime) {
	PhysXCapsule->SetMove(MoveDir * 70.0f);
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
	WalkTime += _DeltaTime;
	PhysXCapsule->SetMove(MoveDir * 100.0f);
	if (2.f <= WalkTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Left_Stop);
		return;
	}
	},
	.End = [=] {
	WalkTime = 0.0f;
	}
	});
	// 왼쪽 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_stop");
	},
	.Update = [=](float _DeltaTime) {
	PhysXCapsule->SetMove(MoveDir * 50.0f);
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Navi_turn_left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_navi_turn_left_90");
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
	// 오른쪽 슬러프 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Navi_turn_Right,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_navi_turn_right_90");
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

	// 왼쪽 90도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Left_90,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_turn_left_90");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	PhysXCapsule->AddWorldRotation({ 0.f, -90.f, 0.f });
	}
	});
	// 왼쪽 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Left_180,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_turn_left_180");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	PhysXCapsule->AddWorldRotation({ 0.f, -180.f, 0.f });
	}
	});
	// 오른쪽 90도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Right_90,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_turn_right_90");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	PhysXCapsule->AddWorldRotation({ 0.f, 90.f, 0.f });
	}
	});
	// 왼쪽 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Right_180,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_turn_right_180");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
	}
	});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     공 격     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 아래에서 위로 횡베기
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_DownUp,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_attack_01");
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
	// 위에서 아래로 횡베기
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_UpDown,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_attack_02");
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
	// 뒤에 있으면 턴해서 공격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_Turn,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_attack_01_turn");
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
	// 돌진공격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_Dash,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_attack_atackhard");
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
	/////////////////////////////////      피격      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////약공격
	// 정면 약공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front,
	.Start = [=] {
	PushDir = GetTransform()->GetWorldBackVector().NormalizeReturn();
	PhysXCapsule->SetPush(PushDir * 20000.0f);
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
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_back_01", true);
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
	// 좌측 약공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_left_01", true);
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
	// 우측 약공격 피격 
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_right_01", true);
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
	PhysXCapsule->AddWorldRotation({ 0, RotationValue, 0 });
	PushDir = GetTransform()->GetWorldBackVector().NormalizeReturn();
	PushDir.RotationYDeg(RotationValue); // 1프레임 차이나기 때문에 한번 꺾어줘야함
	PhysXCapsule->SetPush(PushDir * 100000.0f);
	PhysXCapsule->SetPush({ 0.0f, 50000.0f, 0.0f});
	EnemyRenderer->ChangeAnimation("em0000_blown_back", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back_Loop);
		return;
	}
	},
	.End = [=] {
	}
	});
	// 강공격 피격 후 땋에 닿기 전 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_back_loop");
	},
	.Update = [=](float _DeltaTime) {
	FallCheckDelayTime += _DeltaTime;
	if (true == FloorCheck(FallDistance) /*&& 0.1f <= FallCheckDelayTime*/)
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
	PhysXCapsule->AddWorldRotation({ 0, RotationValue, 0 });
	PushDir = GetTransform()->GetWorldBackVector().NormalizeReturn();
	PushDir.RotationYDeg(RotationValue); // 1프레임 차이나기 때문에 한번 꺾어줘야함
	PhysXCapsule->SetAirState(100000.0f);
	EnemyRenderer->ChangeAnimation("em0000_blown_up", true);
	},
	.Update = [=](float _DeltaTime) {

	FallCheckDelayTime += _DeltaTime;

	if (true == FloorCheck(FallDistance) && 0.5f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Air_Damage_Under);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
	});
	// 에어 상태에서 약공격 맞을때
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Air_Damage_Under,
	.Start = [=] {
	PhysXCapsule->SetAirState(25000.0f);
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
	// 슬램 피격 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
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
	if (true == EnemyRenderer->IsAnimationEnd())
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
		ChangeState(FSM_State_HellCaina::HellCaina_Snatch_End);
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
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back_Loop);
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
	//앞으로 넘어짐
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
				std::bind([=] {CheckBool = true; }),
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

	//NewDir.MoveParent();
	//NewDir.Move("Animation");

	//std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".FBX" });

	//for (size_t i = 0; i < Files.size(); i++)
	//{
	//	std::string View = Files[i].GetFullPath().c_str();
	//	GameEngineFBXAnimation::Load(Files[i].GetFullPath());
	//	EnemyRenderer->CreateFBXAnimation(Files[i].GetFileName(), {.Inter = 0.0166f, .Loop = false});
	//}
	//
}
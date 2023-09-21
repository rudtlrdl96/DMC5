#include "PrecompileHeader.h"
#include "Enemy_Empusa.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "BaseEnemyActor.h"

#include "AnimationEvent.h"
#include "BasePlayerActor.h"
#include "AttackCollision.h"
#include "NetworkManager.h"

Enemy_Empusa::Enemy_Empusa() 
{
}

Enemy_Empusa::~Enemy_Empusa() 
{
}

void Enemy_Empusa::DamageCollisionCheck(float _DeltaTime)
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
	StartRenderShaking(10);

	DamageData Type = AttackCol->GetDamage();

	switch (Type.DamageTypeValue)
	{
	case DamageType::None:
		return;
		break;
	case DamageType::Light:

		if (true == IsHeavyAttack || true == IsAirLanding)
		{
			return;
		}

		if (true == IsAirAttack)
		{
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

	case DamageType::Medium:
		break;
	case DamageType::Heavy:
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		/*ChangeState(FSM_State_Empusa::);*/
		break;
	case DamageType::Air:
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		/*ChangeState(FSM_State_Empusa::);*/
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

	AttackDelayCheck = 0.0f;
}

void Enemy_Empusa::Start()
{
	BaseEnemyActor::Start();
	SetNetObjectType(Net_ActorType::Empusa);
	ChangeState(FSM_State_Empusa::Empusa_Idle);
}

void Enemy_Empusa::Update(float _DeltaTime)
{
	EnemyFSM.Update(_DeltaTime);
	RecognizeCollisionCheck(_DeltaTime);
}

void Enemy_Empusa::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

void Enemy_Empusa::PlayerChase(float _DeltaTime)
{
	
	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		AllDirectSetting();
		ChangeState(FSM_State_Empusa::Empusa_Walk_Start);
		break;
	case EnemyRotation::Left:
		AllDirectSetting();
		ChangeState(FSM_State_Empusa::Empusa_Turn_Left);
		break;
	case EnemyRotation::Left_180:
		AllDirectSetting();
		ChangeState(FSM_State_Empusa::Empusa_Turn_Left_180);
		break;
	case EnemyRotation::Right:
		AllDirectSetting();
		ChangeState(FSM_State_Empusa::Empusa_Turn_Right);
		break;
	case EnemyRotation::Right_180:
		AllDirectSetting();
		ChangeState(FSM_State_Empusa::Empusa_Turn_Right_180);
		break;
	default:
		break;
	}
}

void Enemy_Empusa::EnemyMeshLoad()
{
	//FBX파일경로를 찾아서 로드
	if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "em0100", "mesh"
			}, "em0100.FBX");

		GameEngineFBXMesh::Load(Path);
	}
	
	//EnemyBase에서 Start에서 생성된 Component에 Mesh Set
	EnemyRenderer->SetFBXMesh("em0100.fbx", "AniFBX_Low");
	EnemyRenderer->GetTransform()->SetLocalScale({ 1.0f , 1.0f , 1.0f });
}

void Enemy_Empusa::EnemyTypeLoad()
{
	//Type설정
	EnemyCodeValue = EnemyCode::Empusa;
	EnemyTypeValue = EnemyType::Normal;
	EnemySizeValue = EnemySize::Middle;
	EnemyHP = 100.0f;
	RN_Range = float4::ZERO;
	RN_Player = false;
	MoveSpeed = 50.0f;
}

void Enemy_Empusa::RecognizeCollisionCheck(float _DeltaTime) 
{
	bool asd = IsRecognize;
	if (true == IsRecognize)
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Col = RN_MonsterCollision->Collision(CollisionOrder::Player);
	if (nullptr == Col) { return; }

	IsRecognize = true;
}

void Enemy_Empusa::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("em0100");
	NewDir.Move("Animation");
	AnimationEvent::LoadAll(
		{
			.Dir = NewDir.GetFullPath().c_str(), .Renderer = EnemyRenderer,
			.Objects = {(GameEngineObject*)MonsterCollision.get()},
			.CallBacks_void = 
			{
				std::bind([=] {CheckBool = true; }),
			},
			.CallBacks_int = {
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)
			},
			.CallBacks_float4 = {
			
			}
		}
	);
}

void Enemy_Empusa::EnemyCreateFSM()
{
	MonsterCollision->GetTransform()->SetWorldScale({300,100,300});
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetWorldScale({300,300,300});
	

	//Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Idle,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_Idle_undetected");
	},
	.Update = [=](float _DeltaTime) {
	PlayerChase(_DeltaTime);
	},
	.End = [=] {}
	});

	/////////////////아기 엠푸사 걷는다////////////////
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Start,
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
		ChangeState(FSM_State_Empusa::Empusa_Walk_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	SetForwardMove(150.0f);
	ChangeState(FSM_State_Empusa::Empusa_Walk_Stop);
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (48 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(100.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////Turn/////////////////////////////////////////////////////////////////////

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0100_turn_90_left");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
		return;
	}
	},
	.End = [=] {
		SlerpTime = 0.0f;
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left_180,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0100_turn_180_left");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
		return;
	}
	},
	.End = [=] {
		SlerpTime = 0.0f;
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0100_turn_90_right");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
		return;
	}
	},
	.End = [=] {
		SlerpTime = 0.0f;
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right_180,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0100_turn_180_right");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
		return;
	}
	},
	.End = [=] {
		SlerpTime = 0.0f;
	}
		});

	////////////////////////////////////////////////Move//////////////////////////////////////////////////////
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Right_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_walk_right_start");
	},
	.Update = [=](float _DeltaTime) {
	SetRightMove(20.0f);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Walk_Right_Loop);
		return;
	}
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Right_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_walk_right_loop");
	},
	.Update = [=](float _DeltaTime) {
	SetRightMove(150.0f);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Walk_Right_Stop);
		return;
	}
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Right_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_walk_right_stop");
	},
	.Update = [=](float _DeltaTime) {
	SetRightMove(25.0f);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
		return;
	}
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Left_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_walk_left_start");
	},
	.Update = [=](float _DeltaTime) {
	SetLeftMove(20.0f);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Walk_Left_Loop);
		return;
	}
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Left_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_walk_left_loop");
	},
	.Update = [=](float _DeltaTime) {
	SetLeftMove(150.0f);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		if (nullptr != RN_MonsterCollision)
		{
			ChangeState(FSM_State_Empusa::Empusa_Walk_Left_Stop);
		}
		return;
	}
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Walk_Left_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_walk_left_stop");
	},
	.Update = [=](float _DeltaTime) {
	SetLeftMove(25.0f);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
		return;
	}
	},
	.End = [=] {}
		});
	/////////////////////////////////////////////Attack////////////////////////////////////////////
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_attack_A,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_A");
	},
	.Update = [=](float _DeltaTime) {
	SetLeftMove(25.0f);
	if (true == EnemyRenderer->IsAnimationEnd() && nullptr!= RN_MonsterCollision)
	{
		//ChangeState(FSM_State_Empusa::Empusa_attack_B);
		return;
	}
	else
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
	}
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_attack_B,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_B");
	},
	.Update = [=](float _DeltaTime) {
	SetLeftMove(25.0f);
	if (true == EnemyRenderer->IsAnimationEnd() && nullptr != RN_MonsterCollision)
	{
		//ChangeState(FSM_State_Empusa::Empusa_attack_C);
		return;
	}
	else
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
	}
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_attack_C,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_C");
	},
	.Update = [=](float _DeltaTime) {
	SetLeftMove(25.0f);
	if (true == EnemyRenderer->IsAnimationEnd() && nullptr != RN_MonsterCollision)
	{
		//ChangeState(FSM_State_Empusa::Empusa_attack_D);
		return;
	}
	else
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
	}
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_attack_D,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_D");
	},
	.Update = [=](float _DeltaTime) {
	SetLeftMove(25.0f);
	if (true == EnemyRenderer->IsAnimationEnd() && nullptr != RN_MonsterCollision)
	{
		//ChangeState(FSM_State_Empusa::Empusa_attack_W);
		return;
	}
	else
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
	}
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_attack_W,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_W");
	},
	.Update = [=](float _DeltaTime) {
	SetLeftMove(25.0f);
	if (true == EnemyRenderer->IsAnimationEnd() && nullptr != RN_MonsterCollision)
	{
		//ChangeState(FSM_State_Empusa::Empusa_attack_A);
		return;
	}
	else
	{
		ChangeState(FSM_State_Empusa::Empusa_Idle);
	}
	},
	.End = [=] {}
		});


	/////////////////////////////////////Damage///////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Front,
	.Start = [=] {
	SetPush(30000.0f);
	EnemyRenderer->ChangeAnimation("em0100_angledamage_front");
	},
	.Update = [=](float _DeltaTime) {
	ChangeState(FSM_State_Empusa::Empusa_Idle);
	return;
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Back,
	.Start = [=] {
	SetPush(30000.0f);
	EnemyRenderer->ChangeAnimation("em0100_angledamage_back");
	},
	.Update = [=](float _DeltaTime) {
	ChangeState(FSM_State_Empusa::Empusa_Idle);
	return;
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Left,
	.Start = [=] {
	SetPush(30000.0f);
	EnemyRenderer->ChangeAnimation("em0100_angledamage_left");
	},
	.Update = [=](float _DeltaTime) {
	ChangeState(FSM_State_Empusa::Empusa_Idle);
	return;
	},
	.End = [=] {}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Right,
	.Start = [=] {
	SetPush(30000.0f);
	EnemyRenderer->ChangeAnimation("em0100_angledamage_right");
	},
	.Update = [=](float _DeltaTime) {
	ChangeState(FSM_State_Empusa::Empusa_Idle);
	return;
	},
	.End = [=] {}
		});
}

void Enemy_Empusa::EnemyCreateFSM_Client()
{

}
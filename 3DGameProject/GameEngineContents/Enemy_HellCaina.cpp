#include "PrecompileHeader.h"
#include "Enemy_HellCaina.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
//#include <GameEngineCore/GameEngineLevel.h>

#include "NetworkManager.h"
#include "AnimationEvent.h"

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

	MonsterAttackCollision = CreateComponent<GameEngineCollision>(CollisionOrder::EnemyAttack);
	MonsterAttackCollision->SetColType(ColType::AABBBOX3D);
	MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	MonsterCollision->GetTransform()->SetLocalScale({ 80, 180, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 50, 0 });
	MonsterCollision->SetColType(ColType::AABBBOX3D);

	EnemyMeshLoad();
	EnemyTypeLoad();
	EnemyAnimationLoad();
	EnemyCreateFSM();
}
void Enemy_HellCaina::Update(float _DeltaTime)
{
	//PhysXCapsule->SetLinearVelocityZero();
	EnemyFSM.Update(_DeltaTime);
}

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

void Enemy_HellCaina::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

bool Enemy_HellCaina::FloorCheck(float _Distance)
{
	float4 StartPosision = GetTransform()->GetWorldPosition();
	float4 Direction = float4::DOWN;
	float4 Results = float4::ZERO; // 레이가 닿은 결과값 궁금하면 이거 사용
	float CheckDistance = _Distance;

	bool IsResults = GetLevel()->RayCast(StartPosision, float4::DOWN, Results, _Distance);

	return IsResults;
		//|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldForwardVector() * 75), float4::DOWN, Point, 100.0f)
		//|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldLeftVector() * 75), float4::DOWN, Point, 100.0f)
		//|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldRightVector() * 75), float4::DOWN, Point, 100.0f)
		//|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldBackVector() * 75), float4::DOWN, Point, 100.0f);
}

void Enemy_HellCaina::EnemyCreateFSM()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////   기본 행동   //////////////////////////////////////////
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
		ChangeState(FSM_State_HellCaina::HellCaina_Attack_DownUp);
		return;
	}
	if (true == GameEngineInput::IsDown("MonsterTest4"))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front);
		return;
	}
	if (true == GameEngineInput::IsDown("MonsterTest3"))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back);
		return;
	}
	if (true == GameEngineInput::IsDown("MonsterTest"))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Buster_Start);
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
	},
	.End = [=] {
	}
	});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     공 격     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 아래에서 위로 횡베기 전 대쉬
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_DownUp_step,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_attack_01_step");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Attack_DownUp);
		return;
	}
	},
	.End = [=] {
	}
	});
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
	/////////////////////////////////    스탠드업    //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 뒤로 엎어졌을 때 일어나는 모션
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Lie_Getup,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
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
	// 앞으로 엎어졌을 때 일어나는 모션
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Getup,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
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

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////    스탠드 피격    //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 정면 약공격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_front_01", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == GameEngineInput::IsDown("MonsterTest4"))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front);
		return;
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
	/////////////////////////////////    날아간다    //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 날아가면서 뒤로 고꾸라짐
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back,
	.Start = [=] {
	float4 PushPower = {0.0f, 50000.0f, 100000.0f};
	PhysXCapsule->SetPush(PushPower);
	EnemyRenderer->ChangeAnimation("em0000_blown_back");
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
	// 날아가면서 뒤로 고꾸라짐 최고점 꺾을때 애니메이션 (?)
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back_Under,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_blown_back_under");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
	});
	// 날아가면서 뒤로 고꾸라짐 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back_Loop,
	.Start = [=] {
	//PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_blown_back_loop");
	},
	.Update = [=](float _DeltaTime) {

		FallCheckDelayTime += _DeltaTime;

	if (true == FloorCheck(55.0f) /*&& 0.1f <= FallCheckDelayTime*/)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Buster_Finish);
		return;
	}
	},
	.End = [=] {
		FallCheckDelayTime = 0.0f;
	}
	});
	// 날아가면서 뒤로 고꾸라짐 랜딩
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back_Landing,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Lie_Getup);
		return;
	}
	},
	.End = [=] {
	}
	});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////    에어 피격    //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 그랩 당했을 때
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_M_Buster,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_M-Buster");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Buster_Finish);
		return;
	}
	},
	.End = [=] {
	}
	});
	// 공중 연속 공격 당할 때
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Air_Buster,
	.Start = [=] {
	PhysXCapsule->SetAirState(25000.0f);
	EnemyRenderer->ChangeAnimation("em0000_Air-Buster", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(55.0f))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Buster_Finish);
		return;
	}
	if (true == GameEngineInput::IsDown("MonsterTest"))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Buster_Start);
		return;
	}
	if (true == GameEngineInput::IsDown("MonsterTest2"))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Air_Buster);
		return;
	}
	},
	.End = [=] {
	}
	});
	// 띄우기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Buster_Start,
	.Start = [=] {
	PhysXCapsule->SetAirState(100000.0f);
	EnemyRenderer->ChangeAnimation("em0000_Buster_Start", true);
	},
	.Update = [=](float _DeltaTime) {

	FallCheckDelayTime += _DeltaTime;

	if (true == FloorCheck(55.0f) && 0.5f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Buster_Finish);
		return;
	}
	if (true == GameEngineInput::IsDown("MonsterTest"))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Buster_Start);
		return;
	}
	if (true == GameEngineInput::IsDown("MonsterTest2"))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Air_Buster);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
	});
	// 띄우기 끝 (바운드)
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Buster_Finish,
	.Start = [=] {
	PhysXCapsule->SetLinearVelocityZero();
	EnemyRenderer->ChangeAnimation("em0000_Buster_Finish");
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

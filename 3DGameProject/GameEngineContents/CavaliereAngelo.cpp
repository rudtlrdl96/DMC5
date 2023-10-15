#include "PrecompileHeader.h"
#include "CavaliereAngelo.h"

#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineBase/GameEngineRandom.h>

#include "BasePlayerActor.h"
#include "NetworkManager.h"
#include "AnimationEvent.h"
#include "AttackCollision.h"

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
	EnemyCodeValue = EnemyCode::HellCaina;
	EnemyHP = 0;
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

	EnemyRenderer->GetTransform()->SetLocalScale({ 0.8f , 0.8f , 0.8f });
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

	//std::vector<GameEngineFile> FBXFiles = NewDir.GetAllFile({ ".FBX" });

	//for (size_t i = 0; i < FBXFiles.size(); i++)
	//{
	//	GameEngineFBXAnimation::Load(FBXFiles[i].GetFullPath());
	//	EnemyRenderer->CreateFBXAnimation(FBXFiles[i].GetFileName(), FBXFiles[i].GetFileName(), {.Inter = 0.01666f,});
	//}
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
	MonsterCollision->GetTransform()->SetLocalScale({ 80, 180, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 50, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 600, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	ParryCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	ParryCollision->SetColType(ColType::OBBBOX3D);
	ParryCollision->GetTransform()->SetLocalScale({ 220, 220, 200 });
	ParryCollision->GetTransform()->SetLocalPosition({ 0, 150, 220 });

	// �⺻ ����
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	//EnemyRenderer->Off();
	//MonsterCollision->Off();
	RN_MonsterCollision->Off();

	// �� ������Ʈ Ÿ�� ����
	SetNetObjectType(Net_ActorType::HellCaina);
}

void CavaliereAngelo::Update(float _DeltaTime)
{
	// �̱�, ����, Ŭ���̾�Ʈ ������� ����ؼ� üũ�Ǿ�� �ϴ� �Լ���
	{
		_DeltaTime *= GetTimeScale();
		EnemyRenderer->SetTimeScale(GetTimeScale());

		if (GetTimeScale() == 0.0f)
		{
			MonsterAttackCollision->Off();
			PhysXCapsule->Off();
		}
		else if (false == PhysXCapsule->IsUpdate())
		{
			PhysXCapsule->On();
		}

		RenderShake(_DeltaTime);
		//MonsterSnatch(_DeltaTime);
		ParryCheck();
	}

	// �̱� �÷����� �� ����
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		RecognizeCollisionCheck(_DeltaTime);
		DamageCollisionCheck(_DeltaTime);
		EnemyFSM.Update(_DeltaTime);
	}
	else
	{
		// ���� �÷����� �� ����
		if (NetControllType::ActiveControll == GetControllType())
		{
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

void CavaliereAngelo::PlayerChase(float _DeltaTime)
{
	RotationCheck();

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		AllDirectSetting();
		//ChangeState(FSM_State_HellCaina::HellCaina_Walk_Start);
		break;
	case EnemyRotation::Left:
		AllDirectSetting();
		//ChangeState(FSM_State_HellCaina::HellCaina_Navi_Turn_Left);
		break;
	case EnemyRotation::Left_90:
		AllDirectSetting();
		//ChangeState(FSM_State_HellCaina::HellCaina_Navi_Turn_Left);
		break;
	case EnemyRotation::Left_180:
		//ChangeState(FSM_State_HellCaina::HellCaina_Turn_Left_180);
		break;
	case EnemyRotation::Right:
		AllDirectSetting();
		//ChangeState(FSM_State_HellCaina::HellCaina_Navi_Turn_Right);
		break;
	case EnemyRotation::Right_90:
		AllDirectSetting();
		//ChangeState(FSM_State_HellCaina::HellCaina_Navi_Turn_Right);
		break;
	case EnemyRotation::Right_180:
		//ChangeState(FSM_State_HellCaina::HellCaina_Turn_Right_180);
		break;
	default:
		break;
	}
}

void CavaliereAngelo::PlayerAttack(float _DeltaTime)
{
	RotationCheck();
	AllDirectSetting();

	if (EnemyRotation::Left == EnemyRotationValue)
	{
		//ChangeState(FSM_State_HellCaina::HellCaina_Walk_Right_Start);
	}
	else if (EnemyRotation::Right == EnemyRotationValue)
	{
		//ChangeState(FSM_State_HellCaina::HellCaina_Walk_Left_Start);
	}
	else if (EnemyRotation::Left_180 == EnemyRotationValue || EnemyRotation::Right_180 == EnemyRotationValue)
	{
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		//ChangeState(FSM_State_HellCaina::HellCaina_Attack_Turn);
	}
	else
	{
		//RandomAttack();
	}
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
			//ChangeState(FSM_State_HellCaina::HellCaina_Air_Damage_Under);
			return;
		}

		if (true == IsCollapse)
		{
			StartRenderShaking(8);
			return;
		}

		AttackCalculation();

		switch (EnemyHitDirValue)
		{
		case EnemyHitDirect::Forward:
			//ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			//ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			//ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			//ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		//IsCollapse = false;
		//IsHeavyAttack = true;
		//RotationCheck();
		//PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		//ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back);
		break;
	case DamageType::Air:
		//IsCollapse = false;
		//IsAirAttack = true;
		//RotationCheck();
		//PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		//ChangeState(FSM_State_HellCaina::HellCaina_Blown_Up);
		break;
	case DamageType::Snatch:
		//IsCollapse = false;
		//IsAirAttack = true;
		//StartMonsterSnatch();
		//RotationCheck();
		//PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		//ChangeState(FSM_State_HellCaina::HellCaina_Snatch);
		break;
	case DamageType::Slam:
		//IsCollapse = false;
		//IsSlamAttack = true;
		//ChangeState(FSM_State_HellCaina::HellCaina_Slam_Damage);
		break;
	case DamageType::Buster:
		//IsCollapse = false;
		//IsBusterAttack = true;
		//BusterCalculation();
		//RotationCheck();
		//PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		//PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		//ChangeState(FSM_State_HellCaina::HellCaina_Buster_Start);
		break;
	case DamageType::Stun:
		break;
	default:
		break;
	}

	IsVergilLight = false;
	AttackDelayCheck = 0.0f;
}

void CavaliereAngelo::DamageCollisionCheck_Client(float _DeltaTime)
{

}

void CavaliereAngelo::RecognizeCollisionCheck(float _DeltaTime)
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

void CavaliereAngelo::ParryCheck()
{
	if (false == IsParryCheck || true == ParryOkay)
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Col = ParryCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	ParryOkay = true;
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
	WaitTime += _DeltaTime;
	if (WaitTime >= 1.0f)
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01);
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
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_End);
		return;
	}
	},
	.End = [=] {
	}
	});

	// ���� ���� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_End,
	.Start = [=] {
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
	}
	});

	// ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_warp_start");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);
		if (0 == RandC)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Start);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_End);
		}
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

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Move     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ���ִٰ� ����ڼ� ���ϸ鼭 �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Attack_To_Guard_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_start_L0");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop);
		return;
	}
	},
	.End = [=] {
	}
	});

	// ����ڼ� �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_to_walk");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop);
		return;
	}
	},
	.End = [=] {
	}
	});

	// ����ڼ� �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		if (true)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Start);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_End);
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
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Start");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});

	// Dengeki �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_End);
		return;
	}
	},
	.End = [=] {
	}
		});

	// Dengeki �ȱ� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_End");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Attack     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 01(���ߴ�) ����, ������ ����ڼ�, 66������ on, 69������ off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack01_R");
	},
	.Update = [=](float _DeltaTime) {

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
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
	});

	// 02(�����) ����, ������ ����ڼ�, 44������ on, 47������ off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack02");
	if (true == Event01)
	{
		Event01 = false;
		EnemyRenderer->SetCurFrame(38);
	}
	if (true == Normal01)
	{
		Normal01 = false;
		EnemyRenderer->SetCurFrame(38);
	}
	},
	.Update = [=](float _DeltaTime) {

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
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 03(���ߴ�) ����, ������ ����ڼ�, 29������ on, 33������ off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack03");
	if (true == Event01)
	{
		Event01 = false;
		EnemyRenderer->SetCurFrame(23);
	}
	if (true == Normal01)
	{
		Normal01 = false;
		EnemyRenderer->SetCurFrame(23);
	}
	},
	.Update = [=](float _DeltaTime) {

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
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// �÷��̾� �и� ���� �� �ٷ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);

		if (0 == RandC)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_02);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_03);
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
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);

		if (0 == RandC)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_02);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_03);
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
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Event01 = true;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 01 �и� �� �ٷ� 03 ���� ��, 24 ������ ��ŸƮ (23 ���������� ����)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01_to_04");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Event01 = true;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 01 �и� �� ����, ���� 02 ���� ��, 37������ ��ŸƮ (38 ���������� ����)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_02");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Normal01 = true;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 01 �и� �� ����, ���� 03 ���� ��, 24 ������ ��ŸƮ (23 ���������� ����)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_03");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Normal01 = true;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03);
		return;
	}
	},
	.End = [=] {
	}
		});

	//// 01 �и� �� ����, ���� C1 ���� ��, 62 ������ ��ŸƮ
	//EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_C1,
	//.Start = [=] {
	//EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_C1");
	//},
	//.Update = [=](float _DeltaTime) {
	//if (true == EnemyRenderer->IsAnimationEnd())
	//{
	//	Normal01 = true;
	//	ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03);
	//	return;
	//}
	//},
	//.End = [=] {
	//}
	//	});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Damage     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// �������(�帱) �°� ���� (������ ���� �� ��), ���� �ڼ� ȸ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Damage_Drill");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	ParryStack = 0;
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

	EnemyFSM.ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
}
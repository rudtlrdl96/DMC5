#include "PrecompileHeader.h"
#include "Enemy_HellAntenora.h"

#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineBase/GameEngineRandom.h>

#include "NetworkManager.h"
#include "AnimationEvent.h"
#include "BasePlayerActor.h"
#include "AttackCollision.h"

Enemy_HellAntenora::Enemy_HellAntenora() 
{
}

Enemy_HellAntenora::~Enemy_HellAntenora() 
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////   Actor Init   ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_HellAntenora::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::HellAntenora;
	EnemyHP = 200000;
}

void Enemy_HellAntenora::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("em0001.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "HellAntenora", "mesh"
			}, 
			"em0001.FBX");

		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("em0001.fbx", "AniFBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("em0001.fbx", "AniFBX");
	}
	break;
	default:
		break;
	}

	LeftWeapon = CreateComponent<GameEngineFBXRenderer>();
	RightWeapon = CreateComponent<GameEngineFBXRenderer>();
	
	if (nullptr == GameEngineFBXMesh::Find("wpem0001_00.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "HellAntenora", "mesh"
			},
			"wpem0001_00.FBX");

		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		LeftWeapon->SetFBXMesh("wpem0001_00.fbx", "FBX_Low");
		RightWeapon->SetFBXMesh("wpem0001_00.fbx", "FBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		LeftWeapon->SetFBXMesh("wpem0001_00.fbx", "FBX");
		RightWeapon->SetFBXMesh("wpem0001_00.fbx", "FBX");
	}
	break;
	default:
		break;
	}
	
	LeftWeapon->ShadowOn();
	LeftWeapon->SetDynamic();

	RightWeapon->ShadowOn();
	RightWeapon->SetDynamic();

	if (nullptr == GameEngineTexture::Find("PaperBurnNoise.jpg"))
	{
		GameEngineTexture::Load(GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Texture", "BurnNoiseTexture"
			}, "PaperBurnNoise.jpg"));
	}

	EnemyRenderer->SetTexture("PaperBurnTexture", "PaperBurnNoise.jpg");
	LeftWeapon->SetTexture("PaperBurnTexture", "PaperBurnNoise.jpg");
	RightWeapon->SetTexture("PaperBurnTexture", "PaperBurnNoise.jpg");
}

void Enemy_HellAntenora::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("HellAntenora");
	NewDir.Move("Animation");
	NewDir.Move("damage");

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

	//std::vector<GameEngineFile> FBXFiles = NewDir.GetAllFile({ ".FBX" });

	//for (size_t i = 0; i < FBXFiles.size(); i++)
	//{
	//	GameEngineFBXAnimation::Load(FBXFiles[i].GetFullPath());
	//	EnemyRenderer->CreateFBXAnimation(FBXFiles[i].GetFileName(), FBXFiles[i].GetFileName(), { .Inter = 0.01666f, });
	//}
}

void Enemy_HellAntenora::Start()
{
	BaseEnemyActor::Start();

	//PhysX(충돌)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({ 90, 120, 90 });
	PhysXCapsule->GetDynamic()->setMass(80.0f);
	BindPhysicsWithNet(PhysXCapsule);

	// 랜더러 크기 설정
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -45.0f, 0.0f });

	// 콜리전 옵션, 크기 설정
	MonsterAttackCollision->SetAttackData(DamageType::Light, 100);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);

	MonsterAttackCollision_Two = CreateComponent<AttackCollision>(CollisionOrder::EnemyAttack);
	MonsterAttackCollision_Two->GetTransform()->SetWorldScale(float4::ZERO);
	MonsterAttackCollision_Two->SetAttackData(DamageType::Light, 100);
	MonsterAttackCollision_Two->SetColType(ColType::OBBBOX3D);

	MonsterCollision->GetTransform()->SetLocalScale({ 80, 210, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 65, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 600, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	// 기본 세팅
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	//MonsterCollision->Off();
	RN_MonsterCollision->Off();

	// 무기 붙이기
	EnemyRenderer->SetAttachTransform("L_WeaponHand", LeftWeapon->GetTransform(), float4(0.0f, 0.0f, 0.0f), float4(0.0f, 170.0f, 180.0f), true);
	EnemyRenderer->SetAttachTransform("R_WeaponHand", RightWeapon->GetTransform(), float4(0.0f, 0.0f, 0.0f), float4(0.0f, 170.0f, 180.0f), true);

	float4 MeshScale_L = LeftWeapon->GetMeshScale();
	// MeshScale = { 77.6 , 10.5 , 171.0 }
	MeshScale_L.x *= 0.7f;
	MeshScale_L.z *= 0.6f;
	float4 MeshScale_R = RightWeapon->GetMeshScale();
	MeshScale_R.x *= 0.7f;
	MeshScale_R.z *= 0.6f;

	MonsterAttackCollision->GetTransform()->SetWorldScale(MeshScale_L);
	MonsterAttackCollision_Two->GetTransform()->SetWorldScale(MeshScale_R);

	MonsterAttackCollision->GetTransform()->SetParent(LeftWeapon->GetTransform());
	MonsterAttackCollision_Two->GetTransform()->SetParent(RightWeapon->GetTransform());

	MonsterAttackCollision->GetTransform()->AddLocalPosition(float4{0.0f, 0.0f, 80.0f});
	MonsterAttackCollision_Two->GetTransform()->AddLocalPosition(float4{0.0f, 0.0f, 80.0f});

	MonsterAttackCollision->GetTransform()->AddLocalRotation(float4{ 0.0f, -10.0f, 0.0f });
	MonsterAttackCollision_Two->GetTransform()->AddLocalRotation(float4{ 0.0f, -10.0f, 0.0f });

	MonsterAttackCollision->Off();
	MonsterAttackCollision_Two->Off();

	// 넷 오브젝트 타입 설정1
	SetNetObjectType(Net_ActorType::HellAntenora);

	LinkData_UpdatePacket<bool>(AnimationTurnStart);
	LinkData_UpdatePacket<bool>(IsHeavyAttack);
	LinkData_UpdatePacket<bool>(IsAirAttack);
	LinkData_UpdatePacket<bool>(IsSlamAttack);
	LinkData_UpdatePacket<bool>(IsBusterAttack);
	LinkData_UpdatePacket<bool>(IsVergilLight);
	LinkData_UpdatePacket<bool>(IsCollapse);
	LinkData_UpdatePacket<bool>(IsBurn);
	LinkData_UpdatePacket<int>(EnemyHP);

	SetDamagedNetCallBack<BasePlayerActor>([this](BasePlayerActor* _Attacker) {
		Player = _Attacker;
		DamageData Datas = _Attacker->GetAttackCollision()->GetDamage();
		MinusEnemyHP(Datas.DamageValue);

		if (DamageType::VergilLight == Datas.DamageTypeValue)
		{
			IsVergilLight = true;
		}

		if (DamageType::Stun == Datas.DamageTypeValue)
		{
			StopTime(2.9f);
		}

		HitStop(Datas.DamageTypeValue);

		if (EnemyHP < 0)
		{
			DeathValue = true;
		}

		});
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// 움직임, 히트 관련 ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_HellAntenora::MonsterAttackCollisionOn()
{
	MonsterAttackCollision->On();
	MonsterAttackCollision_Two->On();
}

void Enemy_HellAntenora::MonsterAttackCollisionOff()
{
	MonsterAttackCollision->Off();
	MonsterAttackCollision_Two->Off();
}

void Enemy_HellAntenora::DeathCheck()
{
	if (EnemyHP <= 0)
	{
		DeathValue = true;
	}

	if (true == DeathValue && false == DeathSettig)
	{
		DeathSettig = true;
		MonsterCollision->Off();
		RN_MonsterCollision->Off();
		PhysXCapsule->Off();
	}
}

void Enemy_HellAntenora::PlayerChase(float _DeltaTime)
{
	RotationCheck();

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
	{
		AllDirectSetting();
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			//ChangeState(FSM_State_HellAntenora::Empusa_Biped_Walk_Start);
		}
		else
		{
			//ChangeState(FSM_State_HellAntenora::Empusa_Biped_Run_Start);
		}
	}
	break;
	case EnemyRotation::Left:
	{
		AllDirectSetting();
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			//ChangeState(FSM_State_HellAntenora::Empusa_Biped_Walk_Start);
		}
		else
		{
			//ChangeState(FSM_State_HellAntenora::Empusa_Biped_Run_Start);
		}
	}
	break;
	case EnemyRotation::Left_90:
		//ChangeState(FSM_State_HellAntenora::Empusa_Turn_Left_90);
		break;
	case EnemyRotation::Left_180:
		//ChangeState(FSM_State_HellAntenora::Empusa_Turn_Left_180);
		break;
	case EnemyRotation::Right:
	{
		AllDirectSetting();
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			//ChangeState(FSM_State_HellAntenora::Empusa_Biped_Walk_Start);
		}
		else
		{
			//ChangeState(FSM_State_HellAntenora::Empusa_Biped_Run_Start);
		}
	}
	break;
	case EnemyRotation::Right_90:
		//ChangeState(FSM_State_HellAntenora::Empusa_Turn_Right_90);
		break;
	case EnemyRotation::Right_180:
		//ChangeState(FSM_State_HellAntenora::Empusa_Turn_Right_180);
		break;
	default:
		break;
	}
}

void Enemy_HellAntenora::PlayerAttack(float _DeltaTime)
{
	RotationCheck();
	AllDirectSetting();

	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

	if (0 == RandC)
	{
		//ChangeState(FSM_State_HellAntenora::Empusa_Continuation_Attack);
		return;
	}

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		RandomAttack();
		break;
	case EnemyRotation::Left:
		//ChangeState(FSM_State_HellAntenora::Empusa_Attack_C);
		break;
	case EnemyRotation::Left_90:
		//ChangeState(FSM_State_HellAntenora::Empusa_Attack_C);
		break;
	case EnemyRotation::Left_180:
		//ChangeState(FSM_State_HellAntenora::Empusa_Attack_C);
		break;
	case EnemyRotation::Right:
		//ChangeState(FSM_State_HellAntenora::Empusa_Attack_D);
		break;
	case EnemyRotation::Right_90:
		//ChangeState(FSM_State_HellAntenora::Empusa_Attack_D);
		break;
	case EnemyRotation::Right_180:
		//ChangeState(FSM_State_HellAntenora::Empusa_Attack_D);
		break;
	default:
		break;
	}
}

void Enemy_HellAntenora::RandomAttack()
{
	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);

	if (0 == RandC)
	{
		//ChangeState(FSM_State_HellAntenora::Empusa_Attack_A);
	}
	else if (1 == RandC)
	{
		//ChangeState(FSM_State_HellAntenora::Empusa_Attack_B);
	}
	else if (2 == RandC)
	{
		//ChangeState(FSM_State_HellAntenora::Empusa_Attack_W);
	}
}

void Enemy_HellAntenora::AttackCalculation()
{
	AttackDirectCheck();

	if (true == AnimationTurnStart)
	{
		AnimationTurnStart = false;
		RotationCheck();
		AllDirectSetting();
		EnemyHitDirValue = EnemyHitDirect::Forward;
	}

	PushDirectSetting();
}

void Enemy_HellAntenora::DamageCollisionCheck(float _DeltaTime)
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
	MinusEnemyHP(Data.DamageValue);

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

		if (true == IsBusterAttack)
		{
			return;
		}

		if (true == IsAirAttack || true == IsSlamAttack || true == IsHeavyAttack)
		{
			StartRenderShaking(8);
			ChangeState(FSM_State_HellAntenora::HellAntenora_Air_Damage_Under);
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
			ChangeState(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			ChangeState(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			ChangeState(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			ChangeState(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Back);
		break;
	case DamageType::Air:
		ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Up);
		break;
	case DamageType::Snatch:
		ChangeState(FSM_State_HellAntenora::HellAntenora_Snatch);
		break;
	case DamageType::Slam:
		ChangeState(FSM_State_HellAntenora::HellAntenora_Slam_Damage);
		break;
	case DamageType::Buster:
		ChangeState(FSM_State_HellAntenora::HellAntenora_Buster_Start);
		break;
	case DamageType::Stun:
		StopTime(2.9f);
		break;
	default:
		break;
	}

	HitStop(Data.DamageTypeValue);
	AttackDelayCheck = 0.0f;
}

void Enemy_HellAntenora::DamageCollisionCheck_Client(float _DeltaTime)
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

		if (true == IsBusterAttack)
		{
			return;
		}

		if (true == IsAirAttack || true == IsSlamAttack || true == IsHeavyAttack)
		{
			StartRenderShaking(8);
			ChangeState(FSM_State_HellAntenora::HellAntenora_Air_Damage_Under);
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
			ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Blown_Back);
		break;
	case DamageType::Air:
		ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Blown_Up);
		break;
	case DamageType::Snatch:
		ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Snatch);
		break;
	case DamageType::Slam:
		ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Slam_Damage);
		break;
	case DamageType::Buster:
		ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Buster_Start);
		break;
	case DamageType::Stun:
		StopTime(2.9f);
		break;
	default:
		break;
	}

	AttackDelayCheck = 0.0f;
}

void Enemy_HellAntenora::RecognizeCollisionCheck(float _DeltaTime)
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////     FSM     ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_HellAntenora::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

void Enemy_HellAntenora::ChangeState_Client(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, Player);
}

void Enemy_HellAntenora::EnemyCreateFSM()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Basic     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_Idle_01_loop");
	},
	.Update = [=](float _DeltaTime) 
	{
	WaitTime += _DeltaTime;
	//if (0.5f <= WaitTime)
	//{
	//	ChangeState(FSM_State_HellAntenora::HellAntenora_Counter_Attack);
	//	return;
	//}
	},
	.End = [=] {
	WaitTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      피격      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////약공격
	// 정면 약공 히트
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Front,
	.Start = [=] {

	AttackCalculation();

	if (true == IsVergilLight)
	{
		IsVergilLight = false;
		SetPush(10000.0f);
	}
	else
	{
		SetPush(24000.0f);
	}

	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_front_01", true);
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Death_Front);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 뒤에서 약공 히트시, 105 프레임 애니메이션 턴
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Back,
	.Start = [=] {

	AttackCalculation();

	if (true == IsVergilLight)
	{
		IsVergilLight = false;
		SetPush(10000.0f);
	}
	else
	{
		SetPush(24000.0f);
	}
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_back_01", true);
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Death_Back);
		return;
	}
	if (105 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.0f, 180.0f, 0.0f });
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 왼쪽 약공 히트, 115 프레임 애니메이션 턴
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Left,
	.Start = [=] {

	AttackCalculation();

	if (true == IsVergilLight)
	{
		IsVergilLight = false;
		SetPush(10000.0f);
	}
	else
	{
		SetPush(24000.0f);
	}
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_left_01", true);
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Death_Front);
		return;
	}
	if (115 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.0f, -90.0f, 0.0f });
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 오른쪽 약공 히트, 100 프레임 애니메이션 턴
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Right,
	.Start = [=] {

	AttackCalculation();

	if (true == IsVergilLight)
	{
		IsVergilLight = false;
		SetPush(10000.0f);
	}
	else
	{
		SetPush(24000.0f);
	}
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_right", true);
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Death_Front);
		return;
	}
	if (90 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.0f, 90.0f, 0.0f });
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////강공격
	// 헤비어택 히트시
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Blown_Back,
	.Start = [=] {
	IsCollapse = false;
	IsHeavyAttack = true;
	AttackCalculation();
	RotationCheck();
	AllDirectSetting();
	SetPush(50000.0f);
	SetAir(42000.0f);
	EnemyRenderer->ChangeAnimation("em0001_blown_back", true);
	},
	.Update = [=](float _DeltaTime) {
	FallCheckDelayTime += _DeltaTime;
	if (true == FloorCheck(FallDistance) && 0.2f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});

	// 헤비어택 랜딩
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Blown_Back_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0001_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////에어피격
	// 띄우기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Blown_Up,
	.Start = [=] {
	IsCollapse = false;
	IsAirAttack = true;
	AttackCalculation();
	RotationCheck();
	AllDirectSetting();
	PhysXCapsule->SetAirState(110000.0f);
	EnemyRenderer->ChangeAnimation("em0001_blown_up", true);
	},
	.Update = [=](float _DeltaTime) {

	FallCheckDelayTime += _DeltaTime;

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Up_Loop);
		return;
	}

	if (true == FloorCheck(FallDistance) && 0.5f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0001_air_damage_gun", 1, [=] {
			if (true == IsVergilLight)
			{
				IsVergilLight = false;
				SetAir(10000.0f);
			}
			else
			{
				SetAir(30000.0f);
			}
			});
	}

	// 에어 상태에서 약공격 맞을때
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Air_Damage_Under,
	.Start = [=] {
	AttackCalculation();
	EnemyRenderer->ChangeAnimation("em0001_air_damage_gun", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Up_Loop);
		return;
	}

	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 에어어택 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Blown_Up_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_blown_up_loop", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 에어어택 착지 시 앉은 상태이지만 비틀거리다가 쓰러지는 랜딩
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Blown_Up_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0001_blown_up_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////슬램

	{
		EnemyRenderer->SetAnimationStartEvent("em0001_slam_damage", 1, [=] { SetAir(-120000.0f); });
	}

	// 슬램 피격 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Slam_Damage,
	.Start = [=] {
	IsCollapse = false;
	IsSlamAttack = true;
	AttackCalculation();
	EnemyRenderer->ChangeAnimation("em0001_slam_damage");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd() || true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Slam_Damage_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 공중 loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_slam_damage_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Slam_Damage_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 후 땅에 부딫힘
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Slam_Damage_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0001_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스내치
	// 스내치 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Snatch,
	.Start = [=] {
	IsCollapse = false;
	IsAirAttack = true;
	AttackCalculation();
	StartMonsterSnatch();
	RotationCheck();
	AllDirectSetting();
	EnemyRenderer->ChangeAnimation("em0001_snatch");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Air_Damage_Under);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스턴
	// 스턴 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_stun");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Stun_Revive);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 스턴 end
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_stun_revive");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      쓰러짐      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// 앞으로 엎어졌을 때 일어나는 모션
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Prone_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_prone_getup");
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Prone_Death);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	IsCollapse = false;
	}
		});
	// 앞으로 넘어진 상태에서 Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Prone_Death,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_prone_death");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsBurn = true;
	}
	},
	.End = [=] {
	}
		});
	// 앞으로 쓰러지면서 죽음
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Death_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_death_back");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsBurn = true;
	}
	},
	.End = [=] {
	}
		});
	// 뒤로 쓰러지면서 죽음
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Death_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_death_front");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsBurn = true;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      버스트      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// em0000_Buster_Start, 버스트 히트 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Buster_Start,
	.Start = [=] {
	AttackCalculation();
	IsCollapse = false;
	IsBusterAttack = true;
	BusterCalculation(float4{ 0.0f, -120.0f, 0.0f });
	RotationCheck();
	AllDirectSetting();
	//PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
	EnemyRenderer->ChangeAnimation("em0001_buster_loop");
	},
	.Update = [=](float _DeltaTime) {
	SetMoveStop();
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		BusterEnd();
		SetAir(-120000.0f);
		ChangeState(FSM_State_HellAntenora::HellAntenora_Buster_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 버스트 히트 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Buster_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_slam_damage_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Buster_Finish);
		return;
	}
	},
	.End = [=] {
	}
		});
	// em0000_Buster_Finish, 버스트 히트 땅에 떨어짐
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Buster_Finish,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0001_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		//PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellAntenora::HellAntenora_Prone_Getup);
		return;
	}
	},
	.End = [=] {

	}
		});


	EnemyFSM.ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
}

void Enemy_HellAntenora::EnemyCreateFSM_Client()
{
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_Idle_01_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
}
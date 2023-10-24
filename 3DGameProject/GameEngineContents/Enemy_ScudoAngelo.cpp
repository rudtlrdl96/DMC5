#include "PreCompileHeader.h"
#include "Enemy_ScudoAngelo.h"

#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineBase/GameEngineRandom.h>

#include "NetworkManager.h"
#include "AnimationEvent.h"
#include "BasePlayerActor.h"
#include "AttackCollision.h"
#include "FXSystem.h"

Enemy_ScudoAngelo::Enemy_ScudoAngelo()
{
}

Enemy_ScudoAngelo::~Enemy_ScudoAngelo()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////   Actor Init   ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_ScudoAngelo::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::ScudoAngelo;
	EnemyMaxHP = 200000;
	EnemyHP = EnemyMaxHP;
}

void Enemy_ScudoAngelo::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("Scudo.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "Scudo", "mesh"
			},
			"Scudo.FBX");

		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("Scudo.fbx", "AniFBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("Scudo.fbx", "AniFBX");
	}
	break;
	default:
		break;
	}

	LeftWeapon = CreateComponent<GameEngineFBXRenderer>();  // 방패
	RightWeapon = CreateComponent<GameEngineFBXRenderer>(); // 검

	if (nullptr == GameEngineFBXMesh::Find("wpem0600_00.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "Scudo", "mesh"
			},
			"wpem0600_00.FBX");

		GameEngineFBXMesh::Load(Path);
	}

	if (nullptr == GameEngineFBXMesh::Find("wpem0600_01.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "Scudo", "mesh"
			},
			"wpem0600_01.FBX");

		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		LeftWeapon->SetFBXMesh("wpem0600_01.fbx", "FBX_Low");
		RightWeapon->SetFBXMesh("wpem0600_00.fbx", "FBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		LeftWeapon->SetFBXMesh("wpem0600_01.fbx", "FBX");
		RightWeapon->SetFBXMesh("wpem0600_00.fbx", "FBX");
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

void Enemy_ScudoAngelo::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("Scudo");
	NewDir.Move("Animation");
	NewDir.Move("basic");

	//AnimationEvent::LoadAll
	//(
	//	{
	//		.Dir = NewDir.GetFullPath().c_str(),
	//		.Renderer = EnemyRenderer,
	//		.RendererLocalPos = { 0.0f, -50.0f, 0.0f },
	//		.Objects = {(GameEngineObject*)MonsterAttackCollision.get()},
	//		.CallBacks_void =
	//		{
	//		},
	//		.CallBacks_int =
	//		{
	//			std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)
	//		},
	//		.CallBacks_float4 =
	//		{
	//		}
	//	}
	//);
	{
		// 이펙트 시스템 생성
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Effect");
		NewDir.Move("Enemy");
		std::vector<GameEngineFile> FXFiles = NewDir.GetAllFile({ ".Effect" });

		EffectRenderer = CreateComponent<FXSystem>();
		for (size_t i = 0; i < FXFiles.size(); i++)
		{
			if (nullptr == FXData::Find(FXFiles[i].GetFileName()))
			{
				FXData::Load(FXFiles[i].GetFullPath());
			}
			EffectRenderer->CreateFX(FXData::Find(FXFiles[i].GetFileName()));
		}
	}
	std::vector<GameEngineFile> FBXFiles = NewDir.GetAllFile({ ".FBX" });

	for (size_t i = 0; i < FBXFiles.size(); i++)
	{
		GameEngineFBXAnimation::Load(FBXFiles[i].GetFullPath());
		EnemyRenderer->CreateFBXAnimation(FBXFiles[i].GetFileName(), FBXFiles[i].GetFileName(), { .Inter = 0.01666f, });
	}
}

void Enemy_ScudoAngelo::Start()
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

	//LeftWeapon->GetTransform()->AddLocalRotation({ 0.0f, 180.0f, 0.0f });
	//LeftWeapon->GetTransform()->AddLocalRotation({ -90.0f, 0.0f, 0.0f });

	// 무기 붙이기
	EnemyRenderer->SetAttachTransform("L_Hand", LeftWeapon->GetTransform(), float4(0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 180.0f), true);
	EnemyRenderer->SetAttachTransform("R_WeaponHand", RightWeapon->GetTransform(), float4(0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f), true);

	float4 MeshScale_R = RightWeapon->GetMeshScale();
	MeshScale_R.x *= 0.7f;
	MeshScale_R.z *= 0.6f;

	MonsterAttackCollision->GetTransform()->SetWorldScale(MeshScale_R);
	MonsterAttackCollision->GetTransform()->SetParent(RightWeapon->GetTransform());
	MonsterAttackCollision->GetTransform()->AddLocalPosition(float4{ 0.0f, 0.0f, 100.0f });
	MonsterAttackCollision->GetTransform()->AddLocalRotation(float4{ 0.0f, 0.0f, 0.0f });

	MonsterAttackCollision->Off();
	MonsterAttackCollision_Two->Off();

	// 넷 오브젝트 타입 설정1
	SetNetObjectType(Net_ActorType::ScudoAngelo);

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
			AttackDelayCheck = 1.0f;
		}

		HitStop(Datas.DamageTypeValue);

		if (EnemyHP < 0)
		{
			DeathValue = true;
		}

		IsChangeState = true;
		});
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// 움직임, 히트 관련 ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_ScudoAngelo::MonsterAttackCollisionOn()
{
	MonsterAttackCollision->On();
	MonsterAttackCollision_Two->On();
}

void Enemy_ScudoAngelo::MonsterAttackCollisionOff()
{
	MonsterAttackCollision->Off();
	MonsterAttackCollision_Two->Off();
}

void Enemy_ScudoAngelo::PlayerChase(float _DeltaTime)
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

void Enemy_ScudoAngelo::PlayerAttack(float _DeltaTime)
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

void Enemy_ScudoAngelo::RandomAttack()
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

void Enemy_ScudoAngelo::AttackCalculation()
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

void Enemy_ScudoAngelo::DamageCollisionCheck(float _DeltaTime)
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
			//ChangeState(FSM_State_HellAntenora::HellAntenora_Air_Damage_Under);
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
			//ChangeState(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			//ChangeState(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			//ChangeState(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			//ChangeState(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		//ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Back);
		break;
	case DamageType::Air:
		//ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Up);
		break;
	case DamageType::Snatch:
		//ChangeState(FSM_State_HellAntenora::HellAntenora_Snatch);
		break;
	case DamageType::Slam:
		//ChangeState(FSM_State_HellAntenora::HellAntenora_Slam_Damage);
		break;
	case DamageType::Buster:
		//ChangeState(FSM_State_HellAntenora::HellAntenora_Buster_Start);
		break;
	case DamageType::Stun:
		StopTime(2.9f);
		return;
	default:
		break;
	}

	HitStop(Data.DamageTypeValue);
	AttackDelayCheck = 0.0f;
}

void Enemy_ScudoAngelo::DamageCollisionCheck_Client(float _DeltaTime)
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
			//ChangeState_Client(FSM_State_HellAntenora::Empusa_Air_Damage_Under);
			return;
		}

		if (true == IsCollapse)
		{
			StartRenderShaking(8);
			return;
		}

		AttackDirectCheck();

		switch (EnemyHitDirValue)
		{
		case EnemyHitDirect::Forward:
			//ChangeState_Client(FSM_State_HellAntenora::Empusa_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			//ChangeState_Client(FSM_State_HellAntenora::Empusa_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			//ChangeState_Client(FSM_State_HellAntenora::Empusa_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			//ChangeState_Client(FSM_State_HellAntenora::Empusa_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		//ChangeState_Client(FSM_State_HellAntenora::Empusa_Blown_Back);
		break;
	case DamageType::Air:
		//ChangeState_Client(FSM_State_HellAntenora::Empusa_Air_Damage);
		break;
	case DamageType::Snatch:
		//ChangeState_Client(FSM_State_HellAntenora::Empusa_Snatch);
		break;
	case DamageType::Slam:
		//ChangeState_Client(FSM_State_HellAntenora::Empusa_Slam_Damage);
		break;
	case DamageType::Buster:
		//ChangeState_Client(FSM_State_HellAntenora::Empusa_Buster_Start);
		break;
	case DamageType::Stun:
		break;
	default:
		break;
	}

	AttackDelayCheck = 0.0f;
}

void Enemy_ScudoAngelo::RecognizeCollisionCheck(float _DeltaTime)
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

void Enemy_ScudoAngelo::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
	IsChangeState = true;
}

void Enemy_ScudoAngelo::ChangeState_Client(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, Player);
}

void Enemy_ScudoAngelo::EnemyCreateFSM()
{
	// 최초 등장_02
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Idle,
	.Start = [=] {
	EffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0600_Idle.fbx");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
}

void Enemy_ScudoAngelo::EnemyCreateFSM_Client()
{
	//// 최초 등장_02
	//EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Appear_02,
	//.Start = [=] {
	//EnemyRenderer->ChangeAnimation("em0000_appear_02");
	//},
	//.Update = [=](float _DeltaTime) {
	//},
	//.End = [=] {
	//}
	//	});

	//EnemyFSM.ChangeState(FSM_State_HellCaina::HellCaina_Appear_02);
}
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

	// 무기 붙이기
	EnemyRenderer->SetAttachTransform("L_Hand", LeftWeapon->GetTransform(), float4(0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 180.0f), true);
	EnemyRenderer->SetAttachTransform("R_WeaponHand", RightWeapon->GetTransform(), float4(0.0f, 0.0f, 0.0f), float4(90.0f, -180.0f, 0.0f), true);

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
			//ChangeState(FSM_State_ScudoAngelo::Empusa_Biped_Walk_Start);
		}
		else
		{
			//ChangeState(FSM_State_ScudoAngelo::Empusa_Biped_Run_Start);
		}
	}
	break;
	case EnemyRotation::Left:
	{
		AllDirectSetting();
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			//ChangeState(FSM_State_ScudoAngelo::Empusa_Biped_Walk_Start);
		}
		else
		{
			//ChangeState(FSM_State_ScudoAngelo::Empusa_Biped_Run_Start);
		}
	}
	break;
	case EnemyRotation::Left_90:
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Turn_Left_90);
		break;
	case EnemyRotation::Left_180:
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Turn_Left_180);
		break;
	case EnemyRotation::Right:
	{
		AllDirectSetting();
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			//ChangeState(FSM_State_ScudoAngelo::Empusa_Biped_Walk_Start);
		}
		else
		{
			//ChangeState(FSM_State_ScudoAngelo::Empusa_Biped_Run_Start);
		}
	}
	break;
	case EnemyRotation::Right_90:
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Turn_Right_90);
		break;
	case EnemyRotation::Right_180:
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Turn_Right_180);
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
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Continuation_Attack);
		return;
	}

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		RandomAttack();
		break;
	case EnemyRotation::Left:
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Attack_C);
		break;
	case EnemyRotation::Left_90:
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Attack_C);
		break;
	case EnemyRotation::Left_180:
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Attack_C);
		break;
	case EnemyRotation::Right:
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Attack_D);
		break;
	case EnemyRotation::Right_90:
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Attack_D);
		break;
	case EnemyRotation::Right_180:
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Attack_D);
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
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Attack_A);
	}
	else if (1 == RandC)
	{
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Attack_B);
	}
	else if (2 == RandC)
	{
		//ChangeState(FSM_State_ScudoAngelo::Empusa_Attack_W);
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
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Air_Damage);
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
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Right:
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Back);
		break;
	case DamageType::Air:
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up);
		break;
	case DamageType::Snatch:
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Snatch);
		break;
	case DamageType::Slam:
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage);
		break;
	case DamageType::Buster:
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Buster_Start);
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
			//ChangeState_Client(FSM_State_ScudoAngelo::Empusa_Air_Damage_Under);
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
			//ChangeState_Client(FSM_State_ScudoAngelo::Empusa_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			//ChangeState_Client(FSM_State_ScudoAngelo::Empusa_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			//ChangeState_Client(FSM_State_ScudoAngelo::Empusa_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			//ChangeState_Client(FSM_State_ScudoAngelo::Empusa_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		//ChangeState_Client(FSM_State_ScudoAngelo::Empusa_Blown_Back);
		break;
	case DamageType::Air:
		//ChangeState_Client(FSM_State_ScudoAngelo::Empusa_Air_Damage);
		break;
	case DamageType::Snatch:
		//ChangeState_Client(FSM_State_ScudoAngelo::Empusa_Snatch);
		break;
	case DamageType::Slam:
		//ChangeState_Client(FSM_State_ScudoAngelo::Empusa_Slam_Damage);
		break;
	case DamageType::Buster:
		//ChangeState_Client(FSM_State_ScudoAngelo::Empusa_Buster_Start);
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
	// 최초 등장
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Appear_01,
	.Start = [=] {
	EffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0600_Appear_01");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Response_Stay);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 리스폰 후 가만히 있는 자세, 검 방 앞으로 했다가 서있는 자세로 전환
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Response_Stay,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Response_Stay");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      서있음      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 서있음
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Idle");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	// 공격 자세로 서있음
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_Idle_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_Idle_Loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      피격      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////약공격
	// 정면 약공 히트
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Standing_Damage_Weak_Front", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		AttackCalculation();
		RotationCheck();
		AllDirectSetting_Normal();

		if (true == IsVergilLight)
		{
			IsVergilLight = false;
			SetPush(10000.0f);
		}
		else
		{
			SetPush(24000.0f);
		}
	}
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Death_Front);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 뒤에서 약공 히트시, 65프레임 애니메이션 턴
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Standing_Damage_Weak_Back", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		AttackCalculation();
		RotationCheck();
		AllDirectSetting_Normal();

		if (true == IsVergilLight)
		{
			IsVergilLight = false;
			SetPush(10000.0f);
		}
		else
		{
			SetPush(24000.0f);
		}
	}
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Death_Back);
		return;
	}
	if (65 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.0f, 180.0f, 0.0f });
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////강공격
	// 헤비어택 히트시
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Blown_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Blown_Back", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		IsCollapse = false;
		IsHeavyAttack = true;
		AttackCalculation();
		RotationCheck();
		AllDirectSetting();
		SetPush(50000.0f);
		SetAir(42000.0f);
	}
	FallCheckDelayTime += _DeltaTime;
	if (true == FloorCheck(FallDistance) && 0.2f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});

	// 헤비어택 랜딩
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Blown_Back_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0600_Blown_Back_Landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////에어피격
	// 띄우기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Blown_Up", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		IsCollapse = false;
		IsAirAttack = true;
		AttackCalculation();
		RotationCheck();
		AllDirectSetting();
		PhysXCapsule->SetAirState(110000.0f);
	}
	FallCheckDelayTime += _DeltaTime;

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up_Loop);
		return;
	}
	if (true == FloorCheck(FallDistance) && 0.5f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0600_Air_Damage", 1, [=] {
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Air_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Air_Damage", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up_Loop);
		return;
	}
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 에어어택 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Blown_Up_Loop", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0600_Blown_Up_Landing", 1,
			[=]
			{
				SetPush(37000.0f);
			});
	}

	// 에어 어택 끝나고 랜딩
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0600_Blown_Up_Landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////슬램

	{
		EnemyRenderer->SetAnimationStartEvent("em0600_Slam_Damage", 1, [=] { SetAir(-120000.0f); });
	}

	// 슬램 피격 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Slam_Damage");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		IsCollapse = false;
		IsSlamAttack = true;
		AttackCalculation();
	}
	if (true == EnemyRenderer->IsAnimationEnd() || true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 공중 loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Slam_Damage_Loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 후 땅에 부딫힘
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0600_Slam_Damage_Landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스내치
	// 스내치 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Snatch");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		IsCollapse = false;
		IsAirAttack = true;
		AttackCalculation();
		StartMonsterSnatch();
		RotationCheck();
		AllDirectSetting();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Air_Damage);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스턴
	// 스턴 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Stun");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Stun_Revive);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 스턴 end
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Stun_Revive");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Prone_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Prone_Getup");
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Prone_Death);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	IsCollapse = false;
	}
		});
	// 앞으로 넘어진 상태에서 Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Prone_Death,
	.Start = [=] {
	Sound.PlayVoice(9);
	EnemyRenderer->ChangeAnimation("em0600_Prone_Death");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Death_Back,
	.Start = [=] {
	Sound.PlayVoice(9);
	EnemyRenderer->ChangeAnimation("em0600_Death_Back");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Death_Front,
	.Start = [=] {
	Sound.PlayVoice(9);
	EnemyRenderer->ChangeAnimation("em0600_Death_Front");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Buster_Start,
	.Start = [=] {
			//PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
			EnemyRenderer->ChangeAnimation("em0600_Air_Buster");
			},
			.Update = [=](float _DeltaTime) {
			if (true == IsChangeState)
			{
				IsChangeState = false;
				AttackCalculation();
				IsCollapse = false;
				IsBusterAttack = true;
				BusterCalculation(float4{ 0.0f, -120.0f, 0.0f });
				RotationCheck();
				AllDirectSetting();
			}
			SetMoveStop();
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				BusterEnd();
				SetAir(-120000.0f);
				ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Buster_Loop);
				return;
			}
			},
			.End = [=] {
			}
		});
	// 버스트 히트 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Buster_Loop,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Buster_Finish);
		return;
	}
	},
	.End = [=] {
	}
		});
	// em0000_Buster_Finish, 버스트 히트 땅에 떨어짐
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Buster_Finish,
	.Start = [=] {
	MinusEnemyHP(500);
	//Sound.Play("Enemy_Damage_", 12);
	//Sound.PlayVoice(5);
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0600_Slam_Damage_Landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		//PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Prone_Getup);
		return;
	}
	},
	.End = [=] {

	}
		});


	EnemyFSM.ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Appear_01);
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
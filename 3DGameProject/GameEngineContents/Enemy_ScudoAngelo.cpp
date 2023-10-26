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
	EnemyMaxHP = 2200;
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

	LeftWeapon = CreateComponent<GameEngineFBXRenderer>();  // ����
	RightWeapon = CreateComponent<GameEngineFBXRenderer>(); // ��

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
	//Animation���� ��θ� ã�Ƽ� ���animation���� �ε�
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
		// ����Ʈ �ý��� ����
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

	//PhysX(�浹)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({ 90, 120, 90 });
	PhysXCapsule->GetDynamic()->setMass(80.0f);
	BindPhysicsWithNet(PhysXCapsule);

	// ������ ũ�� ����
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -45.0f, 0.0f });

	// �ݸ��� �ɼ�, ũ�� ����
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

	// �⺻ ����
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	// MonsterCollision->Off();
	// RN_MonsterCollision->Off();

	LeftWeapon->GetTransform()->AddLocalRotation({ 0.0f, 180.0f, 0.0f });
	LeftWeapon->GetTransform()->AddLocalRotation({ -90.0f, 0.0f, 0.0f });

	// ���� ���̱�
	EnemyRenderer->SetAttachTransform("L_Hand", LeftWeapon->GetTransform(), float4(0.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 0.0f));
	EnemyRenderer->SetAttachTransform("R_WeaponHand", RightWeapon->GetTransform(), float4(0.0f, 0.0f, 0.0f), float4(0.0f, 170.0f, 180.0f), true, true);

	float4 MeshScale_R = RightWeapon->GetMeshScale();
	MeshScale_R.x *= 0.7f;
	MeshScale_R.z *= 0.6f;

	MonsterAttackCollision->GetTransform()->SetWorldScale(MeshScale_R);
	MonsterAttackCollision->GetTransform()->SetParent(RightWeapon->GetTransform());
	MonsterAttackCollision->GetTransform()->AddLocalPosition(float4{ 0.0f, 0.0f, 100.0f });
	MonsterAttackCollision->GetTransform()->AddLocalRotation(float4{ 0.0f, 0.0f, 0.0f });

	MonsterAttackCollision->Off();
	MonsterAttackCollision_Two->Off();

	// �� ������Ʈ Ÿ�� ����1
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
///////////////////////////////////////////////// ������, ��Ʈ ���� ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_ScudoAngelo::MoveLoop()
{
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

void Enemy_ScudoAngelo::PlayerChase()
{
	RotationCheck();

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
	{
		AllDirectSetting();

		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

		if (0 == RandC)
		{
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Start);
		}
		else
		{
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Start);
		}
		return;
	}
	break;
	case EnemyRotation::Left:
	{
		AllDirectSetting();

		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

		if (0 == RandC)
		{
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Start);
		}
		else
		{
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Start);
		}
		return;
	}
	break;
	case EnemyRotation::Left_90:
		AllDirectSetting();
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_S);
		break;
	case EnemyRotation::Left_180:
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R);
		break;
	case EnemyRotation::Right:
	{
		AllDirectSetting();

		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

		if (0 == RandC)
		{
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Start);
		}
		else
		{
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Start);
		}
		return;
	}
	break;
	case EnemyRotation::Right_90:
		AllDirectSetting();
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_S);
		break;
	case EnemyRotation::Right_180:
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_L);
		break;
	default:
		break;
	}
}

void Enemy_ScudoAngelo::PlayerAttack()
{
	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);
	if (0 == RandC)
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Group_Command_Attack);
	}
	else
	{
		RotationCheck();
		AllDirectSetting();
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Kesa);
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
	if (FSM_State_ScudoAngelo::ScudoAngelo_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_ScudoAngelo::ScudoAngelo_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_ScudoAngelo::ScudoAngelo_Death_Front == EnemyFSM.GetCurState())
	{
		return;
	}

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
	AttackDelayCheck = 0.0f;

	AttackDirectCheck();

	if (EnemyHitDirect::Forward == EnemyHitDirValue)
	{
		if (FSM_State_ScudoAngelo::ScudoAngelo_Idle == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Start == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Loop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Stop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Group_Command_Attack == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Start == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Loop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_M == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_S == EnemyFSM.GetCurState())
		{
			MinusEnemyHP(50);
			StartRenderShaking(6);
			return;
		}
	}
	
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

		AttackDirectCheck();

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
}

void Enemy_ScudoAngelo::DamageCollisionCheck_Client(float _DeltaTime)
{
	if (FSM_State_ScudoAngelo::ScudoAngelo_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_ScudoAngelo::ScudoAngelo_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_ScudoAngelo::ScudoAngelo_Death_Front == EnemyFSM.GetCurState())
	{
		return;
	}

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

	AttackDirectCheck();
	AttackDelayCheck = 0.0f;

	if (EnemyHitDirect::Forward == EnemyHitDirValue)
	{
		if (FSM_State_ScudoAngelo::ScudoAngelo_Idle == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Start == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Loop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Stop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Group_Command_Attack == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Start == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Loop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_M == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_S == EnemyFSM.GetCurState())
		{
			StartRenderShaking(6);
			return;
		}
	}

	if (DamageType::VergilLight == Data.DamageTypeValue)
	{
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
			ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Air_Damage);
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
			ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Right:
			ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Back);
		break;
	case DamageType::Air:
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up);
		break;
	case DamageType::Snatch:
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Snatch);
		break;
	case DamageType::Slam:
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage);
		break;
	case DamageType::Buster:
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Buster_Start);
		break;
	case DamageType::Stun:
		break;
	default:
		break;
	}
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
	// ���� ����
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

	// ������ �� ������ �ִ� �ڼ�, �� �� ������ �ߴٰ� ���ִ� �ڼ��� ��ȯ
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
	/////////////////////////////////      ������      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ������
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Idle");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime += _DeltaTime;
	if (0.35f <= WaitTime)
	{
		if (true == IsRecognize)
		{
			IsRecognize = false;
			PlayerAttack();
		}
		else
		{
			PlayerChase();
		}
		return;
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	}
		});

	// ���� �ڼ��� ������
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
	/////////////////////////////////      �̵�      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ������ �ȱ� ����, 20������ �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Start,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0600_Walk_Front_Start");
	},
	.Update = [=](float _DeltaTime)
	{
	{
		MoveLoop();
		if (20 <= EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(170.0f);
		}
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Loop);
		return;
	}
	},
	.End = [=]
	{
	}
		});

	// ������ �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Walk_Front_Loop");
	},
	.Update = [=](float _DeltaTime)
	{
	RunTime += _DeltaTime;
	{
		MoveLoop();
		SetForwardMove(220.0f);
	}

	if (true == IsRecognize)
	{
		IsRecognize = false;
		PlayerAttack();
		return;
	}
	else if (RunTime >= 3.0f)
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Stop);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});

	// ������ �ȱ� ��, 30������ �ȱ� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Walk_Front_Stop");
	},
	.Update = [=](float _DeltaTime)
	{
	{
		AllDirectSetting_Normal();
		if (30 >= EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(170.0f);
		}
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

	// ���� 180�� ����(�̸� �߸� ����)
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Turn_Attack_To_Guard_L_Dummy_Turn_R");
	},
	.Update = [=](float _DeltaTime)
	{

	if (10 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// ������ 180�� ����(�̸� �߸� ����)
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_L,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Turn_Attack_To_Guard_L_Dummy_Turn_L");
	},
	.Update = [=](float _DeltaTime)
	{

	if (10 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// �ڷ� ������ S (90�� ȸ����)
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_S,
	.Start = [=] {
	SetThrowback(33000.0f);
	EnemyRenderer->ChangeAnimation("em0600_Step_Back_S");
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

	// �ڷ� ������ M (���� �� ȸ�ǿ�)
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_M,
	.Start = [=] {
	RotationCheck();
	AllDirectSetting();
	SetThrowback(43000.0f);
	EnemyRenderer->ChangeAnimation("em0600_Step_Back_M");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      ����      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ���� ���� ���
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Group_Command_Attack,
	.Start = [=]
	{
	RotationCheck();
	SlerpCalculation();
	SlerpTime = 0.0f;
	EnemyRenderer->ChangeAnimation("em0600_Group_Command_Attack");
	},
	.Update = [=](float _DeltaTime)
	{
	{
		SlerpTurn(_DeltaTime * 2.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);
		if (0 == RandC)
		{
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Group_Command_Attack_End);
		}
		else
		{
			ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Hikkake);
		}
		return;
	}
	},
	.End = [=]
	{
	SlerpTime = 0.0f;
	}
		});

	// �� ���� �� �ڷ�, 13������ ����, 45������ ���� �� // 52������ on, 57������ off // 80 ������ ����, 130 ������ ���� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Hikkake,
	.Start = [=] {
	RotationCheck();
	SlerpCalculation();
	SlerpTime = 0.0f;
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Hikkake");
	},
	.Update = [=](float _DeltaTime)
	{
	{
		AllDirectSetting_Normal();
		if (13 <= EnemyRenderer->GetCurFrame() && 45 >= EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(140.0f);
		}
		if (80 <= EnemyRenderer->GetCurFrame() && 130 >= EnemyRenderer->GetCurFrame())
		{
			SetBackMove(140.0f);
		}
		if (52 > EnemyRenderer->GetCurFrame())
		{
			SlerpTurn(_DeltaTime * 2.0f);
		}
	}
	if (52 <= EnemyRenderer->GetCurFrame() && 57 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_M);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollisionOff();
	SlerpTime = 0.0f;
	}
		});

	// ������ ����, 43������ on, 48������ off
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Kesa,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Kesa");
	},
	.Update = [=](float _DeltaTime)
	{
	if (43 <= EnemyRenderer->GetCurFrame() && 48 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollisionOff();
	IsRecognize = false;
	}
		});

	// ���� ���� ������ ���(�ȶٰ� �ٷ� ���) // 10������ ������ �̵� ����, 55������ ������ �̵� �� // 50������ on, 55������ off // 120 ������ ���� ����, 220 ������ ���� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Group_Command_Attack_End,
	.Start = [=] {
	RotationCheck();
	SlerpCalculation();
	SlerpTime = 0.0f;
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0600_Group_Command_Attack_End");
	},
	.Update = [=](float _DeltaTime)
	{
	{
		AllDirectSetting_Normal();
		if (10 <= EnemyRenderer->GetCurFrame() && 55 >= EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(140.0f);
		}
		if (120 <= EnemyRenderer->GetCurFrame() && 220 >= EnemyRenderer->GetCurFrame())
		{
			SetBackMove(140.0f);
		}
		if (50 > EnemyRenderer->GetCurFrame())
		{
			SlerpTurn(_DeltaTime * 2.0f);
		}
	}
	if (50 <= EnemyRenderer->GetCurFrame() && 55 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_M);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollisionOff();
	SlerpTime = 0.0f;
	}
		});

	// �޸��� ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Run_Start");
	},
	.Update = [=](float _DeltaTime)
	{
	RunTime += _DeltaTime;
	{
		MoveLoop();
		SetForwardMove(400.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Loop);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});

	// �޸��� ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Run_Loop");
	},
	.Update = [=](float _DeltaTime)
	{
	RunTime += _DeltaTime;
	{
		MoveLoop();
		SetForwardMove(400.0f);
	}

	if (true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Attack);
		return;
	}
	else if (RunTime >= 1.5f)
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Stop_A);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});

	// ���� ����, 42������ on, 47������ off // 100������ ���� �� // 110������ ���� ����, 180 ������ ���� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Attack,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Run_Attack");
	},
	.Update = [=](float _DeltaTime)
	{
	RunTime += _DeltaTime;
	{
		AllDirectSetting_Normal();
		if (90 >= EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(300.0f);
		}
		else if (110 <= EnemyRenderer->GetCurFrame() && 180 >= EnemyRenderer->GetCurFrame())
		{
			SetBackMove(200.0f);
		}
		else
		{
			SetMoveStop();
		}
	}
	if (42 <= EnemyRenderer->GetCurFrame() && 47 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});

	// �޸��� ���� ���� // 50������ �ٱ� ��, �ȱ� ����, 100������ �ȱ� �� // 120������ �ȱ� ����, 195������ �ȱ� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Stop_A,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Run_Stop_A");
	},
	.Update = [=](float _DeltaTime)
	{
	RunTime += _DeltaTime;
	{
		AllDirectSetting_Normal();
		if (50 > EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(220.0f);
		}
		else if (50 <= EnemyRenderer->GetCurFrame() && 100 > EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(140.0f);
		}
		else if (120 <= EnemyRenderer->GetCurFrame() && 185 > EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(110.0f);
		}
		else
		{
			SetMoveStop();
		}
	}
	if (42 <= EnemyRenderer->GetCurFrame() && 47 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      �ǰ�      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////�����
	// ���� ��� ��Ʈ
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

	// �ڿ��� ��� ��Ʈ��, 65������ �ִϸ��̼� ��
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

	/////////////////////////������
	// ������ ��Ʈ��
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

	// ������ ����
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

	/////////////////////////�����ǰ�
	// ���� ����
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

	// ���� ���¿��� ����� ������
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

	// ������� ����
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

	// ���� ���� ������ ����
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

	/////////////////////////����

	{
		EnemyRenderer->SetAnimationStartEvent("em0600_Slam_Damage", 1, [=] { SetAir(-120000.0f); });
	}

	// ���� �ǰ� start
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
	// ���� �ǰ� ���� loop
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
	// ���� �ǰ� �� ���� �΋H��
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

	/////////////////////////����ġ
	// ����ġ start
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

	/////////////////////////����
	// ���� start
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
	// ���� end
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
	/////////////////////////////////      ������      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// ������ �������� �� �Ͼ�� ���
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
	// ������ �Ѿ��� ���¿��� Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Prone_Death,
	.Start = [=] {
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
	// ������ �������鼭 ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Death_Back,
	.Start = [=] {
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
	// �ڷ� �������鼭 ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Death_Front,
	.Start = [=] {
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
	/////////////////////////////////      ����Ʈ      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// em0000_Buster_Start, ����Ʈ ��Ʈ ����
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
	// ����Ʈ ��Ʈ ����
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
	// em0000_Buster_Finish, ����Ʈ ��Ʈ ���� ������
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
	//// ���� ����_02
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
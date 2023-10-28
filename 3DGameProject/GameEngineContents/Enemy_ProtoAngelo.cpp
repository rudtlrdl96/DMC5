#include "PreCompileHeader.h"
#include "Enemy_ProtoAngelo.h"

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
Enemy_ProtoAngelo::Enemy_ProtoAngelo() 
{
}

Enemy_ProtoAngelo::~Enemy_ProtoAngelo() 
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////   Actor Init   ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_ProtoAngelo::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::ProtoAngelo;
	EnemyMaxHP = 200000;
	EnemyHP = EnemyMaxHP;
}

void Enemy_ProtoAngelo::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("em0601.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "Proto", "mesh"
			},
			"em0601.FBX");

		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("em0601.fbx", "AniFBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("em0601.fbx", "AniFBX");
	}
	break;
	default:
		break;
	}

	RightWeapon = CreateComponent<GameEngineFBXRenderer>();

	if (nullptr == GameEngineFBXMesh::Find("wpem0601_00.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "Proto", "mesh"
			},
			"wpem0601_00.FBX");

		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		RightWeapon->SetFBXMesh("wpem0601_00.fbx", "FBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		RightWeapon->SetFBXMesh("wpem0601_00.fbx", "FBX");
	}
	break;
	default:
		break;
	}

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
	RightWeapon->SetTexture("PaperBurnTexture", "PaperBurnNoise.jpg");
}

void Enemy_ProtoAngelo::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("Proto");
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

	// 사운드 로드
	Sound.SetVoiceName("Proto_V_");
	Sound.SFXVolume = 0.5f;
	Sound.VoiceVolume = 0.35f;
	if (nullptr == GameEngineSound::Find("Proto_V_0.wav")) {
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("Voice");
		NewDir.Move("Proto");
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
		}

		NewDir.MoveParent();
		NewDir.MoveParent();
		NewDir.Move("SFX");
		NewDir.Move("Proto");
		Files = NewDir.GetAllFile({ ".wav" });

		if (nullptr == GameEngineSound::Find("Proto_SFX_0.wav")) {
			for (size_t i = 0; i < Files.size(); i++)
			{
				GameEngineSound::Load(Files[i].GetFullPath());
			}
		}
		if (nullptr == GameEngineSound::Find("Cavaliere_Damage_0.wav"))
		{
			NewDir.MoveParent();
			NewDir.Move("Cavaliere");
			Files = NewDir.GetAllFile({ ".wav" });
			for (size_t i = 0; i < Files.size(); i++)
			{
				GameEngineSound::Load(Files[i].GetFullPath());
			}
		}
	}

}

void Enemy_ProtoAngelo::Start()
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
	//EnemyRenderer->GetTransform()->AddLocalRotation({ 90.0f, 0.0f, 0.0f });

	// 콜리전 옵션, 크기 설정
	MonsterAttackCollision->SetAttackData(DamageType::Light, 100);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);

	MonsterCollision->GetTransform()->SetLocalScale({ 80, 210, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 65, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 700, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	ParryCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Null);
	ParryCollision->SetColType(ColType::OBBBOX3D);
	ParryCollision->GetTransform()->SetLocalScale({ 200, 200, 220 });
	ParryCollision->GetTransform()->SetLocalPosition({ 0, 100, 170 });

	// 기본 세팅
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	//MonsterCollision->Off();
	//RN_MonsterCollision->Off();

	// 무기 붙이기
	EnemyRenderer->SetAttachTransform("R_WeaponHand", RightWeapon->GetTransform(), float4(0.0f, 0.0f, 0.0f), float4(180.0f, 0.0f, 0.0f), true, true);

	float4 MeshScale_R = RightWeapon->GetMeshScale();
	MeshScale_R.x *= 0.7f;
	MeshScale_R.z *= 0.65f;

	MonsterAttackCollision->GetTransform()->SetWorldScale(MeshScale_R);
	MonsterAttackCollision->GetTransform()->SetParent(RightWeapon->GetTransform());
	MonsterAttackCollision->GetTransform()->AddLocalPosition(float4{ 0.0f, 0.0f, 140.0f });
	MonsterAttackCollision->GetTransform()->AddLocalRotation(float4{ 0.0f, 0.0f, 0.0f });
	MonsterAttackCollision->Off();

	// 넷 오브젝트 타입 설정1
	SetNetObjectType(Net_ActorType::ProtoAngelo);

	LinkData_UpdatePacket<bool>(AnimationTurnStart);
	LinkData_UpdatePacket<bool>(IsHeavyAttack);
	LinkData_UpdatePacket<bool>(IsAirAttack);
	LinkData_UpdatePacket<bool>(IsSlamAttack);
	LinkData_UpdatePacket<bool>(IsBusterAttack);
	LinkData_UpdatePacket<bool>(IsVergilLight);
	LinkData_UpdatePacket<bool>(IsCollapse);
	LinkData_UpdatePacket<bool>(IsBurn);
	LinkData_UpdatePacket<bool>(IsParryCheck);
	LinkData_UpdatePacket<bool>(ParryOkay);
	LinkData_UpdatePacket<int>(EnemyHP);

	BindNetObjEvent(2, [this](std::vector<NetworkObjectBase*> _Actors)
		{
			if (_Actors.size() <= 0)
			{
				MsgAssert("잘못된 DamageCallBack 이벤트입니다");
				return;
			}
			BasePlayerActor* _Player = dynamic_cast<BasePlayerActor*>(_Actors[0]);
			if (nullptr == _Player)
			{
				MsgAssert("잘못된 DamageCallBack 이벤트입니다");
				return;
			}
			// Player = _Player;

			DamageData Datas = _Player->GetAttackCollision()->GetDamage();
			MinusEnemyHP(Datas.DamageValue);
			PlayDamageEvent(Datas.SoundType, true);

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
		});

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

		});
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// 움직임, 히트 관련 ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_ProtoAngelo::ParryCheck()
{
	if (false == IsParryCheck || true == ParryOkay || true == DeathValue)
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Col = ParryCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	if (false == AttackCol->GetIsParryAttack()) { return; }
	AttackCol->ParryEvent();
	ParryOkay = true;
}

void Enemy_ProtoAngelo::ParryCheck_Client()
{
	if (false == IsParryCheck || true == ParryOkay || true == DeathValue)
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Col = ParryCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	if (false == AttackCol->GetIsParryAttack()) { return; }
	AttackCol->ParryEvent();
	ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Lose_Modori);
}

void Enemy_ProtoAngelo::ParryTime()
{
	SetTimeScale(0.2f);
	GetLevel()->TimeEvent.AddEvent(0.4f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			SetTimeScale(0.4f);
		});
	GetLevel()->TimeEvent.AddEvent(0.6f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			SetTimeScale(0.6f);
		});
	GetLevel()->TimeEvent.AddEvent(0.8f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			SetTimeScale(0.8f);
		});
	GetLevel()->TimeEvent.AddEvent(1.f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			SetTimeScale(1.f);
		});
}

void Enemy_ProtoAngelo::PlayDamageEvent(DamageSoundType _Type, bool _PlayVoice)
{
	if (true == _PlayVoice)
	{
		Sound.PlayVoiceRandom(14, 16);
	}
	EffectRenderer->PlayFX("Enemy_Damage_Heavy.effect");

	switch (_Type)
	{
	case DamageSoundType::None:
		break;
	case DamageSoundType::Sword:
		Sound.PlayRandom("Cavaliere_Damage_", 0, 1);
		break;
	case DamageSoundType::Magic:
		Sound.Play("Cavaliere_Damage_", 5);
		break;
	case DamageSoundType::Katana:
		Sound.PlayRandom("Cavaliere_Damage_", 2, 3);
		break;
	case DamageSoundType::Blunt:
		Sound.Play("Cavaliere_Damage_", 4);
		break;
	case DamageSoundType::Gun:
		Sound.Play("Cavaliere_Damage_", 2);
		break;
	default:
		break;
	}
}

void Enemy_ProtoAngelo::MoveLoop()
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

void Enemy_ProtoAngelo::MonsterAttackCollisionOn()
{
	MonsterAttackCollision->On();
	MonsterAttackCollision_Two->On();
}

void Enemy_ProtoAngelo::MonsterAttackCollisionOff()
{
	MonsterAttackCollision->Off();
	MonsterAttackCollision_Two->Off();
}

void Enemy_ProtoAngelo::PlayerChase()
{
	RotationCheck();

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
	{
		AllDirectSetting();

		//int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

		//if (0 == RandC)
		//{
		//	ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_T_Run_Start);
		//}
		//else
		//{
		//	ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		//}
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		return;
	}
	break;
	case EnemyRotation::Left:
	{
		AllDirectSetting();

		//int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

		//if (0 == RandC)
		//{
		//	ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_T_Run_Start);
		//}
		//else
		//{
		//	ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		//}
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		return;
	}
	break;
	case EnemyRotation::Left_90:
		AllDirectSetting();
		//ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Step_Back_S);
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		break;
	case EnemyRotation::Left_180:
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R);
		break;
	case EnemyRotation::Right:
	{
		AllDirectSetting();

		//int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

		//if (0 == RandC)
		//{
		//	ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_T_Run_Start);
		//}
		//else
		//{
		//	ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		//}
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		return;
	}
	break;
	case EnemyRotation::Right_90:
		AllDirectSetting();
		//ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Step_Back_S);
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		break;
	case EnemyRotation::Right_180:
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_L);
		break;
	default:
		break;
	}
}

void Enemy_ProtoAngelo::PlayerAttack()
{
	RotationCheck();
	AllDirectSetting();

	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

	if (0 == RandC)
	{
		//ChangeState(FSM_State_ProtoAngelo::Empusa_Continuation_Attack);
		return;
	}

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		RandomAttack();
		break;
	case EnemyRotation::Left:
		//ChangeState(FSM_State_ProtoAngelo::Empusa_Attack_C);
		break;
	case EnemyRotation::Left_90:
		//ChangeState(FSM_State_ProtoAngelo::Empusa_Attack_C);
		break;
	case EnemyRotation::Left_180:
		//ChangeState(FSM_State_ProtoAngelo::Empusa_Attack_C);
		break;
	case EnemyRotation::Right:
		//ChangeState(FSM_State_ProtoAngelo::Empusa_Attack_D);
		break;
	case EnemyRotation::Right_90:
		//ChangeState(FSM_State_ProtoAngelo::Empusa_Attack_D);
		break;
	case EnemyRotation::Right_180:
		//ChangeState(FSM_State_ProtoAngelo::Empusa_Attack_D);
		break;
	default:
		break;
	}
}

void Enemy_ProtoAngelo::RandomAttack()
{
	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);

	if (0 == RandC)
	{
		//ChangeState(FSM_State_ProtoAngelo::Empusa_Attack_A);
	}
	else if (1 == RandC)
	{
		//ChangeState(FSM_State_ProtoAngelo::Empusa_Attack_B);
	}
	else if (2 == RandC)
	{
		//ChangeState(FSM_State_ProtoAngelo::Empusa_Attack_W);
	}
}

void Enemy_ProtoAngelo::AttackCalculation()
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

void Enemy_ProtoAngelo::DamageCollisionCheck(float _DeltaTime)
{
	if (FSM_State_ProtoAngelo::ProtoAngelo_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_ProtoAngelo::ProtoAngelo_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_ProtoAngelo::ProtoAngelo_Death_Front == EnemyFSM.GetCurState())
	{
		return;
	}

	ParryCheck();

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
	PlayDamageEvent(Data.SoundType, true);
	AttackDelayCheck = 0.0f;

	//if (EnemyHitDirect::Forward == EnemyHitDirValue)
	//{
	//	if (FSM_State_ProtoAngelo::ProtoAngelo_Idle == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Loop == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Stop == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Group_Command_Attack == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Step_Back_M == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Step_Back_S == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Attack_T_Run_Start == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Attack_T_Run_Loop == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Attack_T_Run_Stop_A == EnemyFSM.GetCurState()
	//		|| true == IsSuperArmor)
	//	{
	//		if (Data.DamageTypeValue == DamageType::Buster)
	//		{
	//			MinusEnemyHP(Data.DamageValue);
	//			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Buster_Start);
	//		}
	//		else if (Data.DamageTypeValue == DamageType::Stun)
	//		{
	//			MinusEnemyHP(Data.DamageValue);
	//			StopTime(2.9f);
	//		}
	//		else
	//		{
	//			MinusEnemyHP(70);
	//			StartRenderShaking(6);
	//		}
	//		HitStop(Data.DamageTypeValue);
	//		return;
	//	}
	//}

	if (true == ParryEvent)
	{
		MinusEnemyHP(Data.DamageValue);
		StartRenderShaking(6);
		HitStop(Data.DamageTypeValue);
		return;
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
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Air_Damage);
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
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Right:
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Back);
		break;
	case DamageType::Air:
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up);
		break;
	case DamageType::Snatch:
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Snatch);
		break;
	case DamageType::Slam:
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage);
		break;
	case DamageType::Buster:
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Buster_Start);
		break;
	case DamageType::Stun:
		StopTime(2.9f);
		return;
	default:
		break;
	}
}

void Enemy_ProtoAngelo::DamageCollisionCheck_Client(float _DeltaTime)
{
	if (FSM_State_ProtoAngelo::ProtoAngelo_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_ProtoAngelo::ProtoAngelo_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_ProtoAngelo::ProtoAngelo_Death_Front == EnemyFSM.GetCurState())
	{
		return;
	}

	ParryCheck_Client();

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
	PlayDamageEvent(Data.SoundType, true);
	AttackDelayCheck = 0.0f;

	//if (EnemyHitDirect::Forward == EnemyHitDirValue)
	//{
	//	if (FSM_State_ProtoAngelo::ProtoAngelo_Idle == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Loop == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Stop == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Group_Command_Attack == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Step_Back_M == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Step_Back_S == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Attack_T_Run_Start == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Attack_T_Run_Loop == EnemyFSM.GetCurState()
	//		|| FSM_State_ProtoAngelo::ProtoAngelo_Attack_T_Run_Stop_A == EnemyFSM.GetCurState()
	//		|| true == IsSuperArmor)
	//	{
	//		if (Data.DamageTypeValue == DamageType::Buster)
	//		{
	//			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Buster_Start);
	//		}
	//		else
	//		{
	//			ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Player });
	//			StartRenderShaking(6);
	//		}
	//		return;
	//	}
	//}

	if (true == ParryEvent)
	{
		StartRenderShaking(6);
		return;
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
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Air_Damage);
			return;
		}

		if (true == IsCollapse)
		{
			StartRenderShaking(8);
			ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Player });
			return;
		}

		AttackDirectCheck();

		switch (EnemyHitDirValue)
		{
		case EnemyHitDirect::Forward:
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Right:
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Back);
		break;
	case DamageType::Air:
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up);
		break;
	case DamageType::Snatch:
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Snatch);
		break;
	case DamageType::Slam:
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage);
		break;
	case DamageType::Buster:
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Buster_Start);
		break;
	case DamageType::Stun:
		break;
	default:
		break;
	}
}

void Enemy_ProtoAngelo::RecognizeCollisionCheck(float _DeltaTime)
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

void Enemy_ProtoAngelo::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
	IsChangeState = true;
}

void Enemy_ProtoAngelo::ChangeState_Client(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, Player);
}

void Enemy_ProtoAngelo::EnemyCreateFSM()
{
	// 최초 등장_02
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Appear_01,
	.Start = [=] {
	EffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0601_Appear_01");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Idle,
	.Start = [=] {
	IsRecognize = false;
	EnemyRenderer->ChangeAnimation("em0601_Idle_Loop");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime += _DeltaTime;
	if (0.35f <= WaitTime)
	{
		//if (true == IsRecognize)
		//{
		//	IsRecognize = false;
		//	PlayerAttack();
		//}
		//else
		//{
		//	PlayerChase();
		//}
		//return;
		PlayerChase();
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      이동      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 앞으로 걷기 시작, 18프레임 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start,
	.Start = [=]
	{
	RotationCheck();
	AllDirectSetting_Normal();
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0601_A_Walk_Start_L0");
	},
	.Update = [=](float _DeltaTime)
	{
	{
		{
			SlerpTurn(_DeltaTime * 2.5f);
			AllDirectSetting_Normal();
			if (18 <= EnemyRenderer->GetCurFrame())
			{
				SetForwardMove(170.0f);
			}
		}
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Loop);
		return;
	}
	},
	.End = [=]
	{
	SlerpTime = 0.0f;
	}
		});

	// 앞으로 걷기 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_A_Walk_Loop");
	},
	.Update = [=](float _DeltaTime)
	{
	RunTime += _DeltaTime;
	{
		MoveLoop();
		SetForwardMove(220.0f);
	}

/*	if (true == IsRecognize)
	{
		IsRecognize = false;
		PlayerAttack();
		return;
	}
	else */if (RunTime >= 3.0f)
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Stop);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});

	// 앞으로 걷기 끝, 20프레임 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_A_Walk_End_L0");
	},
	.Update = [=](float _DeltaTime)
	{
	{
		AllDirectSetting_Normal();
		if (20 >= EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(170.0f);
		}
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 왼쪽 돌기(이름 잘못 저장), 20프레임 애니메이션 턴 // 10프레임 전진 시작, 64프레임 전진 종료
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Turn_Attack_To_Quard_R");
	},
	.Update = [=](float _DeltaTime)
	{
	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}

	if (10 > EnemyRenderer->GetCurFrame() && 64 <= EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(140.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 오른쪽 돌기(이름 잘못 저장), 23프레임 애니메이션 턴
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_L,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Turn_Attack_To_Quard_L");
	},
	.Update = [=](float _DeltaTime)
	{

	if (23 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      피격      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 패리 성공 모션
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Parry_Lose_Modori,
	.Start = [=] {
	ParryTime();
	//EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	//EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em0601_Parry_Lose_Modori");
	},
	.Update = [=](float _DeltaTime) {
	if (25 < EnemyRenderer->GetCurFrame())
	{
		ParryEvent = false;
	}
	if (30 <= EnemyRenderer->GetCurFrame() && 70 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(140.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	ParryEvent = false;
	}
		});

	/////////////////////////약공격
	// 정면 약공 히트
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Standing_Damage_Weak_Front", true);
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
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Death_Front);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 뒤에서 약공 히트시, 65프레임 애니메이션 턴
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Standing_Damage_Weak_Back", true);
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
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Death_Back);
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
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////강공격
	// 헤비어택 히트시
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Blown_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Blown_Back", true);
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
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});

	// 헤비어택 랜딩
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Blown_Back_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0601_Blown_Back_Landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////에어피격
	// 띄우기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Blown_Up", true);
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
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up_Loop);
		return;
	}
	if (true == FloorCheck(FallDistance) && 0.5f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0601_Air_Damage", 1, [=] {
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Air_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Air_Damage", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up_Loop);
		return;
	}
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 에어어택 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Blown_Up_Loop", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0601_Blown_Up_Landing", 1,
			[=]
			{
				SetPush(37000.0f);
			});
	}

	// 에어 어택 끝나고 랜딩
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0601_Blown_Up_Landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////슬램

	{
		EnemyRenderer->SetAnimationStartEvent("em0601_Slam_Damage", 1, [=] { SetAir(-120000.0f); });
	}

	// 슬램 피격 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Slam_Damage");
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
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 공중 loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Slam_Damage_Loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 후 땅에 부딫힘
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0601_Slam_Damage_Landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Prone_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스내치
	// 스내치 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Snatch");
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
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Air_Damage);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스턴
	// 스턴 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Stun");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Stun_Revive);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 스턴 end
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Stun_Revive");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
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
	// 앞으로 쓰러져있다가 일어남, 120프레임 뒷걸음 시작, 170프레임 종료
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Prone_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Prone_Getup");
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Prone_Death);
		return;
	}
	if (120 <= EnemyRenderer->GetCurFrame() && 170 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(140.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	IsCollapse = false;
	}
		});
	// 앞으로 넘어진 상태에서 Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Prone_Death,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Prone_Death");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Death_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Death_Back");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Death_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Death_Front");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Buster_Start,
	.Start = [=] {
			//PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
			EnemyRenderer->ChangeAnimation("em0601_Air_Buster");
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
				ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Buster_Loop);
				return;
			}
			},
			.End = [=] {
			}
		});
	// 버스트 히트 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Buster_Loop,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Buster_Finish);
		return;
	}
	},
	.End = [=] {
	}
		});
	// em0000_Buster_Finish, 버스트 히트 땅에 떨어짐
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Buster_Finish,
	.Start = [=] {
	MinusEnemyHP(500);
	//Sound.Play("Enemy_Damage_", 12);
	//Sound.PlayVoice(5);
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0601_Slam_Damage_Landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		//PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Prone_Getup);
		return;
	}
	},
	.End = [=] {

	}
		});

	EnemyFSM.ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Appear_01);
}

void Enemy_ProtoAngelo::EnemyCreateFSM_Client()
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
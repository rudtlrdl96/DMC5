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
#include "Player_MirageBlade.h"

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
	
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		EnemyMaxHP = 2200;
	}
	else
	{
		EnemyMaxHP = 4400;
	}

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
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1),
				std::bind(&SoundController::Play, &Sound, "Scudo_SFX_", std::placeholders::_1),
				std::bind(&SoundController::PlayVoice, &Sound, std::placeholders::_1, false),
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

		MonsterEffectRenderer = CreateComponent<FXSystem>();
		for (size_t i = 0; i < FXFiles.size(); i++)
		{
			if (nullptr == FXData::Find(FXFiles[i].GetFileName()))
			{
				FXData::Load(FXFiles[i].GetFullPath());
			}
			MonsterEffectRenderer->CreateFX(FXData::Find(FXFiles[i].GetFileName()));
		}
	}

	// 사운드 로드
	Sound.SetVoiceName("Scudo_V_");
	if (nullptr == GameEngineSound::Find("Scudo_V_0.wav")) {
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("Voice");
		NewDir.Move("Scudo");
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
		}

		NewDir.MoveParent();
		NewDir.MoveParent();
		NewDir.Move("SFX");
		NewDir.Move("Scudo");
		Files = NewDir.GetAllFile({ ".wav" });

		if (nullptr == GameEngineSound::Find("Scudo_SFX_0.wav")) {
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

	if (nullptr == GameEngineSound::Find("Enemy_Damage_0.wav"))
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("SFX");
		NewDir.Move("Enemy");
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });
		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
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

	if (true == NetworkManager::IsClient())
	{
		PhysXCapsule->TurnOffGravity();
	}

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
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 650, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	ParryCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Null);
	ParryCollision->SetColType(ColType::OBBBOX3D);
	ParryCollision->GetTransform()->SetLocalScale({ 200, 200, 220 });
	ParryCollision->GetTransform()->SetLocalPosition({ 0, 100, 170 });

	// 기본 세팅
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	// MonsterCollision->Off();
	// RN_MonsterCollision->Off();

	LeftWeapon->GetTransform()->AddLocalRotation({ 0.0f, 180.0f, 0.0f });
	LeftWeapon->GetTransform()->AddLocalRotation({ -90.0f, 0.0f, 0.0f });

	// 무기 붙이기
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
	LinkData_UpdatePacket<bool>(IsParryCheck);
	LinkData_UpdatePacket<bool>(ParryOkay);
	LinkData_UpdatePacket<bool>(IsSuperArmor);
	LinkData_UpdatePacket<int>(EnemyHP);

	BindNetObjEvent(2, [this](std::vector<NetworkObjectBase*> _Actors)
		{
			if (_Actors.size() <= 0)
			{
				MsgAssert("잘못된 DamageCallBack 이벤트입니다");
				return;
			}
			BasePlayerActor* _Player = dynamic_cast<BasePlayerActor*>(_Actors[0]);
			DamageData Datas;
			if (nullptr == _Player)
			{
				Player_MirageBlade* _Mirage = dynamic_cast<Player_MirageBlade*>(_Actors[0]);
				if (nullptr == _Mirage) { return; }
				Datas = _Mirage->Collision->GetDamage();
			}
			else
			{
				Datas = _Player->GetAttackCollision()->GetDamage();
			}

			PlayDamageEvent(Datas.SoundType);

			HitStop(Datas.DamageTypeValue);

			if (true == IsCollapse)
			{
				MinusEnemyHP(Datas.DamageValue);
			}
			else
			{
				MinusEnemyHP(70);
			}

			if (DamageType::Stun == Datas.DamageTypeValue)
			{
				StopTime(2.8f);
				AttackDelayCheck = 1.0f;
			}

			if (EnemyHP < 0)
			{
				DeathValue = true;
			}
		});

	SetDamagedNetCallBack<NetworkObjectBase>([this](NetworkObjectBase* _Attacker) {
		BasePlayerActor* _Player = dynamic_cast<BasePlayerActor*>(_Attacker);
		DamageData Datas;
		if (nullptr == _Player)
		{
			Player_MirageBlade* _Mirage = dynamic_cast<Player_MirageBlade*>(_Attacker);
			if (nullptr == _Mirage) { return; }
			Datas = _Mirage->Collision->GetDamage();
		}
		else
		{
			Datas = _Player->GetAttackCollision()->GetDamage();
			Player = _Player;
		}

		MinusEnemyHP(Datas.DamageValue);
		PlayDamageEvent(Datas.SoundType);
		HitStop(Datas.DamageTypeValue);

		if (DamageType::VergilLight == Datas.DamageTypeValue)
		{
			IsVergilLight = true;
		}

		if (DamageType::Stun == Datas.DamageTypeValue)
		{
			StopTime(2.9f);
			AttackDelayCheck = 1.0f;
		}

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

void Enemy_ScudoAngelo::ParryCheck()
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

	AttackDirectCheck();

	if (EnemyHitDirect::Forward == EnemyHitDirValue)
	{
		AttackCol->ParryEvent();
		ParryOkay = true;
	}
}

void Enemy_ScudoAngelo::ParryCheck_Client()
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

	AttackDirectCheck();

	NetworkObjectBase* Obj = dynamic_cast<NetworkObjectBase*>(AttackCol->GetActor());

	if (EnemyHitDirect::Forward == EnemyHitDirValue)
	{
		AttackCol->ParryEvent();
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Parry_Lose_Modori, Obj);
	}
}

void Enemy_ScudoAngelo::ParryTime()
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

void Enemy_ScudoAngelo::PlayDamageEvent(DamageSoundType _Type)
{
	Sound.PlayVoiceRandom(1, 2);
	MonsterEffectRenderer->PlayFX("Enemy_Damage_Heavy.effect");

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
	PlayDamageEvent(Data.SoundType);
	AttackDelayCheck = 0.0f;

	AttackDirectCheck();

	if (EnemyHitDirect::Forward == EnemyHitDirValue)
	{
		if (FSM_State_ScudoAngelo::ScudoAngelo_Idle == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Start == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Loop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Stop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Group_Command_Attack == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_M == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_S == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Start == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Loop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Stop_A == EnemyFSM.GetCurState()
			|| true == IsSuperArmor)
		{
			if (Data.DamageTypeValue == DamageType::Buster)
			{
				MinusEnemyHP(Data.DamageValue);
				ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Buster_Start);
			}
			else if (Data.DamageTypeValue == DamageType::Stun)
			{
				MinusEnemyHP(Data.DamageValue);
				StopTime(2.9f);
			}
			else
			{
				MinusEnemyHP(70);
				StartRenderShaking(6);
			}
			HitStop(Data.DamageTypeValue);
			return;
		}
	}

	if (true == ParryEvent)
	{
		MinusEnemyHP(Data.DamageValue);
		StartRenderShaking(6);
		HitStop(Data.DamageTypeValue);
		return;
	}

	MinusEnemyHP(Data.DamageValue);

	if (DamageType::VergilLight == Data.DamageTypeValue)
	{
		IsVergilLight = true;
		Data.DamageTypeValue = DamageType::Light;
	}

	HitStop(Data.DamageTypeValue);

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
		AttackDelayCheck = 1.0f;
		StopTime(2.9f);
		return;
	default:
		break;
	}
}

void Enemy_ScudoAngelo::DamageCollisionCheck_Client(float _DeltaTime)
{
	if (FSM_State_ScudoAngelo::ScudoAngelo_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_ScudoAngelo::ScudoAngelo_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_ScudoAngelo::ScudoAngelo_Death_Front == EnemyFSM.GetCurState())
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
	PlayDamageEvent(Data.SoundType);

	AttackDirectCheck();
	AttackDelayCheck = 0.0f;

	NetworkObjectBase* Obj = dynamic_cast<NetworkObjectBase*>(AttackCol->GetActor());

	if (EnemyHitDirect::Forward == EnemyHitDirValue)
	{
		if (FSM_State_ScudoAngelo::ScudoAngelo_Idle == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Start == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Loop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Stop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Group_Command_Attack == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_M == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_S == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Start == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Loop == EnemyFSM.GetCurState()
			|| FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Stop_A == EnemyFSM.GetCurState()
			|| true == IsSuperArmor)
		{
			if (Data.DamageTypeValue == DamageType::Buster)
			{
				ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Buster_Start, Obj);
			}
			else
			{
				ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
				StartRenderShaking(6);
			}
			return;
		}
	}

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
			ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Air_Damage, Obj);
			return;
		}

		if (true == IsCollapse)
		{
			StartRenderShaking(8);
			ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
			return;
		}

		AttackDirectCheck();

		switch (EnemyHitDirValue)
		{
		case EnemyHitDirect::Forward:
			ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front, Obj);
			break;
		case EnemyHitDirect::Back:
			ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Back, Obj);
			break;
		case EnemyHitDirect::Left:
			ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front, Obj);
			break;
		case EnemyHitDirect::Right:
			ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front, Obj);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Back, Obj);
		break;
	case DamageType::Air:
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up, Obj);
		break;
	case DamageType::Snatch:
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Snatch, Obj);
		break;
	case DamageType::Slam:
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage, Obj);
		break;
	case DamageType::Buster:
		ChangeState_Client(FSM_State_ScudoAngelo::ScudoAngelo_Buster_Start, Obj);
		break;
	case DamageType::Stun:
		AttackDelayCheck = 1.0f;
		ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
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

void Enemy_ScudoAngelo::ChangeState_Client(int _StateValue, NetworkObjectBase* _Obj)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, _Obj);
}

void Enemy_ScudoAngelo::EnemyCreateFSM()
{
	// 최초 등장
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Appear_01,
	.Start = [=] {
	MonsterEffectRenderer->PlayFX("Enemy_Appear.effect");
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
	IsRecognize = false;
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
	/////////////////////////////////      이동      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 앞으로 걷기 시작, 20프레임 걷기 시작
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

	// 앞으로 걷기 루프
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

	// 앞으로 걷기 끝, 30프레임 걷기 끝
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

	// 왼쪽 180도 돌기(이름 잘못 저장)
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

	// 오른쪽 180도 돌기(이름 잘못 저장)
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

	// 뒤로 빽스텝 S (90도 회전용)
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

	// 뒤로 빽스텝 M (공격 후 회피용)
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_M,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Step_Back_M");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		RotationCheck();
		AllDirectSetting();
		SetThrowback(43000.0f);
	}
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
	/////////////////////////////////      공격      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 공격 시작 모션
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

	// 앞 공격 후 뒤로, 13프레임 전진, 45프레임 전진 끝 // 52프레임 on, 57프레임 off // 80 프레임 후퇴, 130 프레임 후퇴 끝
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
	if (false == ParryOkay)
	{
		if (42 <= EnemyRenderer->GetCurFrame() && 57 > EnemyRenderer->GetCurFrame())
		{
			IsParryCheck = true;
		}
		else
		{
			IsParryCheck = false;
		}
	}
	if (57 > EnemyRenderer->GetCurFrame())
	{
		AttackDirectCheck();
		if (EnemyHitDirect::Forward == EnemyHitDirValue)
		{
			IsSuperArmor = true;
		}
	}
	else
	{
		IsSuperArmor = false;
	}
	if (true == ParryOkay)
	{
		SetMoveStop();
		MonsterAttackCollisionOff();
		ParryOkay = false;
		IsParryCheck = false;
		ParryEvent = true;

		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Parry_Lose_Modori);
		return;
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
	IsSuperArmor = false;
	IsParryCheck = false;
	}
		});

	// 앞으로 공격, 43프레임 on, 48프레임 off
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
	if (48 > EnemyRenderer->GetCurFrame())
	{
		AttackDirectCheck();
		if (EnemyHitDirect::Forward == EnemyHitDirValue)
		{
			IsSuperArmor = true;
		}
	}
	else
	{
		IsSuperArmor = false;
	}
	if (false == ParryOkay)
	{
		if (33 <= EnemyRenderer->GetCurFrame() && 48 > EnemyRenderer->GetCurFrame())
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
		SetMoveStop();
		MonsterAttackCollisionOff();
		ParryOkay = false;
		IsParryCheck = false;
		ParryEvent = true;

		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Parry_Lose_Modori);
		return;
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
	IsSuperArmor = false;
	IsParryCheck = false;
	}
		});

	// 공격 시작 앞으로 찌르기(안뛰고 바로 찌르기) // 10프레임 앞으로 이동 시작, 55프레임 앞으로 이동 끝 // 50프레임 on, 55프레임 off // 120 프레임 후퇴 시작, 220 프레임 후퇴 끝
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
	if (55 > EnemyRenderer->GetCurFrame())
	{
		AttackDirectCheck();
		if (EnemyHitDirect::Forward == EnemyHitDirValue)
		{
			IsSuperArmor = true;
		}
	}
	else
	{
		IsSuperArmor = false;
	}
	if (false == ParryOkay)
	{
		if (40 <= EnemyRenderer->GetCurFrame() && 55 > EnemyRenderer->GetCurFrame())
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
		SetMoveStop();
		MonsterAttackCollisionOff();
		ParryOkay = false;
		IsParryCheck = false;
		ParryEvent = true;

		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Parry_Lose_Modori);
		return;
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
	IsSuperArmor = false;
	IsParryCheck = false;
	}
		});

	// 달리기 어택 시작
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

	// 달리기 어택 루프
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

	// 돌진 공격, 42프레임 on, 47프레임 off // 100프레임 전진 끝 // 110프레임 후퇴 시작, 180 프레임 후퇴 끝
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
	if (false == ParryOkay)
	{
		if (32 <= EnemyRenderer->GetCurFrame() && 47 > EnemyRenderer->GetCurFrame())
		{
			IsParryCheck = true;
		}
		else
		{
			IsParryCheck = false;
		}
	}
	if (47 > EnemyRenderer->GetCurFrame())
	{
		AttackDirectCheck();
		if (EnemyHitDirect::Forward == EnemyHitDirValue)
		{
			IsSuperArmor = true;
		}
	}
	else
	{
		IsSuperArmor = false;
	}
	if (true == ParryOkay)
	{
		SetMoveStop();
		MonsterAttackCollisionOff();
		ParryOkay = false;
		IsParryCheck = false;
		ParryEvent = true;

		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Parry_Lose_Modori);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollisionOff();
	RunTime = 0.0f;
	IsSuperArmor = false;
	IsParryCheck = false;
	}
		});

	// 달리기 어택 종료 // 50프레임 뛰기 끝, 걷기 시작, 100프레임 걷기 끝 // 120프레임 걷기 시작, 195프레임 걷기 끝
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
	/////////////////////////////////      피격      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 패리 성공 모션
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Parry_Lose_Modori,
	.Start = [=] {
	ParryTime();
	//EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	//EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em0600_Parry_Lose_Modori");
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
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	ParryEvent = false;
	}
		});

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
	// 앞으로 쓰러져있다가 일어남, 120프레임 뒷걸음 시작, 170프레임 종료
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
	if (120 <= EnemyRenderer->GetCurFrame() && 170 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(140.0f);
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
	Sound.PlayVoiceRandom(10, 11);
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
	Sound.PlayVoiceRandom(10, 11);
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
	Sound.PlayVoiceRandom(10, 11);
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
		SetAir(-120000.0f);
		ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Buster_Loop);
		return;
	}
	},
	.End = [=] {
	BusterEnd();
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
	Sound.Play("Enemy_Damage_", 12);
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
	// 최초 등장
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Appear_01,
	.Start = [=] {
	MonsterEffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0600_Appear_01");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Response_Stay,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Response_Stay");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Idle");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_Idle_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_Idle_Loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Start,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0600_Walk_Front_Start");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Walk_Front_Loop");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Walk_Front_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Walk_Front_Stop");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Turn_Attack_To_Guard_L_Dummy_Turn_R");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_L,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Turn_Attack_To_Guard_L_Dummy_Turn_L");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_S,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Step_Back_S");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Step_Back_M,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Step_Back_M");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Group_Command_Attack,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0600_Group_Command_Attack");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Hikkake,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Hikkake");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Kesa,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Kesa");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Group_Command_Attack_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Group_Command_Attack_End");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Run_Start");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Run_Loop");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Attack,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Run_Attack");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Attack_T_Run_Stop_A,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Attack_T_Run_Stop_A");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Parry_Lose_Modori,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Parry_Lose_Modori");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Standing_Damage_Weak_Front", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Standing_Damage_Weak_Back", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Blown_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Blown_Back", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Blown_Back_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Blown_Back_Landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Blown_Up", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Air_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Air_Damage", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Blown_Up_Loop", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Blown_Up_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Blown_Up_Landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Slam_Damage");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Slam_Damage_Loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Slam_Damage_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Slam_Damage_Landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Snatch");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Stun");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Prone_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Prone_Getup");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Prone_Death,
	.Start = [=] {
	DeathSetting_Client();
	Sound.PlayVoiceRandom(10, 11);
	EnemyRenderer->ChangeAnimation("em0600_Prone_Death");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Death_Back,
	.Start = [=] {
	DeathSetting_Client();
	Sound.PlayVoiceRandom(10, 11);
	EnemyRenderer->ChangeAnimation("em0600_Death_Back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Death_Front,
	.Start = [=] {
	DeathSetting_Client();
	Sound.PlayVoiceRandom(10, 11);
	EnemyRenderer->ChangeAnimation("em0600_Death_Front");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Buster_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0600_Air_Buster");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Buster_Loop,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_ScudoAngelo::ScudoAngelo_Buster_Finish,
	.Start = [=] {
	Sound.Play("Enemy_Damage_", 12);
	EnemyRenderer->ChangeAnimation("em0600_Slam_Damage_Landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {

	}
		});

	EnemyFSM.ChangeState(FSM_State_ScudoAngelo::ScudoAngelo_Appear_01);
}
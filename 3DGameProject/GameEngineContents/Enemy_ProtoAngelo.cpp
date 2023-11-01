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
#include "Player_MirageBlade.h"

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

	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		EnemyMaxHP = 4000;
	}
	else
	{
		EnemyMaxHP = 8000;
	}

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
	//Animation���� ��θ� ã�Ƽ� ���animation���� �ε�
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
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1),
				std::bind(&SoundController::Play, &Sound, "Proto_SFX_", std::placeholders::_1),
				std::bind(&SoundController::PlayVoice, &Sound, std::placeholders::_1, false),
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

	// ���� �ε�
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
	}

	if (nullptr == GameEngineSound::Find("Proto_SFX_0.wav")) {
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("SFX");
		NewDir.Move("Proto");
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
		}
	}
	if (nullptr == GameEngineSound::Find("Cavaliere_Damage_0.wav"))
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("SFX");
		NewDir.Move("Cavaliere");
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });
		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
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

void Enemy_ProtoAngelo::Start()
{
	BaseEnemyActor::Start();

	//PhysX(�浹)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({ 90, 120, 90 });
	PhysXCapsule->GetDynamic()->setMass(80.0f);

	if (true == NetworkManager::IsClient())
	{
		PhysXCapsule->TurnOffGravity();
	}

	BindPhysicsWithNet(PhysXCapsule);

	// ������ ũ�� ����
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -45.0f, 0.0f });

	// �ݸ��� �ɼ�, ũ�� ����
	MonsterAttackCollision->SetAttackData(DamageType::Light, 100);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);

	MonsterCollision->GetTransform()->SetLocalScale({ 80, 210, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 65, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 750, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	ParryCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Null);
	ParryCollision->SetColType(ColType::OBBBOX3D);
	ParryCollision->GetTransform()->SetLocalScale({ 200, 200, 220 });
	ParryCollision->GetTransform()->SetLocalPosition({ 0, 100, 170 });

	// �⺻ ����
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	// ���� ���̱�
	EnemyRenderer->SetAttachTransform("R_WeaponHand", RightWeapon->GetTransform(), float4(0.0f, 0.0f, 0.0f), float4(180.0f, 0.0f, 0.0f), true, true);

	float4 MeshScale_R = RightWeapon->GetMeshScale();
	MeshScale_R.x *= 0.7f;
	MeshScale_R.z *= 0.65f;

	MonsterAttackCollision->GetTransform()->SetWorldScale(MeshScale_R);
	MonsterAttackCollision->GetTransform()->SetParent(RightWeapon->GetTransform());
	MonsterAttackCollision->GetTransform()->AddLocalPosition(float4{ 0.0f, 0.0f, 140.0f });
	MonsterAttackCollision->GetTransform()->AddLocalRotation(float4{ 0.0f, 0.0f, 0.0f });
	MonsterAttackCollision->Off();

	// �� ������Ʈ Ÿ�� ����1
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
	LinkData_UpdatePacket<bool>(IsSuperArmor);
	LinkData_UpdatePacket<bool>(DeathValue);
	LinkData_UpdatePacket<int>(SuperArmorStack);
	LinkData_UpdatePacket<int>(EnemyHP);
	LinkData_UpdatePacket<int>(ServerPlayerID);

	BindNetObjEvent(2, [this](std::vector<NetworkObjectBase*> _Actors)
		{
			if (_Actors.size() <= 0)
			{
				MsgAssert("�߸��� DamageCallBack �̺�Ʈ�Դϴ�");
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

			PlayDamageEvent(Datas.SoundType, true);

			if (true == IsSuperArmor)
			{
				++SuperArmorStack;
			}

			if (2 <= SuperArmorStack)
			{
				IsSuperArmor = false;
			}

			MinusEnemyHP(Datas.DamageValue);
			HitStop(Datas.DamageTypeValue);

			if (DamageType::Stun == Datas.DamageTypeValue)
			{
				StopTime(2.9f);
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

		});

	BindNetObjEvent(3, [this](std::vector<NetworkObjectBase*> _Actors)
		{
			BusterEnd_Client();
		});
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// ������, ��Ʈ ���� ///////////////////////////////////////////////////////
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

	AttackDirectCheck();

	if (EnemyHitDirect::Forward == EnemyHitDirValue)
	{
		AttackCol->ParryEvent();
		ParryOkay = true;
	}
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

	AttackDirectCheck();

	NetworkObjectBase* Obj = dynamic_cast<NetworkObjectBase*>(AttackCol->GetActor());

	if (EnemyHitDirect::Forward == EnemyHitDirValue)
	{
		AttackCol->ParryEvent();
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Lose_Modori, Obj);
	}
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

void Enemy_ProtoAngelo::PlayerChase()
{
	RotationCheck();

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
	{
		AllDirectSetting();
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		return;
	}
	break;
	case EnemyRotation::Left:
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);
		if (0 == RandC)
		{
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Step_Back);
		}
		else
		{
			AllDirectSetting();
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		}
		return;
	}
	break;
	case EnemyRotation::Left_90:
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);
		if (0 == RandC)
		{
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Step_Back);
		}
		else
		{
			AllDirectSetting();
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		}
		return;
	}
	break;
	case EnemyRotation::Left_180:
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R);
		break;
	case EnemyRotation::Right:
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);
		if (0 == RandC)
		{
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Step_Back);
		}
		else
		{
			AllDirectSetting();
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		}
		return;
	}
	break;
	case EnemyRotation::Right_90:
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);
		if (0 == RandC)
		{
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Step_Back);
		}
		else
		{
			AllDirectSetting();
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start);
		}
		return;
	}
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

	if (EnemyRotation::Left_180 == EnemyRotationValue)
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_01_Turn_L);
	}
	else if (EnemyRotation::Right_180 == EnemyRotationValue)
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_01_Turn_R);
	}
	else
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_Kesa);
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
	MinusEnemyHP(Data.DamageValue);
	AttackDelayCheck = 0.0f;

	if (2 <= SuperArmorStack)
	{
		IsSuperArmor = false;
	}

	if (true == IsSuperArmor)
	{
		DeathCheck();

		if (true == DeathValue)
		{
			AttackCalculation();

			if (EnemyHitDirect::Forward == EnemyHitDirValue
				|| EnemyHitDirect::Left == EnemyHitDirValue
				|| EnemyHitDirect::Right == EnemyHitDirValue)
			{
				ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Death_Back);
			}
			else if (EnemyHitDirect::Back == EnemyHitDirValue)
			{
				ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Death_Back);
			}
			return;
		}

		if (DamageType::Buster == Data.DamageTypeValue)
		{
			HitStop(Data.DamageTypeValue);
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Buster_Start);
			return;
		}

		if (DamageType::Stun == Data.DamageTypeValue)
		{
			++SuperArmorStack;
			HitStop(Data.DamageTypeValue);
			StopTime(2.9f);
			StartRenderShaking(8);
			return;
		}

		if (DamageType::Heavy == Data.DamageTypeValue
			|| DamageType::Air == Data.DamageTypeValue
			|| DamageType::Slam == Data.DamageTypeValue)
		{
			++SuperArmorStack;
			HitStop(Data.DamageTypeValue);
			StartRenderShaking(8);
			return;
		}

		HitStop(Data.DamageTypeValue);
		StartRenderShaking(8);
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
		AttackDelayCheck = 1.0f;
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

	NetworkObjectBase* Obj = dynamic_cast<NetworkObjectBase*>(AttackCol->GetActor());

	if (true == IsSuperArmor)
	{
		if (EnemyHP < 0)
		{
			DeathValue = true;
		}

		if (true == DeathValue)
		{
			AttackCalculation();

			if (EnemyHitDirect::Forward == EnemyHitDirValue
				|| EnemyHitDirect::Left == EnemyHitDirValue
				|| EnemyHitDirect::Right == EnemyHitDirValue)
			{
				ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Death_Front, Obj);
			}
			else if (EnemyHitDirect::Back == EnemyHitDirValue)
			{
				ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Death_Back, Obj);
			}
			return;
		}

		if (DamageType::Buster == Data.DamageTypeValue)
		{
			BusterClientStart = true;
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Buster_Start, Obj);
			return;
		}

		if (DamageType::Stun == Data.DamageTypeValue)
		{
			StopTime(2.9f);
			StartRenderShaking(8);
			ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
			return;
		}

		if (DamageType::Heavy == Data.DamageTypeValue
			|| DamageType::Air == Data.DamageTypeValue
			|| DamageType::Slam == Data.DamageTypeValue)
		{
			StartRenderShaking(8);
			ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
			return;
		}

		StartRenderShaking(8);
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
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Air_Damage, Obj);
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
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front, Obj);
			break;
		case EnemyHitDirect::Back:
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Back, Obj);
			break;
		case EnemyHitDirect::Left:
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front, Obj);
			break;
		case EnemyHitDirect::Right:
			ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front, Obj);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Back, Obj);
		break;
	case DamageType::Air:
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up, Obj);
		break;
	case DamageType::Snatch:
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Snatch, Obj);
		break;
	case DamageType::Slam:
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage, Obj);
		break;
	case DamageType::Buster:
		BusterClientStart = true;
		ChangeState_Client(FSM_State_ProtoAngelo::ProtoAngelo_Buster_Start, Obj);
		break;
	case DamageType::Stun:
		AttackDelayCheck = 1.0f;
		ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
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

void Enemy_ProtoAngelo::ChangeState_Client(int _StateValue, NetworkObjectBase* _Obj)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, _Obj);
}

void Enemy_ProtoAngelo::EnemyCreateFSM()
{
	// ���� ����_02
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Appear_01,
	.Start = [=] {
	MonsterEffectRenderer->PlayFX("Enemy_Appear.effect");
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
		PhysXCapsule->On();
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Idle,
	.Start = [=] {
	IsRecognize = false;
	EnemyRenderer->ChangeAnimation("em0601_Idle_Loop");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime += _DeltaTime;
	if (0.25f <= WaitTime)
	{
		int HalfHP = 0;
		if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
		{
			HalfHP = 2000;
		}
		else
		{
			HalfHP = 3500;
		}

		if (EnemyHP >= HalfHP)
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
		}
		else
		{
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Attack_Start);
		}
		return;
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      �̵�      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// ������ �ȱ� ����, 18������ �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start,
	.Start = [=]
	{
	IsRecognize = false;
	EnemyRenderer->ChangeAnimation("em0601_A_Walk_Start_L0");
	},
	.Update = [=](float _DeltaTime)
	{
	if (true == IsChangeState)
	{
		RotationCheck();
		AllDirectSetting_Normal();
		SlerpCalculation();
	}
	{
		{
			SlerpTurn(_DeltaTime * 5.5f);
			AllDirectSetting_Normal();
			if (18 <= EnemyRenderer->GetCurFrame())
			{
				SetForwardMove(180.0f);
			}
		}
	}
	if (true == IsRecognize)
	{
		IsRecognize = false;
		PlayerAttack();
		return;
	}
	else if (true == EnemyRenderer->IsAnimationEnd())
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

	// ������ �ȱ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Loop,
	.Start = [=] {
	IsRecognize = false;
	EnemyRenderer->ChangeAnimation("em0601_A_Walk_Loop");
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
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Stop);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});

	// ������ �ȱ� ��, 20������ �ȱ� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Stop,
	.Start = [=] {
	IsRecognize = false;
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

	// ���� ����(�̸� �߸� ����), 20������ �ִϸ��̼� �� // 10������ ���� ����, 64������ ���� ����
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

	// ������ ����(�̸� �߸� ����), 23������ �ִϸ��̼� ��
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

	// �ڷ� ������
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Step_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Step_Back");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		RotationCheck();
		AllDirectSetting();
	}
	if (75 >= EnemyRenderer->GetCurFrame())
	{
		SetBackMove(240.0f);
	}
	else if (100 <= EnemyRenderer->GetCurFrame() && 195 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(120.0f);
	}
	else if (240 <= EnemyRenderer->GetCurFrame() && 250 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(100.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
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

	// �������� 180�� ���鼭 �������(04) ���� ���� ��, 10������ �ִϸ��̼� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_01_Turn_L,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_01_Turn_L");
	},
	.Update = [=](float _DeltaTime) {
	if (10 < EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_04);
		return;
	}
	},
	.End = [=] {
	}
		});

	// ���������� 180�� ���鼭 �ߵ�(03) ���� ���� ��, 40������ �ִϸ��̼� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_01_Turn_R,
	.Start = [=] {
	RightAttackEvent = true;
	EnemyRenderer->ChangeAnimation("em0601_Attack_01_Turn_R");
	},
	.Update = [=](float _DeltaTime) {
	if (40 < EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_03);
		return;
	}
	},
	.End = [=] {
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0601_Attack_Kesa", 90, [=] {
			int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);
			IsRecognize = false;
			if (0 != RandC)
			{
				if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
				{
					std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
					bool Iscontact = PlayerContactCheck_Normal(Collision.get());

					if (true == Iscontact)
					{
						IsRecognize = true;
					}

					if (true == IsRecognize)
					{
						SetMoveStop();
						ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_03);
					}
					return;
				}
				else
				{
					IsRecognize = false;
				}
			}
			});
	}

	// ������ ª�� ���� // 30������ ����, 54������ ���� �� // 60������ on, 65������ off // 169 ������ ���� ����, 203������ ���� ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Kesa,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0601_Attack_Kesa");
	},
	.Update = [=](float _DeltaTime) {
	if (30 <= EnemyRenderer->GetCurFrame() && 54 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(140.0f);
	}
	if (60 <= EnemyRenderer->GetCurFrame() && 65 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (169 <= EnemyRenderer->GetCurFrame() && 203 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(140.0f);
	}
	if (false == ParryOkay)
	{
		if (50 <= EnemyRenderer->GetCurFrame() && 65 > EnemyRenderer->GetCurFrame())
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
		MonsterAttackCollision->Off();
		ParryOkay = false;
		IsParryCheck = false;
		IsSuperArmor = false;

		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Lose_Modori);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		WaitTime = 0.2f;
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0601_Attack_03", 38, [=] {
			int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);
			IsRecognize = false;
			if (0 != RandC)
			{
				if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
				{
					std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
					bool Iscontact = PlayerContactCheck_Normal(Collision.get());

					if (true == Iscontact)
					{
						IsRecognize = true;
					}

					if (true == IsRecognize)
					{
						SetMoveStop();
						ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_04);
					}
					return;
				}
				else
				{
					IsRecognize = false;
				}
			}
			});
	}

	// �ߵ�, 38������ ���� �� �������(04)�� ��ȯ ���� // 28������ on 33������ off // 115������ ����, 165������(End) ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_03,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0601_Attack_03");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		RotationCheck();
		AllDirectSetting();
		SlerpCalculation();
	}
	if (28 > EnemyRenderer->GetCurFrame())
	{
		AllDirectSetting_Normal();
		SlerpTurn(_DeltaTime * 5.5f);
	}
	if (28 <= EnemyRenderer->GetCurFrame() && 33 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (115 <= EnemyRenderer->GetCurFrame() && 165 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(140.0f);
	}
	if (false == ParryOkay)
	{
		if (18 <= EnemyRenderer->GetCurFrame() && 33 > EnemyRenderer->GetCurFrame())
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
		MonsterAttackCollision->Off();
		ParryOkay = false;
		IsParryCheck = false;
		IsSuperArmor = false;

		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Lose_Modori);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		WaitTime = 0.2f;
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	SlerpTime = 0.0f;
	}
		});

	// �ߵ� �� �������, 53������ ���� �� ������� ����(04)���� ��ȯ ���� // 88������ on 93������ off // 165������ ����, 227������(End) ��
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_04,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0601_Attack_04");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		RotationCheck();
		AllDirectSetting();
		SlerpCalculation();
	}
	if (88 > EnemyRenderer->GetCurFrame())
	{
		AllDirectSetting_Normal();
		SlerpTurn(_DeltaTime * 5.5f);
	}
	if (88 <= EnemyRenderer->GetCurFrame() && 93 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (165 <= EnemyRenderer->GetCurFrame() && 227 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(140.0f);
	}
	if (false == ParryOkay)
	{
		if (78 <= EnemyRenderer->GetCurFrame() && 93 > EnemyRenderer->GetCurFrame())
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
		MonsterAttackCollision->Off();
		ParryOkay = false;
		IsParryCheck = false;
		IsSuperArmor = false;

		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Lose_Modori);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		WaitTime = 0.2f;
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	SlerpTime = 0.0f;
	}
		});

	// ���ִٰ� ��� �ڼ��� ���� // 15������ ���� ����, 45������ ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Parry_Attack_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Parry_Attack_Start");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		RotationCheck();
		AllDirectSetting();
	}
	if (15 <= EnemyRenderer->GetCurFrame() && 45 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(210.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Attack_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});

	// ��� �޸��� ���� �ڼ� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Parry_Attack_Loop,
	.Start = [=] {
	IsRecognize = false;
	EnemyRenderer->ChangeAnimation("em0601_Parry_Attack_Loop");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime += _DeltaTime;
	if (true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Yokonagi);
		return;
	}
	if (6.0f <= WaitTime)
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Attack_End);
		return;
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	}
		});

	// ���� �� �߰� �� ȸ������(�ߵ�, 03) // 45������ ���� ���� // 63������ On 68������ Off
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Yokonagi,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_Yokonagi");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		RotationCheck();
		AllDirectSetting();
		SlerpCalculation();
	}
	if (46 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(300.0f);
	}
	if (63 > EnemyRenderer->GetCurFrame())
	{
		AllDirectSetting_Normal();
		SlerpTurn(_DeltaTime * 5.5f);
	}
	if (63 < EnemyRenderer->GetCurFrame() && 68 >= EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (false == ParryOkay)
	{
		if (53 <= EnemyRenderer->GetCurFrame() && 68 > EnemyRenderer->GetCurFrame())
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
		MonsterAttackCollision->Off();
		ParryOkay = false;
		IsParryCheck = false;
		IsSuperArmor = false;

		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Lose_Modori);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Step_Back);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	MonsterAttackCollision->Off();
	}
		});

	// ��� �и��ڼ����� ���ִ� �ڼ��� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Parry_Attack_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Parry_Attack_End");
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

	// �ڼ���� ���� ����, Tossin�� �������� ���� ������� // 60������ ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_Start");
	},
	.Update = [=](float _DeltaTime) {
	IsRecognize = false;
	if (true == IsChangeState)
	{
		RotationCheck();
		AllDirectSetting_Normal();
		SlerpCalculation();
	}
	{
		{
			SlerpTurn(_DeltaTime * 5.5f);
			AllDirectSetting_Normal();
			if (60 <= EnemyRenderer->GetCurFrame())
			{
				SetForwardMove(500.0f);
			}
		}
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Loop);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// ��� ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Loop,
	.Start = [=] {
	IsRecognize = false;
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_Loop");
	},
	.Update = [=](float _DeltaTime) {
	RunTime += _DeltaTime;
	SetForwardMove(500.0f);
	if (true == IsRecognize)
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Attack);
		return;
	}
	if (0.9f <= RunTime)
	{
		float DotProductResult = MonsterAndPlayerDotProductDegree();

		if (-90.0f <= DotProductResult && 90.0f >= DotProductResult)
		{
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_End);
		}
		else
		{
			ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_End_Turn);
		}
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});

	// �߽߰� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Attack,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_Yokonagi");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		RotationCheck();
		AllDirectSetting();
		SlerpCalculation();
	}
	if (46 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(300.0f);
	}
	if (63 > EnemyRenderer->GetCurFrame())
	{
		AllDirectSetting_Normal();
		SlerpTurn(_DeltaTime * 5.5f);
	}
	if (63 < EnemyRenderer->GetCurFrame() && 68 >= EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (false == ParryOkay)
	{
		if (53 <= EnemyRenderer->GetCurFrame() && 68 > EnemyRenderer->GetCurFrame())
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
		MonsterAttackCollision->Off();
		ParryOkay = false;
		IsParryCheck = false;
		IsSuperArmor = false;

		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Lose_Modori);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Parry_Attack_End);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	MonsterAttackCollision->Off();
	}
		});

	// ���� �� // 45������ ���� ���� // 85������ ���� ����, 151������ ���� ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_End");
	},
	.Update = [=](float _DeltaTime) {
	if (45 >= EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(500.0f);
	}
	if (85 < EnemyRenderer->GetCurFrame() && 151 >= EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(140.0f);
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

	// ���� ���� �� �ڷ� �� // 140������ ���� ���� // 75������ �ִϸ��̼� �� 
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_End_Turn,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_End_Turn");
	},
	.Update = [=](float _DeltaTime) {
	if (140 >= EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(500.0f);
	}
	if (75 < EnemyRenderer->GetCurFrame())
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
	/////////////////////////////////      �ǰ�      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// �и� ���� ���
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Parry_Lose_Modori,
	.Start = [=] {
	ParryTime();
	IsParryCheck = false;
	IsSuperArmor = false;
	MonsterEffectRenderer->PlayFX("Damage_Parry.effect");
	EnemyRenderer->ChangeAnimation("em0601_Parry_Lose_Modori");
	},
	.Update = [=](float _DeltaTime) {
			//if (25 < EnemyRenderer->GetCurFrame())
			//{
			//	ParryEvent = false;
			//}
			if (30 <= EnemyRenderer->GetCurFrame() && 70 > EnemyRenderer->GetCurFrame())
			{
				SetBackMove(140.0f);
			}
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				IsSuperArmor = true;
				SuperArmorStack = 0;
				ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
				return;
			}
			},
			.End = [=] {
			ParryEvent = false;
			}
		});

	/////////////////////////�����
	// ���� ��� ��Ʈ
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
		IsSuperArmor = true;
		SuperArmorStack = 0;
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// �ڿ��� ��� ��Ʈ��, 65������ �ִϸ��̼� ��
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
		IsSuperArmor = true;
		SuperArmorStack = 0;
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////������
	// ������ ��Ʈ��
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

	// ������ ����
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

	/////////////////////////�����ǰ�
	// ���� ����
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

	// ���� ���¿��� ����� ������
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

	// ������� ����
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

	// ���� ���� ������ ����
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

	/////////////////////////����

	{
		EnemyRenderer->SetAnimationStartEvent("em0601_Slam_Damage", 1, [=] { SetAir(-120000.0f); });
	}

	// ���� �ǰ� start
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
	// ���� �ǰ� ���� loop
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
	// ���� �ǰ� �� ���� �΋H��
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

	/////////////////////////����ġ
	// ����ġ start
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

	/////////////////////////����
	// ���� start
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
	// ���� end
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
	/////////////////////////////////      ������      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// ������ �������ִٰ� �Ͼ, 120������ �ް��� ����, 170������ ����
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
		SetBackMove(120.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsSuperArmor = true;
		SuperArmorStack = 0;
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	IsCollapse = false;
	}
		});
	// ������ �Ѿ��� ���¿��� Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Prone_Death,
	.Start = [=] {
	Sound.PlayVoice(17);
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
	// ������ �������鼭 ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Death_Back,
	.Start = [=] {
	Sound.PlayVoice(17);
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
	// �ڷ� �������鼭 ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Death_Front,
	.Start = [=] {
	Sound.PlayVoice(17);
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
	/////////////////////////////////      ����Ʈ      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// em0000_Buster_Start, ����Ʈ ��Ʈ ����
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Buster_Start,
	.Start = [=] {
	IsSuperArmor = false;
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
		SetAir(-120000.0f);
		ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Buster_Loop);
		return;
	}
	},
	.End = [=] {
	BusterEnd();
	if (true == NetworkManager::IsServer())
	{
		ExcuteNetObjEvent(3, NetObjEventPath::ActiveToPassive, { Player });
	}
	}
		});
	// ����Ʈ ��Ʈ ����
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
	// em0000_Buster_Finish, ����Ʈ ��Ʈ ���� ������
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Buster_Finish,
	.Start = [=] {
	MinusEnemyHP(500);
	Sound.Play("Enemy_Damage_", 12);
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

	EnemyFSM.ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Appear_01);
}

void Enemy_ProtoAngelo::EnemyCreateFSM_Client()
{
	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Appear_01,
	.Start = [=] {
	MonsterEffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0601_Appear_01");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Idle_Loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Start,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0601_A_Walk_Start_L0");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_A_Walk_Loop");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Walk_Front_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_A_Walk_End_L0");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_R,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Turn_Attack_To_Quard_R");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Turn_Attack_To_Guard_L_Dummy_Turn_L,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Turn_Attack_To_Quard_L");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Step_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Step_Back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_01_Turn_L,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_01_Turn_L");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_01_Turn_R,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_01_Turn_R");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Kesa,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_Kesa");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_03");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_04,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_04");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Parry_Attack_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Parry_Attack_Start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Parry_Attack_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Parry_Attack_Loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Yokonagi,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_Yokonagi");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Parry_Attack_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Parry_Attack_End");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_Start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_Loop");
	},
	.Update = [=](float _DeltaTime) {
	RunTime += _DeltaTime;
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_Attack,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_Yokonagi");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_End");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Attack_Tossin_Parry_End_Turn,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Attack_Tossin_Parry_End_Turn");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Parry_Lose_Modori,
	.Start = [=] {
	MonsterEffectRenderer->PlayFX("Damage_Parry.effect");
	EnemyRenderer->ChangeAnimation("em0601_Parry_Lose_Modori");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Standing_Damage_Weak_Front", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Standing_Damage_Weak_Back", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Blown_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Blown_Back", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Blown_Back_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Blown_Back_Landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Blown_Up", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Air_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Air_Damage", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Blown_Up_Loop", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Blown_Up_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Blown_Up_Landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Slam_Damage");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Slam_Damage_Loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Slam_Damage_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Slam_Damage_Landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Snatch");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Stun");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Stun_Revive");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Prone_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0601_Prone_Getup");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Prone_Death,
	.Start = [=] {
	Sound.PlayVoice(17);
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

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Death_Back,
	.Start = [=] {
	Sound.PlayVoice(17);
	EnemyRenderer->ChangeAnimation("em0601_Death_Back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Death_Front,
	.Start = [=] {
	Sound.PlayVoice(17);
	EnemyRenderer->ChangeAnimation("em0601_Death_Front");
	},
	.Update = [=](float _DeltaTime) {

	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Buster_Start,
	.Start = [=] {
	BusterCalculation_Client(float4{ 0.f, -45.f, 0.f });
	EnemyRenderer->ChangeAnimation("em0601_Air_Buster");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Buster_Loop,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_ProtoAngelo::ProtoAngelo_Buster_Finish,
	.Start = [=] {
	Sound.Play("Enemy_Damage_", 12);
	EnemyRenderer->ChangeAnimation("em0601_Slam_Damage_Landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {

	}
		});

	EnemyFSM.ChangeState(FSM_State_ProtoAngelo::ProtoAngelo_Appear_01);
}
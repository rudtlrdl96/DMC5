#include "PrecompileHeader.h"
#include "Enemy_Empusa.h"

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
Enemy_Empusa::Enemy_Empusa() 
{
}

Enemy_Empusa::~Enemy_Empusa() 
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////   Actor Init   ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_Empusa::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::Empusa;
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		EnemyMaxHP = 1200;
	}
	else
	{
		EnemyMaxHP = 2400;
	}

	EnemyHP = EnemyMaxHP;
}

void Enemy_Empusa::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Character", "Enemy", "Empusa", "mesh"
			},
			"em0100.FBX"
		);
		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("em0100.fbx", "AniFBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("em0100.fbx", "AniFBX");
	}
	break;
	default:
		break;
	}

	if (nullptr == GameEngineTexture::Find("PaperBurnNoise.jpg"))
	{
		GameEngineTexture::Load(GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Texture", "BurnNoiseTexture"
			}, "PaperBurnNoise.jpg"));
	}

	EnemyRenderer->SetTexture("PaperBurnTexture", "PaperBurnNoise.jpg");
}

void Enemy_Empusa::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("Empusa");
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
				std::bind(&SoundController::Play, &Sound, "Empusa_SFX_",std::placeholders::_1),
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
	Sound.SetVoiceName("Empusa_V_");
	if (nullptr == GameEngineSound::Find("Empusa_V_0.wav")) {
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("Voice");
		NewDir.Move("Empusa");
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
		}

		NewDir.MoveParent();
		NewDir.MoveParent();
		NewDir.Move("SFX");
		NewDir.Move("Empusa");
		Files = NewDir.GetAllFile({ ".wav" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
		}

		if (nullptr == GameEngineSound::Find("Enemy_Damage_0.wav"))
		{
			NewDir.MoveParent();
			NewDir.Move("Enemy");
			Files = NewDir.GetAllFile({ ".wav" });
			for (size_t i = 0; i < Files.size(); i++)
			{
				GameEngineSound::Load(Files[i].GetFullPath());
			}
		}
	}
}

void Enemy_Empusa::Start()
{
	BaseEnemyActor::Start();

	//PhysX(충돌)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({ 90, 110, 90 });
	PhysXCapsule->GetDynamic()->setMass(80.0f);

	if (true == NetworkManager::IsClient())
	{
		PhysXCapsule->TurnOffGravity();
	}
	
	BindPhysicsWithNet(PhysXCapsule);

	// 랜더러 크기 설정
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -50.0f, 0.0f });

	// 콜리전 옵션, 크기 설정
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);
	MonsterCollision->GetTransform()->SetLocalScale({ 80, 210, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 65, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 600, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	// 기본 세팅
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	MonsterAttackCollision->Off();

	// 넷 오브젝트 타입 설정
	SetNetObjectType(Net_ActorType::Empusa);

	LinkData_UpdatePacket<bool>(AnimationTurnStart);
	LinkData_UpdatePacket<bool>(IsHeavyAttack);
	LinkData_UpdatePacket<bool>(IsAirAttack);
	LinkData_UpdatePacket<bool>(IsSlamAttack);
	LinkData_UpdatePacket<bool>(IsBusterAttack);
	LinkData_UpdatePacket<bool>(IsVergilLight);
	LinkData_UpdatePacket<bool>(IsCollapse);
	LinkData_UpdatePacket<bool>(IsBurn);
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
			//Player = _Player;

			//DamageData Datas = Player->GetAttackCollision()->GetDamage();
			MonsterAttackCollision->Off();
			PlayDamageEvent(Datas.DamageTypeValue, Datas.SoundType);
			MinusEnemyHP(Datas.DamageValue);
			Sound.PlayVoiceRandom(4, 5, false);

			if (DamageType::Stun == Datas.DamageTypeValue)
			{
				StopTime(2.8f);
				AttackDelayCheck = 1.0f;
			}

			HitStop(Datas.DamageTypeValue);

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

		MonsterAttackCollision->Off();
		PlayDamageEvent(Datas.DamageTypeValue, Datas.SoundType);
		MinusEnemyHP(Datas.DamageValue);
		Sound.PlayVoiceRandom(4, 5, false);

		if (DamageType::VergilLight == Datas.DamageTypeValue)
		{
			IsVergilLight = true;
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

void Enemy_Empusa::PlayerChase()
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
			ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Start);
		}
		else
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Start);
		}
	}
		break;
	case EnemyRotation::Left:
	{
		AllDirectSetting();
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Start);
		}
		else
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Start);
		}
	}
		break;
	case EnemyRotation::Left_90:
		WaitTime += 0.2f;
		ChangeState(FSM_State_Empusa::Empusa_Turn_Left_90);
		break;
	case EnemyRotation::Left_180:
		WaitTime += 0.2f;
		ChangeState(FSM_State_Empusa::Empusa_Turn_Left_180);
		break;
	case EnemyRotation::Right:
	{
		AllDirectSetting();
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Start);
		}
		else
		{
			ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Start);
		}
	}
		break;
	case EnemyRotation::Right_90:
		WaitTime += 0.2f;
		ChangeState(FSM_State_Empusa::Empusa_Turn_Right_90);
		break;
	case EnemyRotation::Right_180:
		WaitTime += 0.2f;
		ChangeState(FSM_State_Empusa::Empusa_Turn_Right_180);
		break;
	default:
		break;
	}
}

void Enemy_Empusa::PlayerAttack()
{
	RotationCheck();
	AllDirectSetting();

	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

	if (0 == RandC)
	{
		ChangeState(FSM_State_Empusa::Empusa_Continuation_Attack);
		return;
	}

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		RandomAttack();
		break;
	case EnemyRotation::Left:
		ChangeState(FSM_State_Empusa::Empusa_Attack_C);
		break;
	case EnemyRotation::Left_90:
		ChangeState(FSM_State_Empusa::Empusa_Attack_C);
		break;
	case EnemyRotation::Left_180:
		ChangeState(FSM_State_Empusa::Empusa_Attack_C);
		break;
	case EnemyRotation::Right:
		ChangeState(FSM_State_Empusa::Empusa_Attack_D);
		break;
	case EnemyRotation::Right_90:
		ChangeState(FSM_State_Empusa::Empusa_Attack_D);
		break;
	case EnemyRotation::Right_180:
		ChangeState(FSM_State_Empusa::Empusa_Attack_D);
		break;
	default:
		break;
	}
}

void Enemy_Empusa::RandomAttack()
{
	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);

	if (0 == RandC)
	{
		ChangeState(FSM_State_Empusa::Empusa_Attack_A);
	}
	else if (1 == RandC)
	{
		ChangeState(FSM_State_Empusa::Empusa_Attack_B);
	}
	else if (2 == RandC)
	{
		ChangeState(FSM_State_Empusa::Empusa_Attack_W);
	}
}

void Enemy_Empusa::AttackCalculation()
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

void Enemy_Empusa::DamageCollisionCheck(float _DeltaTime)
{
	if (FSM_State_Empusa::Empusa_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_Empusa::Empusa_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_Empusa::Empusa_Downward_Death == EnemyFSM.GetCurState())
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
	MinusEnemyHP(Data.DamageValue);
	AttackDelayCheck = 0.0f;
	PlayDamageEvent(Data.DamageTypeValue, Data.SoundType);
	Sound.PlayVoiceRandom(4, 5, false);

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
			ChangeState(FSM_State_Empusa::Empusa_Air_Damage_Under);
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

	case DamageType::Heavy:
		ChangeState(FSM_State_Empusa::Empusa_Blown_Back);
		break;
	case DamageType::Air:
		ChangeState(FSM_State_Empusa::Empusa_Air_Damage);
		break;
	case DamageType::Snatch:
		ChangeState(FSM_State_Empusa::Empusa_Snatch);
		break;
	case DamageType::Slam:
		ChangeState(FSM_State_Empusa::Empusa_Slam_Damage);
		break;
	case DamageType::Buster:
		ChangeState(FSM_State_Empusa::Empusa_Buster_Start);
		break;
	case DamageType::Stun:
		AttackDelayCheck = 1.0f;
		StopTime(2.9f);
		return;
	default:
		break;
	}
}

void Enemy_Empusa::DamageCollisionCheck_Client(float _DeltaTime)
{
	if (FSM_State_Empusa::Empusa_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_Empusa::Empusa_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_Empusa::Empusa_Downward_Death == EnemyFSM.GetCurState())
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
	AttackDelayCheck = 0.0f;
	PlayDamageEvent(Data.DamageTypeValue, Data.SoundType);
	Sound.PlayVoiceRandom(4, 5, false);

	NetworkObjectBase* Obj = dynamic_cast<NetworkObjectBase*>(AttackCol->GetActor());

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
			ChangeState_Client(FSM_State_Empusa::Empusa_Air_Damage_Under, Obj);
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
			ChangeState_Client(FSM_State_Empusa::Empusa_Standing_Damage_Weak_Front, Obj);
			break;
		case EnemyHitDirect::Back:
			ChangeState_Client(FSM_State_Empusa::Empusa_Standing_Damage_Weak_Back, Obj);
			break;
		case EnemyHitDirect::Left:
			ChangeState_Client(FSM_State_Empusa::Empusa_Standing_Damage_Weak_Left, Obj);
			break;
		case EnemyHitDirect::Right:
			ChangeState_Client(FSM_State_Empusa::Empusa_Standing_Damage_Weak_Right, Obj);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState_Client(FSM_State_Empusa::Empusa_Blown_Back, Obj);
		break;
	case DamageType::Air:
		ChangeState_Client(FSM_State_Empusa::Empusa_Air_Damage, Obj);
		break;
	case DamageType::Snatch:
		ChangeState_Client(FSM_State_Empusa::Empusa_Snatch, Obj);
		break;
	case DamageType::Slam:
		ChangeState_Client(FSM_State_Empusa::Empusa_Slam_Damage, Obj);
		break;
	case DamageType::Buster:
		ChangeState_Client(FSM_State_Empusa::Empusa_Buster_Start, Obj);
		break;
	case DamageType::Stun:
		AttackDelayCheck = 1.0f;
		ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
		break;
	default:
		break;
	}
}

void Enemy_Empusa::RecognizeCollisionCheck(float _DeltaTime)
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

void Enemy_Empusa::MoveLoop()
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////     FSM     ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_Empusa::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
	IsChangeState = true;
}

void Enemy_Empusa::ChangeState_Client(int _StateValue, NetworkObjectBase* _Obj)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, _Obj);
}

void Enemy_Empusa::EnemyCreateFSM()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     등장     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 최초 등장_A
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Appear_A,
	.Start = [=] {
	EffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0100_enter_ground_A");
	},
	.Update = [=](float _DeltaTime) {
	EnemyRenderer->On();
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
		PhysXCapsule->On();
	}
		});
	// 최초 등장_B
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Appear_B,
	.Start = [=] {
	EffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0100_enter_ground_B");
	},
	.Update = [=](float _DeltaTime) {
	EnemyRenderer->On();
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
		PhysXCapsule->On();
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Idle     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 미어캣 Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Idle,
	.Start = [=] {
	IsRecognize = false;
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_biped_idle");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime += _DeltaTime;
	if (0.35f <= WaitTime)
	{
		if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
		{
			std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
			bool Iscontact = PlayerContactCheck_Normal(Collision.get());

			if (true == Iscontact)
			{
				IsRecognize = true;
			}
		}
		else
		{
			IsRecognize = false;
		}

		if (false == IsRecognize)
		{
			PlayerChase();
		}
		else
		{
			IsRecognize = false;
			PlayerAttack();
		}
		return;
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	}
	});

	// 돌격 Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Quadruped_Idle,
	.Start = [=] {
	IsRecognize = false;
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_quadruped_idle");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime += _DeltaTime;
	if (0.4f <= WaitTime)
	{
		ChangeState(FSM_State_Empusa::Empusa_Quadruped_To_Biped);
		return;
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	}
		});

	// 돌격 -> 미어캣 Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Quadruped_To_Biped,
	.Start = [=] {
	IsRecognize = false;
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_quadruped_to_biped");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      Move      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 백스텝
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Step_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_step_back");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		RotationCheck();
		AllDirectSetting();
		SetThrowback(45000.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Quadruped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 앞으로 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Start,
	.Start = [=] {
	MoveLoop();
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_start");
	},
	.Update = [=](float _DeltaTime) {
	{
		MoveLoop();
		if (8 < EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(100.0f);
		}
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0100_biped_walk_stop", 208, [=] { ++WalkCount; });
	}

	// 앞으로 걷기 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	{
		MoveLoop();
		SetForwardMove(140.0f);
	}
	if (0.5f >= MonsterAndPlayerDotProduct() || true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Stop);
		return;
	}

	if (1 == WalkCount)
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Walk_Stop);
		return;
	}
	},
	.End = [=] {
	WalkCount = 0;
	}
		});
	// 앞으로 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (48 > EnemyRenderer->GetCurFrame())
	{
		AllDirectSetting_Normal();
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		//RotationCheck();
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 앞으로 뛰기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Start,
	.Start = [=] {
	MoveLoop();
	EnemyRenderer->ChangeAnimation("em0100_biped_run_start");
	},
	.Update = [=](float _DeltaTime) {
	{
		MoveLoop();
		if (8 < EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(140.0f);
		}
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 앞으로 뛰기 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_loop");
	},
	.Update = [=](float _DeltaTime) {
	RunTime += _DeltaTime;
	{
		MoveLoop();
		SetForwardMove(200.0f);
	}
	if (0.5f >= MonsterAndPlayerDotProduct() || true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Stop);
		return;
	}

	if (2.0f <= RunTime)
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Run_Stop);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});
	// 앞으로 뛰기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (48 > EnemyRenderer->GetCurFrame())
	{
		AllDirectSetting_Normal();
		SetForwardMove(110.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		//RotationCheck();
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////   Rotation   //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 왼쪽 90도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left_90,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_turn_90_left");
	},
	.Update = [=](float _DeltaTime) {

	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, -90.f, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// 오른쪽 90도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right_90,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_turn_90_right");
	},
	.Update = [=](float _DeltaTime) {
	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 90.f, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// 왼쪽 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left_180,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_turn_180_left");
	},
	.Update = [=](float _DeltaTime) {
	if (30 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, -180.f, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 오른쪽 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right_180,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0100_turn_180_right");
	},
	.Update = [=](float _DeltaTime) {
	if (30 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     공 격     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	{
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_A", 135, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_A", 136, [=] { SetAdvance(29000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_B", 67, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_B", 68, [=] { SetAdvance(29000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_W", 135, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_W", 136, [=] { SetAdvance(29000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_C", 87, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_C", 88, [=] { SetAdvance(24000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_D", 87, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0100_attack_D", 88, [=] { SetAdvance(24000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0100_continuation_attack", 100, [=] { 	RotationCheck(); AllDirectSetting(); });
	}

	// 오른손 위에서 아래로 베기, 136 on, 142 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_A,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0100_attack_A");
	},
	.Update = [=](float _DeltaTime) {
	if (85 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 118)
	{
		SetForwardMove(70.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			ChangeState(FSM_State_Empusa::Empusa_Quadruped_To_Biped);
		}
		else
		{
			ChangeState(FSM_State_Empusa::Empusa_Step_Back);
		}
		return;
	}
	},
	.End = [=] {
	}
		});
	// 왼손 위에서 아래로 베기, 68 on, 73 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_B,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0100_attack_B");
	},
	.Update = [=](float _DeltaTime) {
	if (30 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 57)
	{
		SetForwardMove(70.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			ChangeState(FSM_State_Empusa::Empusa_Quadruped_To_Biped);
		}
		else
		{
			ChangeState(FSM_State_Empusa::Empusa_Step_Back);
		}
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	}
		});
	// 양손 내려찍기, 151 on, 155 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_W,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0100_attack_W");
	},
	.Update = [=](float _DeltaTime) {
	if (285 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 320)
	{
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Quadruped_To_Biped);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	}
		});
	// 오른손 횡베기, 60 on, 64 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_C,
	.Start = [=] {
	SlerpCalculation();
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0100_attack_C");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			ChangeState(FSM_State_Empusa::Empusa_Quadruped_To_Biped);
		}
		else
		{
			ChangeState(FSM_State_Empusa::Empusa_Step_Back);
		}
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	MonsterAttackCollision->Off();
	}
		});
	// 왼손 횡베기, 60 on, 65 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_D,
	.Start = [=] {
	SlerpCalculation();
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0100_attack_D");
	},
	.Update = [=](float _DeltaTime) {
	SlerpTurn(_DeltaTime);
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		int RandC = GameEngineRandom::MainRandom.RandomInt(0, 2);
		if (0 == RandC)
		{
			ChangeState(FSM_State_Empusa::Empusa_Quadruped_To_Biped);
		}
		else
		{
			ChangeState(FSM_State_Empusa::Empusa_Step_Back);
		}
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	MonsterAttackCollision->Off();
	}
		});

	// 돌진공격, 128/131, 161/163, 195/197, 230/233, 296/301
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Continuation_Attack,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0100_continuation_attack");
	},
	.Update = [=](float _DeltaTime) {
	if (100 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 300)
	{
		SetForwardMove(150.0f);
	}

	if (295 < EnemyRenderer->GetCurFrame() && AttackTypeChange == false)
	{
		AttackTypeChange = true;
		MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	AttackTypeChange = false;
	MonsterAttackCollision->Off();
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      피격      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////약공격
	// 정면 약공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_front", true);
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
			SetPush(26000.0f);
		}
	}
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_Empusa::Empusa_Death_Back);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 후면 약공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_back", true);
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
			SetPush(26000.0f);
		}
	}
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_Empusa::Empusa_Death_Front);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 좌측 약공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_left", true);
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
			SetPush(26000.0f);
		}
	}
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_Empusa::Empusa_Death_Back);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 우측 약공격 피격 
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_right", true);
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
			SetPush(26000.0f);
		}
	}
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_Empusa::Empusa_Death_Back);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////강공격
	// 강공격 맞고 날아감
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Blown_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_blown_back_start", true);
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
		ChangeState(FSM_State_Empusa::Empusa_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});
	/////////////////////////에어피격
	// 띄우기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Air_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_air_damage", true);
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

	if (true == FloorCheck(FallDistance) && 0.5f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_Empusa::Empusa_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0100_air_damage_under", 1, [=] {
			if (true == IsBusterAttack)
			{
				return;
			}
				
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Air_Damage_Under,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_air_damage_under", true);
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_Empusa::Empusa_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 강공격 피격, 에어피격 끝나고 땅에 닿을때
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Blown_Back_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0100_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Downward_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////슬램

	{
		EnemyRenderer->SetAnimationStartEvent("em0100_slam_damage_start", 1, [=] { SetAir(-120000.0f); });
	}

	// 슬램 피격 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_start");
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
		ChangeState(FSM_State_Empusa::Empusa_Slam_Damage_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 공중 loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_fall_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_Empusa::Empusa_Slam_Damage_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 후 땅에 부딫힘
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Downward_Getup);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스내치
	// 스내치 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_snatch_loop");
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
		ChangeState(FSM_State_Empusa::Empusa_Air_Damage_Under);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 스내치 end
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Snatch_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_snatch_end");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Air_Damage_Under);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스턴
	// 스턴 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_stun_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Stun_Revive);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 스턴 end
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_stun_revive");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      쓰러짐      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 앞으로 엎어졌을 때 일어나는 모션
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Getup,
	.Start = [=] {
	PhysXCapsule->AddWorldRotation({ 0.0f, -20.0f, 0.0f });
	EnemyRenderer->ChangeAnimation("em0100_downward_down_standup");
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_Empusa::Empusa_Downward_Death);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Biped_Idle);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	IsCollapse = false;
	}
		});
	// 앞으로 넘어진 상태에서 Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Death,
	.Start = [=] {
	Sound.PlayVoice(15);
	EnemyRenderer->ChangeAnimation("em0100_downward_die");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Death_Back,
	.Start = [=] {
	Sound.PlayVoice(15);
	EnemyRenderer->ChangeAnimation("em0100_death_back");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Death_Front,
	.Start = [=] {
	Sound.PlayVoice(15);
	EnemyRenderer->ChangeAnimation("em0100_death_front");
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

	// em0100_Buster_Start, 버스트 히트 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_air_damage_under");
	},
	.Update = [=](float _DeltaTime) {
	if (true == IsChangeState)
	{
		IsChangeState = false;
		IsCollapse = false;
		IsBusterAttack = true;
		AttackCalculation();
		BusterCalculation(float4{ 0.f, -45.f, 0.f });
		RotationCheck();
		AllDirectSetting();
	}
	SetMoveStop();
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		SetAir(-120000.0f);
		ChangeState(FSM_State_Empusa::Empusa_Buster_Loop);
		return;
	}
	},
	.End = [=] {
	BusterEnd();
	}
		});
	// 버스트 히트 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Loop,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_Empusa::Empusa_Buster_Finish);
		return;
	}
	},
	.End = [=] {
	}
		});
	// em0100_Buster_Finish, 버스트 히트 땅에 떨어짐
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Finish,
	.Start = [=] {
	MinusEnemyHP(500);
	Sound.Play("Enemy_Damage_", 12);
	Sound.PlayVoiceRandom(4, 5, false);
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0100_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_Empusa::Empusa_Downward_Getup);
		return;
	}
	},
	.End = [=] {

	}
		});

	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);

	if (0 == RandC)
	{
		EnemyFSM.ChangeState(FSM_State_Empusa::Empusa_Appear_A);
		IsAppearA = true;
	}
	else
	{
		EnemyFSM.ChangeState(FSM_State_Empusa::Empusa_Appear_B);
		IsAppearA = false;
	}
}

void Enemy_Empusa::EnemyCreateFSM_Client()
{
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Appear_A,
	.Start = [=] {
	EffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0100_enter_ground_A");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Appear_B,
	.Start = [=] {
	EffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0100_enter_ground_B");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_idle");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Quadruped_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_quadruped_idle");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Quadruped_To_Biped,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_quadruped_to_biped");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Step_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_step_back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_walk_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Biped_Run_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_biped_run_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left_90,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_turn_90_left");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right_90,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_turn_90_right");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Left_180,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_turn_180_left");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Turn_Right_180,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_turn_180_right");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_A,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_A");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_B,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_B");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_C,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_C");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_D,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_D");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Attack_W,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_attack_W");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Continuation_Attack,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_continuation_attack");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_front", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_back", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_left", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Standing_Damage_Weak_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_angledamage_right", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Blown_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_blown_back_start", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Air_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_air_damage", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Air_Damage_Under,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_air_damage_under", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Blown_Back_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_fall_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Slam_Damage_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_snatch_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Snatch_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_snatch_end");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_stun_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_stun_revive");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_downward_down_standup");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Downward_Death,
	.Start = [=] {
	Sound.PlayVoice(15);
	EnemyRenderer->ChangeAnimation("em0100_downward_die");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Death_Back,
	.Start = [=] {
	Sound.PlayVoice(15);
	EnemyRenderer->ChangeAnimation("em0100_death_back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Death_Front,
	.Start = [=] {
	Sound.PlayVoice(15);
	EnemyRenderer->ChangeAnimation("em0100_death_front");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0100_air_damage_under");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Loop,
	.Start = [=] {
	//EnemyRenderer->ChangeAnimation("em0100_slam_damage_fall_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Empusa::Empusa_Buster_Finish,
	.Start = [=] {
	Sound.Play("Enemy_Damage_", 12);
	EnemyRenderer->ChangeAnimation("em0100_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {

	}
		});

	if (IsAppearA == true)
	{
		EnemyFSM.ChangeState(FSM_State_Empusa::Empusa_Appear_A);
	}
	else
	{
		EnemyFSM.ChangeState(FSM_State_Empusa::Empusa_Appear_B);
	}
}
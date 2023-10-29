#include "PrecompileHeader.h"
#include "Enemy_HellCaina.h"

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
Enemy_HellCaina::Enemy_HellCaina()
{
}

Enemy_HellCaina::~Enemy_HellCaina()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////   Actor Init   ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_HellCaina::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::HellCaina;
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		EnemyMaxHP = 1500;
	}
	else
	{
		EnemyMaxHP = 3000;
	}
	
	EnemyHP = EnemyMaxHP;
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

	if (nullptr == GameEngineTexture::Find("PaperBurnNoise.jpg"))
	{
		GameEngineTexture::Load(GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Texture", "BurnNoiseTexture"
			}, "PaperBurnNoise.jpg"));
	}

	EnemyRenderer->SetTexture("PaperBurnTexture", "PaperBurnNoise.jpg");
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
			.RendererLocalPos = { 0.0f, -45.0f, 0.0f },
			.Objects = {(GameEngineObject*)MonsterAttackCollision.get()},
			.CallBacks_void =
			{
			},
			.CallBacks_int =
			{
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1),
				std::bind(&SoundController::Play, &Sound, "HellCaina_SFX_",std::placeholders::_1),
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
	Sound.SetVoiceName("HellCaina_V_");
	if (nullptr == GameEngineSound::Find("HellCaina_V_0.wav")) {
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("Voice");
		NewDir.Move("HellCaina");
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
		}

		NewDir.MoveParent();
		NewDir.MoveParent();
		NewDir.Move("SFX");
		NewDir.Move("HellCaina");
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

void Enemy_HellCaina::Start()
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

	//MonsterCollision->Off();
	//RN_MonsterCollision->Off();
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	MonsterAttackCollision->Off();

	// 넷 오브젝트 타입 설정
	SetNetObjectType(Net_ActorType::HellCaina);

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
			if (nullptr == _Player)
			{
				MsgAssert("잘못된 DamageCallBack 이벤트입니다");
				return;
			}
			//Player = _Player;

			//DamageData Datas = Player->GetAttackCollision()->GetDamage();
			MonsterAttackCollision->Off();
			DamageData Datas = _Player->GetAttackCollision()->GetDamage();
			MinusEnemyHP(Datas.DamageValue);
			PlayDamageEvent(Datas.DamageTypeValue, Datas.SoundType);
			Sound.PlayVoice(3, false);

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

	SetDamagedNetCallBack<BasePlayerActor>([this](BasePlayerActor* _Attacker) {

		Player = _Attacker;

		MonsterAttackCollision->Off();
		DamageData Datas = _Attacker->GetAttackCollision()->GetDamage();
		MinusEnemyHP(Datas.DamageValue);
		PlayDamageEvent(Datas.DamageTypeValue, Datas.SoundType);
		Sound.PlayVoice(3, false);

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

void Enemy_HellCaina::PlayerChase()
{
	RotationCheck();

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		AllDirectSetting();
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Start);
		break;
	case EnemyRotation::Left:
		AllDirectSetting();
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellCaina::HellCaina_Navi_Turn_Left);
		break;
	case EnemyRotation::Left_90:
		AllDirectSetting();
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellCaina::HellCaina_Navi_Turn_Left);
		break;
	case EnemyRotation::Left_180:
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellCaina::HellCaina_Turn_Left_180);
		break;
	case EnemyRotation::Right:
		AllDirectSetting();
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellCaina::HellCaina_Navi_Turn_Right);
		break;
	case EnemyRotation::Right_90:
		AllDirectSetting();
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellCaina::HellCaina_Navi_Turn_Right);
		break;
	case EnemyRotation::Right_180:
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellCaina::HellCaina_Turn_Right_180);
		break;
	default:
		break;
	}
}

void Enemy_HellCaina::PlayerAttack()
{
	RotationCheck();
	AllDirectSetting();

	if (EnemyRotation::Left == EnemyRotationValue)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Right_Start);
	}
	else if (EnemyRotation::Right == EnemyRotationValue)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Left_Start);
	}
	else if (EnemyRotation::Left_180 == EnemyRotationValue || EnemyRotation::Right_180 == EnemyRotationValue)
	{
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Attack_Turn);
	}
	else
	{
		RandomAttack();
	}
}

void Enemy_HellCaina::RandomAttack()
{
	int RandC = GameEngineRandom::MainRandom.RandomInt(0, 1);

	if (0 == RandC)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Attack_DownUp);
	}
	else
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Attack_UpDown);
	}
}

void Enemy_HellCaina::AttackCalculation()
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

void Enemy_HellCaina::DamageCollisionCheck(float _DeltaTime)
{
	if (FSM_State_HellCaina::HellCaina_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_HellCaina::HellCaina_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_HellCaina::HellCaina_Prone_Death == EnemyFSM.GetCurState())
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
	AttackDelayCheck = 0.0f;

	DamageData Data = AttackCol->GetDamage();
	MinusEnemyHP(Data.DamageValue);
	PlayDamageEvent(Data.DamageTypeValue, Data.SoundType);
	Sound.PlayVoice(3, false);

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
			ChangeState(FSM_State_HellCaina::HellCaina_Air_Damage_Under);
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
			ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back);
		break;
	case DamageType::Air:
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Up);
		break;
	case DamageType::Snatch:
		ChangeState(FSM_State_HellCaina::HellCaina_Snatch);
		break;
	case DamageType::Slam:
		ChangeState(FSM_State_HellCaina::HellCaina_Slam_Damage);
		break;
	case DamageType::Buster:
		ChangeState(FSM_State_HellCaina::HellCaina_Buster_Start);
		break;
	case DamageType::Stun:
		StopTime(2.9f);
		AttackDelayCheck = 1.0f;
		return;
	default:
		break;
	}
}

void Enemy_HellCaina::DamageCollisionCheck_Client(float _DeltaTime)
{
	if (FSM_State_HellCaina::HellCaina_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_HellCaina::HellCaina_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_HellCaina::HellCaina_Prone_Death == EnemyFSM.GetCurState())
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
	DamageData Data = AttackCol->GetDamage();
	AttackDelayCheck = 0.0f;
	PlayDamageEvent(Data.DamageTypeValue, Data.SoundType);
	Sound.PlayVoice(3, false);

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
			ChangeState_Client(FSM_State_HellCaina::HellCaina_Air_Damage_Under);
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
			ChangeState_Client(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			ChangeState_Client(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			ChangeState_Client(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			ChangeState_Client(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState_Client(FSM_State_HellCaina::HellCaina_Blown_Back);
		break;
	case DamageType::Air:
		ChangeState_Client(FSM_State_HellCaina::HellCaina_Blown_Up);
		break;
	case DamageType::Snatch:
		ChangeState_Client(FSM_State_HellCaina::HellCaina_Snatch);
		break;
	case DamageType::Slam:
		ChangeState_Client(FSM_State_HellCaina::HellCaina_Slam_Damage);
		break;
	case DamageType::Buster:
		ChangeState_Client(FSM_State_HellCaina::HellCaina_Buster_Start);
		break;
	case DamageType::Stun:
		ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Player });
		AttackDelayCheck = 1.0f;
		break;
	default:
		break;
	}
}

void Enemy_HellCaina::RecognizeCollisionCheck(float _DeltaTime)
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

void Enemy_HellCaina::MoveLoop()
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

void Enemy_HellCaina::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
	IsChangeState = true;
}

void Enemy_HellCaina::ChangeState_Client(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, Player);
}

void Enemy_HellCaina::EnemyCreateFSM()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     등장     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 최초 등장_02
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Appear_02,
	.Start = [=] {
	EffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0000_appear_02");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
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

	// Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Idle,
	.Start = [=] {
	IsRecognize = false;
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_Idle_01");
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
	// 앞으로 함성 위협
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Menace_Front,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_menace_01");
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
	// 위로 함성 위협
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Menace_Up,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_menace_02");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		RotationCheck();
		AllDirectSetting();
		ChangeState(FSM_State_HellCaina::HellCaina_Attack_Dash);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      Move      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	{
		EnemyRenderer->SetAnimationStartEvent("em0000_walk_start", 1, [=] {

			int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);

			if (0 == RandC)
			{
				ChangeState(FSM_State_HellCaina::HellCaina_Menace_Up);
			}

			; });
	}

	// 앞으로 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Start,
	.Start = [=] {
	MoveLoop();
	EnemyRenderer->ChangeAnimation("em0000_walk_start");
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
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 앞으로 걷기 Loop

	{
		EnemyRenderer->SetAnimationStartEvent("em0000_walk_loop", 208, [=] { ++WalkCount; });
	}

	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	{
		MoveLoop();
		SetForwardMove(140.0f);
	}
	if (0.5f >= MonsterAndPlayerDotProduct() || true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Stop);
		return;
	}

	if (1 == WalkCount)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Stop);
		return;
	}
	},
	.End = [=] {
	WalkCount = 0;
	}
		});
	// 앞으로 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_stop");
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
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 오른쪽 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_start");
	},
	.Update = [=](float _DeltaTime) {
	if (5 < EnemyRenderer->GetCurFrame())
	{
		SetRightMove(35.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Right_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 오른쪽 걷기 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_loop");
	},
	.Update = [=](float _DeltaTime) {
	SetRightMove(70.0f);
	AttackDelayTime += _DeltaTime;
	if (2.3f <= AttackDelayTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Right_Stop);
		return;
	}
	else if (1.7f <= AttackDelayTime)
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

		if (true == IsRecognize)
		{
			IsRecognize = false;

			if (EnemyRotation::Left_180 == EnemyRotationValue || EnemyRotation::Right_180 == EnemyRotationValue)
			{
				RotationCheck();
				AllDirectSetting();
				ChangeState(FSM_State_HellCaina::HellCaina_Attack_Turn);
			}
			else
			{
				RotationCheck();
				AllDirectSetting();
				RandomAttack();
			}
		}
	}
	},
	.End = [=] {
	AttackDelayTime = 0.0f;
	WalkCount = 0;
	}
		});
	// 오른쪽 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (30 > EnemyRenderer->GetCurFrame())
	{
		SetRightMove(20.0f);
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

	// 왼쪽 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_start");
	},
	.Update = [=](float _DeltaTime) {
	if (5 < EnemyRenderer->GetCurFrame())
	{
		SetLeftMove(35.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Left_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 왼쪽 걷기 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_loop");
	},
	.Update = [=](float _DeltaTime) {
	SetLeftMove(70.0f);
	AttackDelayTime += _DeltaTime;
	if (2.3f <= AttackDelayTime)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Walk_Left_Stop);
		return;
	}
	else if (1.7f <= AttackDelayTime)
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

		if (true == IsRecognize)
		{
			IsRecognize = false;
			if (EnemyRotation::Left_180 == EnemyRotationValue || EnemyRotation::Right_180 == EnemyRotationValue)
			{
				RotationCheck();
				AllDirectSetting();
				ChangeState(FSM_State_HellCaina::HellCaina_Attack_Turn);
			}
			else
			{
				RotationCheck();
				AllDirectSetting();
				RandomAttack();
			}
		}
	}
	},
	.End = [=] {
	AttackDelayTime = 0.0f;
	WalkCount = 0;
	}
		});
	// 왼쪽 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_stop");
	},
	.Update = [=](float _DeltaTime) {
	if (30 > EnemyRenderer->GetCurFrame())
	{
		SetLeftMove(20.0f);
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
	/////////////////////////////////   Rotation   //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 왼쪽 슬러프 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Navi_Turn_Left,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0000_navi_turn_left_90");
	},
	.Update = [=](float _DeltaTime) {

	SlerpTurn(_DeltaTime);

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// 오른쪽 슬러프 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Navi_Turn_Right,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0000_navi_turn_right_90");
	},
	.Update = [=](float _DeltaTime) {

	SlerpTurn(_DeltaTime);

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// 왼쪽 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Left_180,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_turn_left_180");
	},
	.Update = [=](float _DeltaTime) {
	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, -180.f, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 왼쪽 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Right_180,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_turn_right_180");
	},
	.Update = [=](float _DeltaTime) {
	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
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
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_01", 62, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_01", 63, [=] { SetAdvance(33000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_02", 57, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_02", 58, [=] { SetAdvance(33000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_01_turn", 70, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_01_turn", 71, [=] { SetThrowback(-33000.0f); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_atackhard", 116, [=] { SetMoveStop(); });
		EnemyRenderer->SetAnimationStartEvent("em0000_attack_atackhard", 117, [=] { SetAdvance(34000.0f); });
	}

	// 아래에서 위로 횡베기, 70 on, 74 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_DownUp,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_attack_01");
	},
	.Update = [=](float _DeltaTime) {
	if (EnemyRenderer->GetCurFrame() <= 61)
	{
		SetForwardMove(90.0f);
	}
	if (138 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 178)
	{
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	}
		});
	// 위에서 아래로 횡베기, 79 on, 83 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_UpDown,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_attack_02");
	},
	.Update = [=](float _DeltaTime) {
	if (5 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 55)
	{
		SetForwardMove(80.0f);
	}
	if (138 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 160)
	{
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	}
		});
	// 뒤에 있으면 턴해서 공격, 68 on, 72 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_Turn,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_attack_01_turn");
	},
	.Update = [=](float _DeltaTime) {
	if (20 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (10 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 65)
	{
		SetForwardMove(80.0f);
	}
	if (138 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 160)
	{
		SetForwardMove(90.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	}
		});
	// 돌진베기, 110 start, 133 on, 116 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_Dash,
	.Start = [=] {
	DashAttackSetting = true;
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0000_attack_atackhard");
	},
	.Update = [=](float _DeltaTime) {
	{
		MoveLoop();
		if (10 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 115)
		{
			SetForwardMove(500.0f);
		}
	}

	if (true == IsRecognize && true == DashAttackSetting)
	{
		IsRecognize = false;
		DashAttackSetting = false;
		EnemyRenderer->SetCurFrame(115);
	}

	if (345 < EnemyRenderer->GetCurFrame() && EnemyRenderer->GetCurFrame() <= 390)
	{
		SetForwardMove(60.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	DashAttackSetting = false;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      피격      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////약공격
	// 정면 약공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_front_01", true);
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
		ChangeState(FSM_State_HellCaina::HellCaina_Death_Front);
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

	// 후면 약공격 피격 (뒤로돈다)
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_back_01", true);
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
		ChangeState(FSM_State_HellCaina::HellCaina_Death_Back);
		return;
	}
	if (90 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.0f, 180.0f, 0.0f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 좌측 약공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_left_01", true);
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
		ChangeState(FSM_State_HellCaina::HellCaina_Death_Front);
		return;
	}
	if (90 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.0f, -90.0f, 0.0f });
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 우측 약공격 피격 
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_right_01", true);
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
		ChangeState(FSM_State_HellCaina::HellCaina_Death_Front);
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
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////강공격
	// 강공격 맞고 날아감
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back,
	.Start = [=] {

	EnemyRenderer->ChangeAnimation("em0000_blown_back", true);
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
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});
	/////////////////////////에어피격
	// 띄우기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Up,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_up", true);
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
		ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back_Landing);
		return;
	}
	},
	.End = [=] {
	FallCheckDelayTime = 0.0f;
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0000_air_damage_under", 1, [=] {
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Air_Damage_Under,
	.Start = [=] {
			//AttackCalculation();
			EnemyRenderer->ChangeAnimation("em0000_air_damage_under", true);
			},
			.Update = [=](float _DeltaTime) {
			if (true == FloorCheck(FallDistance))
			{
				ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back_Landing);
				return;
			}
			},
			.End = [=] {
			}
		});
	// 강공격 피격, 에어피격 끝나고 땅에 닿을때
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0000_blown_back_landing");
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

	/////////////////////////슬램

	{
		EnemyRenderer->SetAnimationStartEvent("em0000_slam_damage", 1, [=] { SetAir(-120000.0f); });
	}

	// 슬램 피격 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage");
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
		ChangeState(FSM_State_HellCaina::HellCaina_Slam_Damage_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 공중 loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage_loop");
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Slam_Damage_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 슬램 피격 후 땅에 부딫힘
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage_Landing,
	.Start = [=] {
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0000_slam_damage_landing");
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

	/////////////////////////스내치
	// 스내치 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_snatch");
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
		ChangeState(FSM_State_HellCaina::HellCaina_Air_Damage_Under);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////스턴
	// 스턴 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_stun");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Stun_Revive);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 스턴 end
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_stun_revive");
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
	/////////////////////////////////      쓰러짐      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	// 
	// 앞으로 엎어졌을 때 일어나는 모션
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_getup");
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Prone_Death);
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Idle);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	IsCollapse = false;
	}
		});
	// 앞으로 넘어진 상태에서 Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Death,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_death");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Death_Back,
	.Start = [=] {
	Sound.PlayVoice(5, false);
	EnemyRenderer->ChangeAnimation("em0000_death_back");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Death_Front,
	.Start = [=] {
	Sound.PlayVoice(5, false);
	EnemyRenderer->ChangeAnimation("em0000_death_front");
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Buster_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_Buster_Start");
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
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
	}
	SetMoveStop();
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		BusterEnd();
		SetAir(-120000.0f);
		ChangeState(FSM_State_HellCaina::HellCaina_Buster_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 버스트 히트 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Buster_Loop,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime) {
	if (true == FloorCheck(FallDistance))
	{
		ChangeState(FSM_State_HellCaina::HellCaina_Buster_Finish);
		return;
	}
	},
	.End = [=] {
	}
		});
	// em0000_Buster_Finish, 버스트 히트 땅에 떨어짐
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Buster_Finish,
	.Start = [=] {
	MinusEnemyHP(500);
	Sound.Play("Enemy_Damage_", 12);
	Sound.PlayVoice(3);
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsCollapse = true;
	EnemyRenderer->ChangeAnimation("em0000_Buster_Finish");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellCaina::HellCaina_Prone_Getup);
		return;
	}
	},
	.End = [=] {

	}
		});

	EnemyFSM.ChangeState(FSM_State_HellCaina::HellCaina_Appear_02);
}

void Enemy_HellCaina::EnemyCreateFSM_Client()
{
	// 최초 등장_02
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Appear_02,
	.Start = [=] {
	EffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0000_appear_02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_Idle_01");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 앞으로 함성 위협
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Menace_Front,
	.Start = [=] {
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
	EnemyRenderer->ChangeAnimation("em0000_menace_02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	// 앞으로 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 앞으로 걷기 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 앞으로 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	// 오른쪽 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 오른쪽 걷기 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 오른쪽 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Right_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_right_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 왼쪽 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 왼쪽 걷기 Loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 왼쪽 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Walk_Left_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_walk_left_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	// 왼쪽 슬러프 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Navi_Turn_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_navi_turn_left_90");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 오른쪽 슬러프 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Navi_Turn_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_navi_turn_right_90");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 왼쪽 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Left_180,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_turn_left_180");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 왼쪽 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Turn_Right_180,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_turn_right_180");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 아래에서 위로 횡베기
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_DownUp,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_attack_01");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 위에서 아래로 횡베기
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_UpDown,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_attack_02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 뒤에 있으면 턴해서 공격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_Turn,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_attack_01_turn");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 돌진공격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Attack_Dash,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_attack_atackhard");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_front_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_back_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 좌측 약공격 피격
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_left_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 우측 약공격 피격 
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_standing_damage_weak_right_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_back", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 띄우기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Up,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_up", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	// 에어 상태에서 약공격 맞을때
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Air_Damage_Under,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_air_damage_under", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 강공격 피격, 에어피격 끝나고 땅에 닿을때
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Blown_Back_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	// 슬램 피격 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 슬램 피격 공중 loop
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 슬램 피격 후 땅에 부딫힘
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Slam_Damage_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	// 스내치 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_snatch");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	// 스턴 start
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_stun");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 스턴 end
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_stun_revive");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 앞으로 엎어졌을 때 일어나는 모션
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_getup");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 앞으로 넘어진 상태에서 Death
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Prone_Death,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_prone_death");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 앞으로 쓰러지면서 죽음
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Death_Back,
	.Start = [=] {
	Sound.PlayVoice(5, false);
	EnemyRenderer->ChangeAnimation("em0000_death_back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 뒤로 쓰러지면서 죽음
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Death_Front,
	.Start = [=] {
	Sound.PlayVoice(5, false);
	EnemyRenderer->ChangeAnimation("em0000_death_front");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	// em0000_Buster_Start, 버스트 히트 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Buster_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_Buster_Start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// 버스트 히트 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Buster_Loop,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	// em0000_Buster_Finish, 버스트 히트 땅에 떨어짐
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellCaina::HellCaina_Buster_Finish,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0000_Buster_Finish");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {

	}
		});

	EnemyFSM.ChangeState(FSM_State_HellCaina::HellCaina_Appear_02);
}
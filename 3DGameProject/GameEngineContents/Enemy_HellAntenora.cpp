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
#include "FXSystem.h"
#include "Player_MirageBlade.h"

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
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		EnemyMaxHP = 2500;
	}
	else
	{
		EnemyMaxHP = 5000;
	}

	EnemyHP = EnemyMaxHP;
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
	Sound.SetVoiceName("HellAntenora_V_");
	if (nullptr == GameEngineSound::Find("HellAntenora_V_0.wav")) {
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("Voice");
		NewDir.Move("HellAntenora");
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

		if (nullptr == GameEngineSound::Find("HellCaina_SFX_0.wav")) {
			for (size_t i = 0; i < Files.size(); i++)
			{
				GameEngineSound::Load(Files[i].GetFullPath());
			}
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

void Enemy_HellAntenora::Start()
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
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);

	MonsterAttackCollision_Two = CreateComponent<AttackCollision>(CollisionOrder::EnemyAttack);
	MonsterAttackCollision_Two->GetTransform()->SetWorldScale(float4::ZERO);
	MonsterAttackCollision_Two->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	MonsterAttackCollision_Two->SetColType(ColType::OBBBOX3D);

	MonsterCollision->GetTransform()->SetLocalScale({ 80, 210, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 65, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 700, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	// 기본 세팅
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	//MonsterCollision->Off();
	//RN_MonsterCollision->Off();

	// 무기 붙이기
	EnemyRenderer->SetAttachTransform("L_WeaponHand", LeftWeapon, float4(0.0f, 0.0f, 0.0f), float4(30.0f, 170.0f, 180.0f), true, true);
	EnemyRenderer->SetAttachTransform("R_WeaponHand", RightWeapon, float4(0.0f, 0.0f, 0.0f), float4(30.0f, 170.0f, 180.0f), true, true);

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
	LinkData_UpdatePacket<bool>(DashAttackSetting);
	LinkData_UpdatePacket<bool>(IsBurn);
	LinkData_UpdatePacket<bool>(DeathValue);
	LinkData_UpdatePacket<int>(EnemyHP);
	LinkData_UpdatePacket<int>(ServerPlayerID);

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

			PlayDamageEvent(Datas.DamageTypeValue, Datas.SoundType);
			Sound.PlayVoice(5);

			MultiAttackStack = 0;
			MinusEnemyHP(Datas.DamageValue);
			HitStop(Datas.DamageTypeValue);

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

		PlayDamageEvent(Datas.DamageTypeValue, Datas.SoundType);
		Sound.PlayVoice(5);
		
		MonsterAttackCollision->Off();
		MultiAttackStack = 0;
		MinusEnemyHP(Datas.DamageValue);
		HitStop(Datas.DamageTypeValue);

		if (DamageType::VergilLight == Datas.DamageTypeValue)
		{
			if (false == DashAttackSetting)
			{
				IsVergilLight = true;
			}
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

void Enemy_HellAntenora::PlayerChase()
{
	RotationCheck();

	switch (EnemyRotationValue)
	{
	case EnemyRotation::Forward:
		AllDirectSetting();
		ChangeState(FSM_State_HellAntenora::HellAntenora_Walk_Start);
		break;
	case EnemyRotation::Left:
		AllDirectSetting();
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellAntenora::HellAntenora_Navi_Turn_Left);
		break;
	case EnemyRotation::Left_90:
		AllDirectSetting();
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellAntenora::HellAntenora_Navi_Turn_Left);
		break;
	case EnemyRotation::Left_180:
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellAntenora::HellAntenora_Turn_Left_180);
		break;
	case EnemyRotation::Right:
		AllDirectSetting();
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellAntenora::HellAntenora_Navi_Turn_Right);
		break;
	case EnemyRotation::Right_90:
		AllDirectSetting();
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellAntenora::HellAntenora_Navi_Turn_Right);
		break;
	case EnemyRotation::Right_180:
		WaitTime += 0.2f;
		ChangeState(FSM_State_HellAntenora::HellAntenora_Turn_Right_180);
		break;
	default:
		break;
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
	if (FSM_State_HellAntenora::HellAntenora_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_HellAntenora::HellAntenora_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_HellAntenora::HellAntenora_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_HellAntenora::HellAntenora_Blown_Up_Landing == EnemyFSM.GetCurState())
	{
		return;
	}

	//if (true == DeathValue)
	//{
	//	return;
	//}

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
	MultiAttackStack = 0;
	PlayDamageEvent(Data.DamageTypeValue, Data.SoundType);
	Sound.PlayVoice(5);

	if (DamageType::VergilLight == Data.DamageTypeValue)
	{
		IsVergilLight = true;
		Data.DamageTypeValue = DamageType::Light;
	}

	if (true == DashAttackSetting)
	{
		if (Data.DamageTypeValue == DamageType::Buster)
		{
			MinusEnemyHP(Data.DamageValue);
			ChangeState(FSM_State_HellAntenora::HellAntenora_Buster_Start);
		}
		else
		{
			HitStop(Data.DamageTypeValue);
			IsHeavyAttack = false;
			IsAirAttack = false;
			IsSlamAttack = false;
			IsBusterAttack = false;
			IsVergilLight = false;
			IsCollapse = false;
			IsRecognize = false;
			HitStop(Data.DamageTypeValue);
			StartRenderShaking(8);
		}
		return;
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
			ChangeState(FSM_State_HellAntenora::HellAntenora_Air_Damage_Under);
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
		AttackDelayCheck = 1.0f;
		StopTime(2.9f);
		return;
	default:
		break;
	}
}

void Enemy_HellAntenora::DamageCollisionCheck_Client(float _DeltaTime)
{
	if (FSM_State_HellAntenora::HellAntenora_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_HellAntenora::HellAntenora_Death_Back == EnemyFSM.GetCurState()
		|| FSM_State_HellAntenora::HellAntenora_Death_Front == EnemyFSM.GetCurState()
		|| FSM_State_HellAntenora::HellAntenora_Blown_Up_Landing == EnemyFSM.GetCurState())
	{
		return;
	}

	//if (true == DeathValue)
	//{
	//	return;
	//}

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
	PlayDamageEvent(Data.DamageTypeValue, Data.SoundType);
	AttackDelayCheck = 0.0f;
	Sound.PlayVoice(5);

	NetworkObjectBase* Obj = dynamic_cast<NetworkObjectBase*>(AttackCol->GetActor());

	if (DamageType::VergilLight == Data.DamageTypeValue)
	{
		Data.DamageTypeValue = DamageType::Light;
	}

	if (true == DashAttackSetting)
	{
		if (Data.DamageTypeValue == DamageType::Buster)
		{
			BusterClientStart = true;
			ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Buster_Start, Obj);
		}
		else
		{
			ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
			HitStop(Data.DamageTypeValue);
			StartRenderShaking(8);
		}
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
			ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Air_Damage_Under, Obj);
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
			ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Front, Obj);
			break;
		case EnemyHitDirect::Back:
			ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Back, Obj);
			break;
		case EnemyHitDirect::Left:
			ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Left, Obj);
			break;
		case EnemyHitDirect::Right:
			ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Right, Obj);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Blown_Back, Obj);
		break;
	case DamageType::Air:
		ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Blown_Up, Obj);
		break;
	case DamageType::Snatch:
		ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Snatch, Obj);
		break;
	case DamageType::Slam:
		ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Slam_Damage, Obj);
		break;
	case DamageType::Buster:
		BusterClientStart = true;
		ChangeState_Client(FSM_State_HellAntenora::HellAntenora_Buster_Start, Obj);
		break;
	case DamageType::Stun:
		AttackDelayCheck = 1.0f;
		ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
		break;
	default:
		break;
	}
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

void Enemy_HellAntenora::MoveLoop()
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

void Enemy_HellAntenora::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
	IsChangeState = true;
}

void Enemy_HellAntenora::ChangeState_Client(int _StateValue, NetworkObjectBase* _Obj)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, _Obj);
}

void Enemy_HellAntenora::EnemyCreateFSM()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Basic     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 최초 등장_02
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Appear_01,
	.Start = [=] {
	MonsterEffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0001_appear_01");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
		PhysXCapsule->On();
	}		

		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Basic     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// Idle
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Idle,
	.Start = [=] {
	IsRecognize = false;
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0001_Idle_01_loop");
	},
	.Update = [=](float _DeltaTime) 
	{
	WaitTime += _DeltaTime;
	if (0.35f <= WaitTime)
	{
		if (true == IsRecognize)
		{
			IsRecognize = false;

			RotationCheck();
			AllDirectSetting();

			if (EnemyRotation::Left_180 == EnemyRotationValue)
			{
				ChangeState(FSM_State_HellAntenora::HellAntenora_Turn_Attack_Left);
			}
			else if (EnemyRotation::Right_180 == EnemyRotationValue)
			{
				ChangeState(FSM_State_HellAntenora::HellAntenora_Turn_Attack_Right);
			}
			else
			{
				ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_Start);
			}
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

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Move     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Walk_Start,
	.Start = [=] 
	{
	MoveLoop();
	EnemyRenderer->ChangeAnimation("em0001_walk_start");
	},
	.Update = [=](float _DeltaTime)
	{
	{
		MoveLoop();
		if (40 <= EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(170.0f);
		}
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Walk_Loop);
		return;
	}
	},
	.End = [=] 
	{
	}
		});

	// 걷기 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_walk_loop");
	},
	.Update = [=](float _DeltaTime)
	{
	RunTime += _DeltaTime;
	{
		MoveLoop();
		SetForwardMove(210.0f);
	}

	if (true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_Start);
		return;
	}
	else if (RunTime >= 4.0f)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Walk_End);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});

	// 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Walk_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_walk_stop");
	},
	.Update = [=](float _DeltaTime)
	{

	{
		AllDirectSetting_Normal();
		if (50 >= EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(170.0f);
		}
	}

	if (70 <= EnemyRenderer->GetCurFrame())
	{
		RotationCheck();
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Navi_Turn_Left,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0001_turn_left_90_noTransRot");
	},
	.Update = [=](float _DeltaTime) {

	SlerpTurn(_DeltaTime);

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// 오른쪽 슬러프 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Navi_Turn_Right,
	.Start = [=] {
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0001_turn_right_90_noTransRot");
	},
	.Update = [=](float _DeltaTime) {

	SlerpTurn(_DeltaTime);

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});
	// 왼쪽 180도 회전, 25프레임 애니메이션 턴
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Turn_Left_180,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0001_turn_left_180");
	},
	.Update = [=](float _DeltaTime) {
	if (25 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, -180.f, 0.f });
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});
	// 왼쪽 180도 회전, 25프레임 애니메이션 턴
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Turn_Right_180,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0001_turn_right_180");
	},
	.Update = [=](float _DeltaTime) {
	if (25 <= EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      Attack      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	{
		EnemyRenderer->SetAnimationStartEvent("em0001_Attack_counter_landing", 1,
			[=]
			{
				SetThrowback(45000.0f);
			});
	}

	// 돌진 전 뒤구르기
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Counter_Start,
	.Start = [=] {
	DashAttackSetting = true;
	IsHeavyAttack = false;
	IsAirAttack = false;
	IsSlamAttack = false;
	IsBusterAttack = false;
	IsRecognize = false;
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_Start");
	},
	.Update = [=](float _DeltaTime) {
	
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Counter_Landing);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	DashAttackSetting = false;
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0001_Attack_counter_landing", 1,
			[=]
			{
				if (true == IsAirAttackSetting)
				{
					SetThrowback(45000.0f);
				}
			});
	}

	// 엎드렸다가 돌진, 36 ~ 60 ~ 105(end)
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Counter_Landing,
	.Start = [=] {
	IsRecognize = false;
	DashAttackSetting = true;
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_landing");
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Up_Landing);
		return;
	}
	{
		MoveLoop();

		if (36 <= EnemyRenderer->GetCurFrame() && 60 > EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(300.0f);
		}
		else if (60 <= EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(500.0f);
		}
	}
	if (60 <= EnemyRenderer->GetCurFrame())
	{
		float4 EnemyPosition = GetTransform()->GetWorldPosition();
		float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();

		if (EnemyPosition.y + 200.0f < PlayerPosition.y)
		{
			ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Miss_Loop);
			return;
		}
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Counter_Loop);
		return;
	}
	},
	.End = [=] {
	DashAttackSetting = false;
	IsRecognize = false;
	}
	});

	// 돌진 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Counter_Loop,
	.Start = [=] {
	IsRecognize = false;
	DashAttackSetting = true;
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_loop");
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		SetMoveStop();
		ChangeState(FSM_State_HellAntenora::HellAntenora_Death_Front);
		return;
	}
	RunTime += _DeltaTime;
	{
		MoveLoop();
		SetForwardMove(500.0f);
	}
	if (true == IsRecognize)
	{

		ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Counter_Attack);
		return;
	}
	{
		float4 EnemyPosition = GetTransform()->GetWorldPosition();
		float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();

		if (EnemyPosition.y + 200.0f < PlayerPosition.y)
		{
			ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Miss_Loop);
			return;
		}
	}
	if (3.0f <= RunTime)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Counter_Stop);
		return;
	}
	},
	.End = [=] {
	IsRecognize = false;
	DashAttackSetting = false;
	RunTime = 0.0f;
	}
		});

	// y축 벗어나면 앞으로만 돌진 루프 후 종료
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Miss_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_loop");
	},
	.Update = [=](float _DeltaTime) {
	DeathCheck();
	if (true == DeathValue)
	{
		SetMoveStop();
		ChangeState(FSM_State_HellAntenora::HellAntenora_Death_Front);
		return;
	}
	RunTime += _DeltaTime;
	{
		SetForwardMove(500.0f);
	}
	if (0.5f <= RunTime)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Counter_Stop);
		return;
	}
	},
	.End = [=] {
	RunTime = 0.0f;
	}
		});

	// 앞으로 휘두르고 뒤로 돔, 38 on, 43 off / 55 프레임 애니메이션 턴
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Counter_Attack,
	.Start = [=] {
	RotationCheck();
	SlerpCalculation();
	SlerpTime = 0.0f;
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	MonsterAttackCollision_Two->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_attack");
	},
	.Update = [=](float _DeltaTime) {

	if (EnemyRenderer->GetCurFrame() < 30)
	{
		RotationCheck();
		SlerpCalculation();
		SlerpTime = 0.0f;
		SlerpTurn(_DeltaTime * 5.0f);
	}
	if (43 > EnemyRenderer->GetCurFrame())
	{
		AllDirectSetting_Normal();
		SetForwardMove(300.0f);
	}
	if (38 <= EnemyRenderer->GetCurFrame() && 43 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollisionOn();
	}
	else
	{
		MonsterAttackCollisionOff();
	}

	if (55 <= EnemyRenderer->GetCurFrame())
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
	MonsterAttackCollisionOff();
	SlerpTime = 0.0f;
	}
		});

	// 돌진 종료
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Counter_Stop,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_stop");
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

	// 오른쪽에서 왼쪽으로 휘두르기 공격 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_Start,
	.Start = [=] 
	{
	SetMoveStop();
	RotationCheck();
	AllDirectSetting();
	EnemyRenderer->ChangeAnimation("em0001_multiattack_Start");
	},
	.Update = [=](float _DeltaTime) 
	{
	{
		SetForwardMove(220.0f);
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_SideAttack_to_Right);
		return;
	}
	},
	.End = [=] 
	{
	}
		});

	// 오른쪽에서 왼쪽으로 크게 휘두름, 22 R on, 27 R Off / 37 L on, 42 L Off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_SideAttack_to_Right,
	.Start = [=]
	{
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);     // L
	MonsterAttackCollision_Two->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE); // R
	RotationCheck();
	AllDirectSetting_Normal();
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0001_multiattack_SideAttack_to_R");
	},
	.Update = [=](float _DeltaTime)
	{
	if (22 > EnemyRenderer->GetCurFrame())
	{
		SlerpTurn(_DeltaTime * 3.0f);
		AllDirectSetting_Normal();
		SetForwardMove(220.0f);
	}
	if (22 <= EnemyRenderer->GetCurFrame() && 27 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision_Two->On();
	}
	else
	{
		MonsterAttackCollision_Two->Off();
	}
	if (37 <= EnemyRenderer->GetCurFrame() && 42 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		if (true == IsRecognize)
		{
			IsRecognize = false;
			++MultiAttackStack;
		
			if (3 > MultiAttackStack)
			{
				ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_SideAttack_To_Left);
			}
			else
			{
				MultiAttackStack = 0;
				ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_To_Attack_End_Right);
			}
		}
		else
		{
			MultiAttackStack = 0;
			ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_UpAttack_To_Left);
		}
		return;
	}
	},
	.End = [=]
	{
	SlerpTime = 0.0f;
	MonsterAttackCollisionOff();
	}
		});

	// 왼쪽에서 오른쪽으로 크게 휘두름, 22 L on, 27 L off / 37 R on, 42 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_SideAttack_To_Left,
	.Start = [=]
	{
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);     // L
	MonsterAttackCollision_Two->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE); // R
	RotationCheck();
	AllDirectSetting_Normal();
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0001_multiattack_SideAttack_to_L");
	},
	.Update = [=](float _DeltaTime)
	{
	if (22 > EnemyRenderer->GetCurFrame())
	{
		SlerpTurn(_DeltaTime * 3.0f);
		AllDirectSetting_Normal();
		SetForwardMove(220.0f);
	}
	if (22 <= EnemyRenderer->GetCurFrame() && 27 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (37 <= EnemyRenderer->GetCurFrame() && 42 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision_Two->On();
	}
	else
	{
		MonsterAttackCollision_Two->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		if (true == IsRecognize)
		{
			IsRecognize = false;
			++MultiAttackStack;

			if (3 > MultiAttackStack)
			{
				ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_SideAttack_to_Right);
			}
			else
			{
				MultiAttackStack = 0;
				ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_To_Attack_End_Left);
			}
		}
		else
		{
			MultiAttackStack = 0;
			ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_UpAttack_To_Right);
		}
		return;
	}
	},
	.End = [=]
	{
	SlerpTime = 0.0f;
	MonsterAttackCollisionOff();
	}
		});

	// 오른쪽에서 왼쪽으로 휘두른 뒤 다시 왼쪽 위로 휘두르는 연계공격, 60 R on, 65 R Off, 76 L On, 81 L Off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_UpAttack_To_Right,
	.Start = [=]
	{
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);     // L
	MonsterAttackCollision_Two->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE); // R
	RotationCheck();
	AllDirectSetting_Normal();
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0001_multiattack_UpAttack_to_R");
	},
	.Update = [=](float _DeltaTime)
	{
	if (60 > EnemyRenderer->GetCurFrame())
	{
		SlerpTurn(_DeltaTime * 3.0f);
		AllDirectSetting_Normal();
		SetForwardMove(220.0f);
	}
	if (60 <= EnemyRenderer->GetCurFrame() && 65 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision_Two->On();
	}
	else
	{
		MonsterAttackCollision_Two->Off();
	}
	if (76 <= EnemyRenderer->GetCurFrame() && 81 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_From_Right_End);
		return;
	}
	},
	.End = [=]
	{
	MonsterAttackCollisionOff();
	}
		});

	// 왼쪽에서 오른쪽으로 휘두른 뒤 다시 오른쪽 위로 휘두르는 연계공격, 61 L on, 66 L Off, 76 R On, 81 R Off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_UpAttack_To_Left,
	.Start = [=]
	{
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);     // L
	MonsterAttackCollision_Two->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE); // R
	RotationCheck();
	AllDirectSetting_Normal();
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em0001_multiattack_UpAttack_to_L");
	},
	.Update = [=](float _DeltaTime)
	{
	if (61 > EnemyRenderer->GetCurFrame())
	{
		SlerpTurn(_DeltaTime * 3.0f);
		AllDirectSetting_Normal();
		SetForwardMove(220.0f);
	}
	if (61 <= EnemyRenderer->GetCurFrame() && 66 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (76 <= EnemyRenderer->GetCurFrame() && 81 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision_Two->On();
	}
	else
	{
		MonsterAttackCollision_Two->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Multiattack_From_Left_End);
		return;
	}
	},
	.End = [=]
	{
	MonsterAttackCollisionOff();
	}
		});

	// 왼쪽에서 오른쪽으로 휘두른 뒤 위에서 찍는 연계공격, 73 on, 78 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_To_Attack_End_Left,
	.Start = [=]
	{
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);     // L
	MonsterAttackCollision_Two->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE); // R
	RotationCheck();
	AllDirectSetting();
	EnemyRenderer->ChangeAnimation("em0001_multiattack_to_attack_End_L");
	},
	.Update = [=](float _DeltaTime)
	{
	if (73 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(220.0f);
	}
	if (73 <= EnemyRenderer->GetCurFrame() && 78 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollisionOn();
	}
	else
	{
		MonsterAttackCollisionOff();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=]
	{
	MonsterAttackCollisionOff();
	}
		});

	// 오른쪽에서 왼쪽으로 휘두른 뒤 위에서 찍는 연계공격, 73 on, 78 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_To_Attack_End_Right,
	.Start = [=]
	{
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);     // L
	MonsterAttackCollision_Two->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE); // R
	RotationCheck();
	AllDirectSetting();
	EnemyRenderer->ChangeAnimation("em0001_multiattack_to_attack_End_R");
	},
	.Update = [=](float _DeltaTime)
	{
	if (73 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(220.0f);
	}
	if (73 <= EnemyRenderer->GetCurFrame() && 78 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollisionOn();
	}
	else
	{
		MonsterAttackCollisionOff();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=]
	{
	MonsterAttackCollisionOff();
	}
		});

	// 오른쪽에서 왼쪽으로 크게 휘두름 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_From_Right_End,
	.Start = [=]
	{
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0001_multiattack_from_R_End");
	},
	.Update = [=](float _DeltaTime)
	{
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=]
	{
	}
		});

	// 왼쪽에서 오른쪽으로 크게 휘두름 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_From_Left_End,
	.Start = [=]
	{
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0001_multiattack_from_L_End");
	},
	.Update = [=](float _DeltaTime)
	{
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=]
	{
	}
	});

	// 왼쪽 뒤로 돌면서 공격, 78 프레임 애니메이션 턴, 84 L on, 89 L Off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Turn_Attack_Left,
	.Start = [=]
	{
	SetMoveStop();
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);     // L
	EnemyRenderer->ChangeAnimation("em0001_turn_attack_L");
	},
	.Update = [=](float _DeltaTime)
	{
	if (78 < EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (84 <= EnemyRenderer->GetCurFrame() && 89 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision->On();
	}
	else
	{
		MonsterAttackCollision->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=]
	{
	MonsterAttackCollisionOff();
	}
		});

	// 오른쪽 뒤로 돌면서 공격, 78 프레임 애니메이션 턴, 85 L on, 90 L Off
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Turn_Attack_Right,
	.Start = [=]
	{
	SetMoveStop();
	MonsterAttackCollision_Two->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);     // R
	EnemyRenderer->ChangeAnimation("em0001_turn_attack_R");
	},
	.Update = [=](float _DeltaTime)
	{
	if (78 < EnemyRenderer->GetCurFrame())
	{
		AnimationTurnStart = true;
	}
	if (85 <= EnemyRenderer->GetCurFrame() && 90 > EnemyRenderer->GetCurFrame())
	{
		MonsterAttackCollision_Two->On();
	}
	else
	{
		MonsterAttackCollision_Two->Off();
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		AnimationTurnStart = false;
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		ChangeState(FSM_State_HellAntenora::HellAntenora_Idle);
		return;
	}
	},
	.End = [=]
	{
	MonsterAttackCollisionOff();
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////      피격      //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////약공격
	// 정면 약공 히트
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_front_01", true);
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
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_back_01", true);
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
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_left_01", true);
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
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_right", true);
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
	EnemyRenderer->ChangeAnimation("em0001_blown_back", true);
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
		ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Counter_Start);
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
	EnemyRenderer->ChangeAnimation("em0001_blown_up", true);
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
		ChangeState(FSM_State_HellAntenora::HellAntenora_Blown_Up_Loop);
		return;
	}
	if (true == FloorCheck(FallDistance) && 0.5f <= FallCheckDelayTime)
	{
		ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Counter_Landing);
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
		ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Counter_Landing);
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
		ChangeState(FSM_State_HellAntenora::HellAntenora_Attack_Counter_Landing);
		return;
	}
	},
	.End = [=] {
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em0001_blown_up_landing", 1, 
			[=] 
			{
				SetPush(37000.0f);
			});
	}

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
		ChangeState(FSM_State_HellAntenora::HellAntenora_Prone_Death);
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
	EnemyRenderer->ChangeAnimation("em0001_slam_damage");
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
	EnemyRenderer->ChangeAnimation("em0001_snatch");
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
	Sound.PlayVoice(9);
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
	Sound.PlayVoice(9);
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
	Sound.PlayVoice(9);
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
	//PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
	EnemyRenderer->ChangeAnimation("em0001_air_damage_gun");
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
		ChangeState(FSM_State_HellAntenora::HellAntenora_Buster_Loop);
		return;
	}
	},
	.End = [=] {
	BusterEnd();
	}
		});
	// 버스트 히트 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Buster_Loop,
	.Start = [=] {
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
	MinusEnemyHP(500);
	Sound.Play("Enemy_Damage_", 12);
	Sound.PlayVoice(5);
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


	EnemyFSM.ChangeState(FSM_State_HellAntenora::HellAntenora_Appear_01);
}

void Enemy_HellAntenora::EnemyCreateFSM_Client()
{
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Appear_01,
	.Start = [=] {
	MonsterEffectRenderer->PlayFX("Enemy_Appear.effect");
	EnemyRenderer->ChangeAnimation("em0001_appear_01");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Idle,
	.Start = [=] {
	SetMoveStop();
	EnemyRenderer->ChangeAnimation("em0001_Idle_01_loop");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Walk_Start,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_walk_start");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Walk_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_walk_loop");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Walk_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_walk_stop");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Navi_Turn_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_turn_left_90_noTransRot");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Navi_Turn_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_turn_right_90_noTransRot");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Turn_Left_180,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_turn_left_180");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Turn_Right_180,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_turn_right_180");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Counter_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_Start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Counter_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Counter_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Miss_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Counter_Attack,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_attack");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Attack_Counter_Stop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_Attack_counter_stop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_Start,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_multiattack_Start");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_SideAttack_to_Right,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_multiattack_SideAttack_to_R");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_SideAttack_To_Left,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_multiattack_SideAttack_to_L");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_UpAttack_To_Right,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_multiattack_UpAttack_to_R");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_UpAttack_To_Left,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_multiattack_UpAttack_to_L");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_To_Attack_End_Left,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_multiattack_to_attack_End_L");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_To_Attack_End_Right,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_multiattack_to_attack_End_R");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_From_Right_End,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_multiattack_from_R_End");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Multiattack_From_Left_End,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_multiattack_from_L_End");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Turn_Attack_Left,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_turn_attack_L");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Turn_Attack_Right,
	.Start = [=]
	{
	EnemyRenderer->ChangeAnimation("em0001_turn_attack_R");
	},
	.Update = [=](float _DeltaTime)
	{
	},
	.End = [=]
	{
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_front_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_back_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_left_01", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Standing_Damage_Weak_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_standing_damage_weak_right", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Blown_Back,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_blown_back", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Blown_Back_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_blown_back_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Blown_Up,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_blown_up", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Air_Damage_Under,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_air_damage_gun", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Blown_Up_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_blown_up_loop", true);
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Blown_Up_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_blown_up_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Slam_Damage,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_slam_damage");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Slam_Damage_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_slam_damage_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Slam_Damage_Landing,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Snatch,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_snatch");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Stun,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_stun");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Stun_Revive,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_stun_revive");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Prone_Getup,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em0001_prone_getup");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Prone_Death,
	.Start = [=] {
	DeathSetting_Client();
	Sound.PlayVoice(9);
	EnemyRenderer->ChangeAnimation("em0001_prone_death");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Death_Back,
	.Start = [=] {
	DeathSetting_Client();
	Sound.PlayVoice(9);
	EnemyRenderer->ChangeAnimation("em0001_death_back");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Death_Front,
	.Start = [=] {
	DeathSetting_Client();
	Sound.PlayVoice(9);
	EnemyRenderer->ChangeAnimation("em0001_death_front");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Buster_Start,
	.Start = [=] {
	BusterCalculation_Client(float4{ 0.0f, -120.0f, 0.0f });
	EnemyRenderer->ChangeAnimation("em0001_air_damage_gun");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	BusterEnd_Client();
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Buster_Loop,
	.Start = [=] {
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_HellAntenora::HellAntenora_Buster_Finish,
	.Start = [=] {
	Sound.Play("Enemy_Damage_", 12);
	EnemyRenderer->ChangeAnimation("em0001_slam_damage_landing");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {

	}
		});

	EnemyFSM.ChangeState(FSM_State_HellAntenora::HellAntenora_Appear_01);
}
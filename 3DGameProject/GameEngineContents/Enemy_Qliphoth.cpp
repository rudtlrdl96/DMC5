#include "PrecompileHeader.h"
#include "Enemy_Qliphoth.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "AttackCollision.h"
#include "BasePlayerActor.h"
#include "AnimationEvent.h"
#include "FXSystem.h"
#include "NetworkManager.h"
Enemy_Qliphoth::Enemy_Qliphoth()
{
}

Enemy_Qliphoth::~Enemy_Qliphoth()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////   Actor Init   ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Enemy_Qliphoth::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::HellCaina;
	
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		EnemyMaxHP = 1000;
	}
	else
	{
		EnemyMaxHP = 2000;
	}

	EnemyHP = EnemyMaxHP;
}

void Enemy_Qliphoth::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("Qliphoth.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Character", "Enemy", "Qliphoth", "mesh"
			}
		);
		GameEngineFBXMesh::Load(Path + "\\Qliphoth.FBX");
		GameEngineFBXMesh::Load(Path + "\\Qliphoth_Stone.FBX");
	}

	StoneRenderer = CreateComponent<GameEngineFBXRenderer>();
	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("Qliphoth.fbx", "AniFBX_Low");
		StoneRenderer->SetFBXMesh("Qliphoth_Stone.fbx", "FBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("Qliphoth.fbx", "AniFBX");
		StoneRenderer->SetFBXMesh("Qliphoth_Stone.fbx", "FBX");
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
	//EnemyRenderer->GetRenderUnit(0, 2)->Off();
}

void Enemy_Qliphoth::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("Qliphoth");
	NewDir.Move("Animation");

	AnimationEvent::LoadAll
	(
		{
			.Dir = NewDir.GetFullPath().c_str(),
			.Renderer = EnemyRenderer,
			.RendererLocalPos = { 0.0f, -200.0f, 0.0f },
			.Objects = {(GameEngineObject*)MonsterAttackCollision.get()},
			.CallBacks_void =
			{
				std::bind(&SoundController::Play, &Sound, "Qliphoth_", 4)
			},
			.CallBacks_int =
			{
			//std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1),
			//std::bind(&SoundController::Play, &Sound, "HellCaina_SFX_",std::placeholders::_1),
			//std::bind(&SoundController::PlayVoice, &Sound, std::placeholders::_1, false),
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

		NewDir.MoveParent();
		NewDir.Move("Qliphoth");
		if (nullptr == FXData::Find("Qliphoth_Appear.effect"))
		{
			FXData::Load(NewDir.GetPlusFileName("Qliphoth_Appear.effect").GetFullPath());
		}
		EffectRenderer->CreateFX(FXData::Find("Qliphoth_Appear.effect"));
	}

	// 사운드 로드
	Sound.SetVoiceName("Qliphoth_");
	if (nullptr == GameEngineSound::Find("Qliphoth_0.wav")) {
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("SFX");
		NewDir.Move("Qliphoth");
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
		}

		if (nullptr == GameEngineSound::Find("Enemy_Damage_0.wav"))
		{
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
}


void Enemy_Qliphoth::Start()
{
	BaseEnemyActor::Start();

	// 랜더러 크기 설정
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -200.0f, 0.0f });
	StoneRenderer->GetTransform()->AddLocalPosition({ 0.0f, -75.0f, 0.0f });
	StoneRenderer->GetTransform()->SetWorldRotation(float4::ZERO);
	// 콜리전 옵션, 크기 설정
	MonsterAttackCollision->SetAttackData(DamageType::Light, MONSTER_LIGHT_DAMAGE);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);
	MonsterCollision->GetTransform()->SetLocalScale({ 200, 200, 200 });
	MonsterCollision->SetColType(ColType::SPHERE3D);

	MonsterCollision_0 = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	MonsterCollision_0->GetTransform()->SetLocalScale({ 200, 200, 200 });
	MonsterCollision_0->SetColType(ColType::SPHERE3D);
	EnemyRenderer->SetAttachTransform("Head02_S", MonsterCollision_0->GetTransform(), float4::ZERO, float4::ZERO, false);

	MonsterCollision_1 = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	MonsterCollision_1->GetTransform()->SetLocalScale({ 200, 200, 200 });
	MonsterCollision_1->SetColType(ColType::SPHERE3D);
	EnemyRenderer->SetAttachTransform("Vine05_end", MonsterCollision_1->GetTransform(), float4::ZERO, float4::ZERO, false);

	MonsterCollision_2 = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	MonsterCollision_2->GetTransform()->SetLocalScale({ 200, 200, 200 });
	MonsterCollision_2->SetColType(ColType::SPHERE3D);
	EnemyRenderer->SetAttachTransform("Vine03_end", MonsterCollision_2->GetTransform(), float4::ZERO, float4::ZERO, false);

	MonsterCollision_3 = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	MonsterCollision_3->GetTransform()->SetLocalScale({ 200, 200, 200 });
	MonsterCollision_3->SetColType(ColType::SPHERE3D);
	EnemyRenderer->SetAttachTransform("Vine01_end", MonsterCollision_3->GetTransform(), float4::ZERO, float4::ZERO, false);

	RN_MonsterCollision->GetTransform()->SetLocalScale({ 2000, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 200, 0 });

	// 기본 세팅
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	//MonsterCollision->Off();
	//RN_MonsterCollision->Off();
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	MonsterAttackCollision->Off();

	// 넷 오브젝트 타입 설정
	SetNetObjectType(Net_ActorType::Qliphoth);

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
			DamageData Datas = _Player->GetAttackCollision()->GetDamage();
			MinusEnemyHP(Datas.DamageValue);
			PlayDamageEvent(Datas.DamageTypeValue, Datas.SoundType);

			if (DamageType::Stun == Datas.DamageTypeValue)
			{
				StopTime(2.8f);
				AttackDelayCheck = 1.0f;
			}

			StartRenderShaking(8);
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
		PlayDamageEvent(Datas.DamageTypeValue, Datas.SoundType);

		if (DamageType::Stun == Datas.DamageTypeValue)
		{
			StopTime(2.9f);
			AttackDelayCheck = 1.0f;
		}

		StartRenderShaking(8);
		HitStop(Datas.DamageTypeValue);

		if (EnemyHP < 0)
		{
			DeathValue = true;
		}

		IsChangeState = true;
		});
}

////////////////////////////////////////////////////////////////////
/////////////////////////// Collision ///////////////////////////////
////////////////////////////////////////////////////////////////////

std::shared_ptr<class GameEngineCollision> Enemy_Qliphoth::ColCheck()
{
	std::shared_ptr<GameEngineCollision> Col = MonsterCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr != Col) { return Col; }
	Col = MonsterCollision_0->Collision(CollisionOrder::PlayerAttack);
	if (nullptr != Col) { return Col; }
	Col = MonsterCollision_1->Collision(CollisionOrder::PlayerAttack);
	if (nullptr != Col) { return Col; }
	Col = MonsterCollision_2->Collision(CollisionOrder::PlayerAttack);
	if (nullptr != Col) { return Col; }
	Col = MonsterCollision_3->Collision(CollisionOrder::PlayerAttack);
	return Col;
}

void Enemy_Qliphoth::DamageCollisionCheck(float _DeltaTime)
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

	std::shared_ptr<GameEngineCollision> Col = ColCheck();
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	PlayerAttackCheck(AttackCol.get());
	MonsterAttackCollision->Off();

	DamageData Data = AttackCol->GetDamage();
	MinusEnemyHP(Data.DamageValue);
	AttackDelayCheck = 0.0f;
	PlayDamageEvent(Data.DamageTypeValue, Data.SoundType);

	//Sound.PlayVoice(3, false);

	switch (Data.DamageTypeValue)
	{
	case DamageType::None:
	case DamageType::Snatch:
	case DamageType::Buster:
		return;
	case DamageType::Light:
	case DamageType::Heavy:
	case DamageType::Air:
	case DamageType::Slam:
		ChangeState(FSM_State_Qliphoth::Qliphoth_Damage);
		StartRenderShaking(8);
		AttackDelayCheck = 0.0f;
		HitStop(Data.DamageTypeValue);
		break;
	case DamageType::Stun:
		StopTime(2.9f);
		return;
	default:
		break;
	}

}

void Enemy_Qliphoth::DamageCollisionCheck_Client(float _DeltaTime)
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

	std::shared_ptr<GameEngineCollision> Col = ColCheck();
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	PlayerAttackCheck(AttackCol.get());
	MonsterAttackCollision->Off();

	DamageData Data = AttackCol->GetDamage();
	AttackDelayCheck = 0.0f;
	PlayDamageEvent(Data.DamageTypeValue, Data.SoundType);

	//Sound.PlayVoice(3, false);

	switch (Data.DamageTypeValue)
	{
	case DamageType::None:
	case DamageType::Snatch:
	case DamageType::Buster:
	case DamageType::Stun:
		return;
	case DamageType::Light:
	case DamageType::Heavy:
	case DamageType::Air:
	case DamageType::Slam:
		ChangeState_Client(FSM_State_Qliphoth::Qliphoth_Damage);
		StartRenderShaking(8);
		break;
	default:
		break;
	}

}


void Enemy_Qliphoth::RecognizeCollisionCheck(float _DeltaTime)
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

void Enemy_Qliphoth::DeathCheck()
{
	if (EnemyHP <= 0)
	{
		DeathValue = true;
		RedOrbDrop();
		DeathEvent();
	}

	if (true == DeathValue && false == DeathSettig)
	{
		DeathSettig = true;
		MonsterCollision->Off();
		RN_MonsterCollision->Off();
	}
}

////////////////////////////////////////////////////////////////
////////////////////////    회전	////////////////////////////
////////////////////////////////////////////////////////////////

void Enemy_Qliphoth::SlerpTurn(float _DeltaTime)
{
	SlerpTime += _DeltaTime;
	RotationValue = float4::SLerpQuaternion(CurRotation, GoalRotation, SlerpTime);
	GetTransform()->SetWorldRotation(RotationValue);
}

////////////////////////////////////////////////////////////////
////////////////////////    FSM		////////////////////////////
////////////////////////////////////////////////////////////////

void Enemy_Qliphoth::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
	IsChangeState = true;
}

void Enemy_Qliphoth::ChangeState_Client(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, Player);
}

void Enemy_Qliphoth::EnemyCreateFSM()
{
	// Appear
	EnemyRenderer->SetAnimationStartEvent("em1000_Appear", 1, [=] {
		Sound.Play("Qliphoth_", 0);
		Sound.Play("Qliphoth_", 5);
		});
	// Appear
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Appear,
	.Start = [=] {
			EffectRenderer->PlayFX("Qliphoth_Appear.effect");
			//EffectRenderer->PlayFX("Enemy_Appear.effect");
			EnemyRenderer->ChangeAnimation("em1000_Appear");
		},
		.Update = [=](float _DeltaTime) {
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Qliphoth::Qliphoth_Idle);
				return;
			}
		},
		.End = [=] {

		}
		});

	// Idle
	static float RotCheckDelay = 0.0f;
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Idle,
	.Start = [=] {
			EnemyRenderer->ChangeAnimation("em1000_Idle");
			SlerpCalculation();
			SlerpTime = 0;
			RotCheckDelay = 0;
			IsRecognize = nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
		},
		.Update = [=](float _DeltaTime) {
			if (false == IsRecognize) { return; }

			RotCheckDelay += _DeltaTime;
			if (0.1f < RotCheckDelay)
			{
				RotationCheck();
				if (EnemyRotationValue == EnemyRotation::Forward)
				{
					ChangeState(FSM_State_Qliphoth::Qliphoth_Attack);
					return;
				}
				SlerpCalculation();
				SlerpTime = 0;
				RotCheckDelay = 0;
			}
			SlerpTurn(_DeltaTime * 1.5f);
		},
	.End = [=] {

	}
		});

	// Attack
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Attack,
	.Start = [=] {
			//EffectRenderer->PlayFX("Enemy_Appear.effect");
			EnemyRenderer->ChangeAnimation("em1000_Attack");
		},
		.Update = [=](float _DeltaTime) {
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Qliphoth::Qliphoth_Idle);
				return;
			}

			RotationCheck();
			SlerpCalculation();
			SlerpTime = 0.0f;
			SlerpTurn(_DeltaTime * 10.0f);
		},
		.End = [=] {

		}
		});

	// Damage
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Damage,
	.Start = [=] {
			Sound.PlayRandom("Qliphoth_", 1, 2);
			EnemyRenderer->ChangeAnimation("em1000_Damage", true);
		},
		.Update = [=](float _DeltaTime) {
			DeathCheck();
			if (true == DeathValue)
			{
				ChangeState(FSM_State_Qliphoth::Qliphoth_Dead);
				return;
			}
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				ChangeState(FSM_State_Qliphoth::Qliphoth_Idle);
				return;
			}
		},
		.End = [=] {

		}
		});

	// Dead
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Dead,
	.Start = [=] {
			Sound.Play("Qliphoth_", 3);
			//EffectRenderer->PlayFX("Enemy_Appear.effect");
			EnemyRenderer->ChangeAnimation("em1000_Dead");
			GameEngineTransform* ActorTran = GetLevel()->CreateActor<GameEngineActor>()->GetTransform();
			ActorTran->SetWorldPosition(GetTransform()->GetWorldPosition());
			StoneRenderer->GetTransform()->SetParent(ActorTran, false);
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


	EnemyFSM.ChangeState(FSM_State_Qliphoth::Qliphoth_Appear);
}

void Enemy_Qliphoth::EnemyCreateFSM_Client()
{
	// Appear
	EnemyRenderer->SetAnimationStartEvent("em1000_Appear", 1, [=] {
		Sound.Play("Qliphoth_", 0);
		Sound.Play("Qliphoth_", 5);
	});
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Appear,
	.Start = [=] {
			EffectRenderer->PlayFX("Qliphoth_Appear.effect");
			EnemyRenderer->ChangeAnimation("em1000_Appear");
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
		});

	// Idle
	static float RotCheckDelay = 0.0f;
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Idle,
	.Start = [=] {
			EnemyRenderer->ChangeAnimation("em1000_Idle");
		},
		.Update = [=](float _DeltaTime) {
		},
	.End = [=] {

	}
		});

	// Attack
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Attack,
	.Start = [=] {
			EnemyRenderer->ChangeAnimation("em1000_Attack");
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
		});

	// Damage
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Damage,
	.Start = [=] {
			Sound.PlayRandom("Qliphoth_", 1, 2);
			EnemyRenderer->ChangeAnimation("em1000_Damage", true);
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
		});

	// Dead
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Dead,
	.Start = [=] {
			Sound.Play("Qliphoth_", 3);
			EnemyRenderer->ChangeAnimation("em1000_Dead");
			GameEngineTransform* ActorTran = GetLevel()->CreateActor<GameEngineActor>()->GetTransform();
			ActorTran->SetWorldPosition(GetTransform()->GetWorldPosition());
			StoneRenderer->GetTransform()->SetParent(ActorTran, false);
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
		});
	EnemyFSM.ChangeState(FSM_State_Qliphoth::Qliphoth_Appear);
}
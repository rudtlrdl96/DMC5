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
	EnemyMaxHP = 1000;
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
			},
			"Qliphoth.FBX"
		);
		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("Qliphoth.fbx", "AniFBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("Qliphoth.fbx", "AniFBX");
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

	EnemyRenderer->GetRenderUnit(0, 2)->Off();
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
			.RendererLocalPos = { 0.0f, 0.0f, 0.0f },
			.Objects = {(GameEngineObject*)MonsterAttackCollision.get()},
			.CallBacks_void =
			{
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
	}

	// 사운드 로드
	Sound.SetVoiceName("Qliphoth_V_");
	if (nullptr == GameEngineSound::Find("Qliphoth_V_0.wav")) {
		//GameEngineDirectory NewDir;
		//NewDir.MoveParentToDirectory("ContentResources");
		//NewDir.Move("ContentResources");
		//NewDir.Move("Sound");
		//NewDir.Move("Voice");
		//NewDir.Move("Qliphoth");
		//std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });

		//for (size_t i = 0; i < Files.size(); i++)
		//{
		//	GameEngineSound::Load(Files[i].GetFullPath());
		//}

		//NewDir.MoveParent();
		//NewDir.MoveParent();
		//NewDir.Move("SFX");
		//NewDir.Move("Qliphoth");
		//Files = NewDir.GetAllFile({ ".wav" });

		//for (size_t i = 0; i < Files.size(); i++)
		//{
		//	GameEngineSound::Load(Files[i].GetFullPath());
		//}

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
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	//MonsterCollision->Off();
	//RN_MonsterCollision->Off();
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, MONSTER_HEAVY_DAMAGE);
	MonsterAttackCollision->Off();

	// 넷 오브젝트 타입 설정
	SetNetObjectType(Net_ActorType::Qliphoth);

	LinkData_UpdatePacket<bool>(IsBurn);
	LinkData_UpdatePacket<int>(EnemyHP);

	SetDamagedNetCallBack<BasePlayerActor>([this](BasePlayerActor* _Attacker) {
		Player = _Attacker;
		DamageData Datas = _Attacker->GetAttackCollision()->GetDamage();
		MinusEnemyHP(Datas.DamageValue);
		PlayDamageEvent(Datas.DamageTypeValue, Datas.SoundType);
		//Sound.PlayVoice(3, false);

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

////////////////////////////////////////////////////////////////////
/////////////////////////// Collision ///////////////////////////////
////////////////////////////////////////////////////////////////////

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

	std::shared_ptr<GameEngineCollision> Col = MonsterCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	PlayerAttackCheck(AttackCol.get());
	MonsterAttackCollision->Off();

	DamageData Data = AttackCol->GetDamage();
	MinusEnemyHP(Data.DamageValue);
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

	std::shared_ptr<GameEngineCollision> Col = MonsterCollision->Collision(CollisionOrder::PlayerAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	PlayerAttackCheck(AttackCol.get());
	MonsterAttackCollision->Off();

	DamageData Data = AttackCol->GetDamage();
	//MinusEnemyHP(Data.DamageValue);
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
		StartRenderShaking(8);
		AttackDelayCheck = 0.0f;
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
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Appear,
	.Start = [=] {
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
			RotationCheck();
			RotCheckDelay = 0;
		},
		.Update = [=](float _DeltaTime) {
			RotCheckDelay += _DeltaTime;
			if (0.1f < RotCheckDelay)
			{
				RotationCheck();
				if (EnemyRotationValue == EnemyRotation::Forward)
				{
					int Random = GameEngineRandom::MainRandom.RandomInt(0, 1);
					if (Random == 0)
					{
						ChangeState(FSM_State_Qliphoth::Qliphoth_Attack);
					}
					else
					{
						ChangeState(FSM_State_Qliphoth::Qliphoth_Attack_Hard);
					}
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
		},
		.End = [=] {

		}
		});
	// Attack Hard
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Attack_Hard,
	.Start = [=] {
			//EffectRenderer->PlayFX("Enemy_Appear.effect");
			EnemyRenderer->ChangeAnimation("em1000_Attack_Hard");
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

	EnemyFSM.ChangeState(FSM_State_Qliphoth::Qliphoth_Appear);
}

void Enemy_Qliphoth::EnemyCreateFSM_Client()
{
	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Appear,
.Start = [=] {
			//EffectRenderer->PlayFX("Enemy_Appear.effect");
			EnemyRenderer->ChangeAnimation("em1000_Appear");
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
		});

	EnemyFSM.CreateState({ .StateValue = FSM_State_Qliphoth::Qliphoth_Idle,
	.Start = [=] {
			EnemyRenderer->ChangeAnimation("em1000_Idle");
		},
		.Update = [=](float _DeltaTime) {
		},
		.End = [=] {

		}
		});

	EnemyFSM.ChangeState(FSM_State_Qliphoth::Qliphoth_Appear);
}
#include "PrecompileHeader.h"
#include "CavaliereAngelo.h"

#include <cmath>

#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineBase/GameEngineRandom.h>

#include "BasePlayerActor.h"
#include "NetworkManager.h"
#include "AnimationEvent.h"
#include "AttackCollision.h"
#include "FXSystem.h"
#include "Cavaliere_Electric.h"
#include "Player_MirageBlade.h"
#include "EffectRenderer.h"

CavaliereAngelo::CavaliereAngelo()
{
}

CavaliereAngelo::~CavaliereAngelo()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////   Actor Init   ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CavaliereAngelo::EnemyTypeLoad()
{
	EnemyCodeValue = EnemyCode::CavaliereAngelo;

	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		EnemyMaxHP = 10000;
	}
	else
	{
		EnemyMaxHP = 20000;
	}

	EnemyHP = EnemyMaxHP;
	HPServerStack = 0;
	HPClientStack = 0;
}

void CavaliereAngelo::EnemyMeshLoad()
{
	if (nullptr == GameEngineFBXMesh::Find("em5501.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Character", "Enemy", "CavaliereAngelo", "mesh"
			},
			"em5501.FBX"
		);
		GameEngineFBXMesh::Load(Path);
	}

	switch (GameEngineOption::GetOption("Shader"))
	{
	case GameEngineOptionValue::Low:
	{
		EnemyRenderer->SetFBXMesh("em5501.fbx", "AniFBX_Low");
	}
	break;
	case GameEngineOptionValue::High:
	{
		EnemyRenderer->SetFBXMesh("em5501.fbx", "AniFBX");
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

	///////////////// 무기 부분 라이트 예제 코드 /////////////////
	WeaponEffectPivot = CreateComponent<GameEngineComponent>();

	std::shared_ptr<GameEngineLight> EffectLight = GetLevel()->CreatePointLight({}, ShadowTextureScale::S_64, 450);
	EffectLight->GetTransform()->SetParent(WeaponEffectPivot->GetTransform());
	EffectLight->GetTransform()->SetLocalPosition({0, -200, 0});

	EffectLight->ShadowOff();
	EffectLight->SetLightPower(5.0f);
	EffectLight->SetLightColor({ 0.5f, 0.2f, 0.7f });
	EffectLight->IsDebugDraw = true;

	EnemyRenderer->SetAttachTransform("R_WeaponHand", WeaponEffectPivot->GetTransform(), { 0, 0, 0}, {90, 0, 0}, true);

	// CircleLight

	if (nullptr == GameEngineTexture::Find("CavaliereAngeloWeaponEffect.tga"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{ "Effect", "Texture" },
			"CavaliereAngeloWeaponEffect.tga");

		GameEngineTexture::Load(Path);
	}	
	
	if (nullptr == GameEngineFBXMesh::Find("wpem5501_00.fbx"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{ "Character", "Enemy", "CavaliereAngelo", "mesh"},
			"wpem5501_00.fbx");

		GameEngineFBXMesh::Load(Path);
	}

	{
		WeaponEffect = CreateComponent<EffectRenderer>();

		WeaponEffect->SetFBXMesh("wpem5501_00.fbx", "Effect_3D");

		WeaponEffect->GetTransform()->SetParent(WeaponEffectPivot->GetTransform());
		WeaponEffect->GetTransform()->SetLocalPosition({ -6.6f, -38.8f, -1.8f });
		WeaponEffect->GetTransform()->SetLocalRotation({ 97.0f, 38.0f, 90.0f });
		WeaponEffect->GetTransform()->SetLocalScale({ 1.3f, 1.3f, 1.05f });

		WeaponEffect->SetTexture("DiffuseTexture", "EngineBaseNormal.png");
		WeaponEffect->EffectOption.MulColor = { 0.5, 0.2f, 0.7f, 0.2f};
		WeaponEffect->SetBloomColor({ 2.0f, 0.8f, 2.8f });

		WeaponEffect->SetDistortionTexture("", {0.1f, 0.1f});
	}

	// 느리구나 쓰러지는 것 조차
	//SetTimeScale(0.0f);
}

void CavaliereAngelo::DrawEditor()
{
	ImGui::Spacing();

	static float AttRot[4] = { 90, 0, 0, 1};

	ImGui::DragFloat4("Attach Rot", AttRot, 1.0f);	

	if (true == ImGui::Button("Weapon Effect Pivot Change"))
	{
		EnemyRenderer->SetDettachTransform();
		EnemyRenderer->SetAttachTransform("R_WeaponHand", WeaponEffectPivot->GetTransform(), { 0, 0, 0 }, { AttRot[0], AttRot[1], AttRot[2] }, true);
	}

	static float EffectRot[4] = { -90, 0, 0, 1};
	ImGui::DragFloat4("Effect Rot", EffectRot, 1.0f);

	if (true == ImGui::Button("Weapon Effect Rot Change"))
	{
		WeaponEffect->GetTransform()->SetLocalRotation({ EffectRot[0], EffectRot[1], EffectRot[2]});
	}
}

void CavaliereAngelo::EnemyAnimationLoad()
{
	//Animation정보 경로를 찾아서 모든animation파일 로드
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Character");
	NewDir.Move("Enemy");
	NewDir.Move("CavaliereAngelo");
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
				std::bind(&SoundController::Play, &Sound, "Cavaliere_SFX_",std::placeholders::_1),
				std::bind(&SoundController::PlayVoice, &Sound, std::placeholders::_1, false),
				std::bind(&SoundController::Play, &Sound, "Cavaliere_Damage_",std::placeholders::_1),
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
		NewDir.Move("CavalierAngelo");
		std::vector<GameEngineFile> FXFiles = NewDir.GetAllFile({ ".Effect" });

		EffectRenderer_0 = CreateComponent<FXSystem>();
		EffectRenderer_1 = CreateComponent<FXSystem>();
		for (size_t i = 0; i < FXFiles.size(); i++)
		{
			if (nullptr == FXData::Find(FXFiles[i].GetFileName()))
			{
				FXData::Load(FXFiles[i].GetFullPath());
			}
			EffectRenderer_0->CreateFX(FXData::Find(FXFiles[i].GetFileName()));
			EffectRenderer_1->CreateFX(FXData::Find(FXFiles[i].GetFileName()));
		}
	}

	// 사운드 로드
	Sound.SetVoiceName("Cavaliere_V_");
	if (nullptr == GameEngineSound::Find("Cavaliere_SFX_0.wav")) {
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("Voice");
		NewDir.Move("Cavaliere");
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
		}

		NewDir.MoveParent();
		NewDir.MoveParent();
		NewDir.Move("SFX");
		NewDir.Move("Cavaliere");
		Files = NewDir.GetAllFile({ ".wav" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSound::Load(Files[i].GetFullPath());
		}
	}
}

void CavaliereAngelo::Start()
{
	BaseEnemyActor::Start();

	//PhysX(충돌)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({ 100, 200, 100 });
	PhysXCapsule->GetDynamic()->setMass(200.0f);

	if (true == NetworkManager::IsClient())
	{
		PhysXCapsule->TurnOffGravity();
	}

	BindPhysicsWithNet(PhysXCapsule);

	// 랜더러 크기 설정
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -45.0f, 0.0f });

	// 콜리전 옵션, 크기 설정
	MonsterAttackCollision->SetAttackData(DamageType::Light, BOSS_LIGHT_DAMAGE);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);
	MonsterCollision->GetTransform()->SetLocalScale({ 150, 300, 150 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 150, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 1000, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	ParryCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Null);
	ParryCollision->SetColType(ColType::OBBBOX3D);
	ParryCollision->GetTransform()->SetLocalScale({ 220, 220, 400 });
	ParryCollision->GetTransform()->SetLocalPosition({ 0, 150, 220 });

	Electric = GetLevel()->CreateActor<Cavaliere_Electric>();
	Electric->GetTransform()->SetParent(GetTransform());
	Electric->Off();

	// 기본 세팅
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	MonsterAttackCollision->SetAttackData(DamageType::Heavy, BOSS_HEAVY_DAMAGE);
	MonsterAttackCollision->Off();

	// 넷 오브젝트 타입 설정
	SetNetObjectType(Net_ActorType::CavaliereAngelo);

	LinkData_UpdatePacket<bool>(IsCharge);
	LinkData_UpdatePacket<bool>(IsBurn);
	LinkData_UpdatePacket<bool>(IsStun);
	LinkData_UpdatePacket<bool>(IsParryCheck);
	LinkData_UpdatePacket<bool>(ParryOkay);
	LinkData_UpdatePacket<int>(ChargeDamageStack);
	LinkData_UpdatePacket<int>(EnemyHP);
	LinkData_UpdatePacket<int>(ParryStack);
	LinkData_UpdatePacket<int>(HPServerStack);
	LinkData_UpdatePacket<int>(HPClientStack);

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

			PlayDamageSound(Datas.SoundType);

			MinusEnemyHP(Datas.DamageValue);
			HPClientStackPlus(Datas.DamageValue);

			if (600 < HPClientStack)
			{
				HPServerStack = 0;
				HPClientStack = 0;
				Player = _Player;
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
		}

		MinusEnemyHP(Datas.DamageValue);
		HPClientStackPlus(Datas.DamageValue);
		PlayDamageSound(Datas.SoundType);

		if (600 < HPClientStack)
		{
			HPServerStack = 0;
			HPClientStack = 0;
			Player = _Attacker;
		}

		if (DamageType::VergilLight == Datas.DamageTypeValue)
		{
			IsVergilLight = true;
		}

		if (DamageType::Buster == Datas.DamageTypeValue && true == IsStun)
		{
			IsStun = false;
			SetTimeScale(0.4f);
			GetLevel()->TimeEvent.AddEvent(.316f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					StartRenderShaking(8);
					if (300 < EnemyHP)
					{
						MinusEnemyHP(300);
						HPClientStackPlus(300);
					}
				});
			GetLevel()->TimeEvent.AddEvent(.683f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					StartRenderShaking(8);
					if (300 < EnemyHP)
					{
						MinusEnemyHP(300);
						HPClientStackPlus(300);
					}
				});
			GetLevel()->TimeEvent.AddEvent(1.13f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					StartRenderShaking(8);
					if (300 < EnemyHP)
					{
						MinusEnemyHP(300);
						HPClientStackPlus(300);
					}
				});
			GetLevel()->TimeEvent.AddEvent(1.4f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					StartRenderShaking(8);
					if (300 < EnemyHP)
					{
						MinusEnemyHP(300);
						HPClientStackPlus(300);
					}
				});
			GetLevel()->TimeEvent.AddEvent(1.6f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					Sound.Play("Cavaliere_Damage_", 3);
					StartRenderShaking(8);
					if (300 < EnemyHP)
					{
						MinusEnemyHP(300);
						HPClientStackPlus(300);
					}
				});
			GetLevel()->TimeEvent.AddEvent(1.7f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					SetTimeScale(0.3f);
				});
			GetLevel()->TimeEvent.AddEvent(2.5f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					Sound.Play("Cavaliere_Damage_", 6);
					StartRenderShaking(8);
					MinusEnemyHP(800);
					HPClientStackPlus(800);
					SetTimeScale(0.0f);
				});
			GetLevel()->TimeEvent.AddEvent(3.5f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					SetTimeScale(1.0f);
				});
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
}

void CavaliereAngelo::Update(float _DeltaTime)
{
	// 싱글, 서버, 클라이언트 상관없이 계속해서 체크되어야 하는 함수들
	{
		_DeltaTime *= GetTimeScale();
		EnemyRenderer->SetTimeScale(GetTimeScale());
		EffectRenderer_0->TimeScale = GetTimeScale();
		EffectRenderer_1->TimeScale = GetTimeScale();

		//if (GetTimeScale() == 0.0f)
		//{
		//	MonsterAttackCollision->Off();
		//	PhysXCapsule->Off();
		//}
		//else if (false == PhysXCapsule->IsUpdate())
		//{
		//	PhysXCapsule->On();
		//}

		RendererBurn(_DeltaTime);
		RenderShake(_DeltaTime);
	}

	// 싱글 플레이일 때 실행
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		DeathCheck();
		ParryCheck();
		RecognizeCollisionCheck(_DeltaTime);
		DamageCollisionCheck(_DeltaTime);
		EnemyFSM.Update(_DeltaTime);

		if (true == GameEngineInput::IsDown("MonsterTest"))
		{
			EnemyHP = 1;
		}
	}
	else
	{
		// 서버 플레이일 때 실행
		if (NetControllType::ActiveControll == GetControllType())
		{
			DeathCheck();
			ParryCheck();
			RecognizeCollisionCheck(_DeltaTime);
			DamageCollisionCheck(_DeltaTime);
			EnemyFSM.Update(_DeltaTime);
		}
		// 클라이언트 플레이일 때 실행
		else
		{
			PhysXCapsule->SetLinearVelocityZero();
			ParryCheck_Client();
			RecognizeCollisionCheck(_DeltaTime);
			Update_NetworkTrans(_DeltaTime);
			DamageCollisionCheck_Client(_DeltaTime);
			EnemyFSM.Update(_DeltaTime);
			MonsterAttackCollision->Off();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// 움직임, 히트 관련 ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CavaliereAngelo::DeathCheck()
{
	if (EnemyHP <= 0)
	{
		DeathValue = true;
		DeathEvent();
	}

	if (true == DeathValue && false == DeathSettig)
	{
		DeathSettig = true;
		MonsterCollision->Off();
		ParryCollision->Off();
		RN_MonsterCollision->Off();
		PhysXCapsule->Off();

		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Daeth_Front);
	}
}

void CavaliereAngelo::PlayerChase()
{
	RotationCheck();

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

	if (EnemyRotation::Right_180 == EnemyRotationValue)
	{
		if (false == IsRecognize)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Right);
		}
		else
		{
			IsRecognize = false;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Right);
		}
	}
	else if (EnemyRotation::Left_180 == EnemyRotationValue)
	{
		if (false == IsRecognize)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Left);
		}
		else
		{
			IsRecognize = false;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Left);
		}
	}
	else
	{
		if (false == IsRecognize)
		{
			if (true == IsPowerUp)
			{
				IsPowerUp = false;
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Start);
			}
			else
			{
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Start);
			}
		}
		else
		{
			IsRecognize = false;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01);
		}
	}

	return;
}

void CavaliereAngelo::AttackCalculation()
{
	AttackDirectCheck();

	if (true == AnimationTurnStart)
	{
		AnimationTurnStart = false;
		RotationCheck();
		PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		EnemyHitDirValue = EnemyHitDirect::Forward;
	}

	PushDirectSetting();
}

void CavaliereAngelo::DamageCollisionCheck(float _DeltaTime)
{
	if (FSM_State_CavaliereAngelo::CavaliereAngelo_Daeth_Front == EnemyFSM.GetCurState())
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
	PlayDamageSound(Data.SoundType);
	if (DamageType::VergilLight == Data.DamageTypeValue)
	{
		IsVergilLight = true;
		Data.DamageTypeValue = DamageType::Light;
	}

	StartRenderShaking(6);
	MinusEnemyHP(Data.DamageValue);
	HPSeverStackPlus(Data.DamageValue);

	if (NetControllType::ActiveControll == GetControllType())
	{
		if (600 < HPServerStack)
		{
			HPServerStack = 0;
			HPClientStack = 0;

			std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
			size_t Playersize = Players.size();

			int PlayerID = Player->GetNetObjectID();

			for (size_t i = 0; i < Playersize; i++)
			{
				int PlayersID = Players[i]->GetNetObjectID();

				if (ServerPlayerID == PlayersID)
				{
					Player = Players[i];
				}
			}
		}
	}

	if (EnemyHP <= 0)
	{
		DeathValue = true;
		return;
	}

	if (true == IsCharge)
	{
		ChargeDamageStack += Data.DamageValue;
	}

	if (ChargeDamageStack >= 650)
	{
		ChargeDamageStack = 0;
		RotationCheck();
		AllDirectSetting();
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Start);
	}

	if (DamageType::Buster == Data.DamageTypeValue && true == IsStun)
	{
		IsStun = false;
		SetTimeScale(0.4f);
		GetLevel()->TimeEvent.AddEvent(.316f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				StartRenderShaking(8);
				if (300 < EnemyHP)
				{
					MinusEnemyHP(300);
					HPSeverStackPlus(300);
				}
			});
		GetLevel()->TimeEvent.AddEvent(.683f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				StartRenderShaking(8);
				if (300 < EnemyHP)
				{
					MinusEnemyHP(300);
					HPSeverStackPlus(300);
				}
			});
		GetLevel()->TimeEvent.AddEvent(1.13f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				StartRenderShaking(8);
				if (300 < EnemyHP)
				{
					MinusEnemyHP(300);
					HPSeverStackPlus(300);
				}
			});
		GetLevel()->TimeEvent.AddEvent(1.4f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				StartRenderShaking(8);
				if (300 < EnemyHP)
				{
					MinusEnemyHP(300);
					HPSeverStackPlus(300);
				}
			});
		GetLevel()->TimeEvent.AddEvent(1.6f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				Sound.Play("Cavaliere_Damage_", 3);
				StartRenderShaking(8);
				if (300 < EnemyHP)
				{
					MinusEnemyHP(300);
					HPSeverStackPlus(300);
				}
			});
		GetLevel()->TimeEvent.AddEvent(1.7f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				SetTimeScale(0.3f);
			});
		GetLevel()->TimeEvent.AddEvent(2.5f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				Sound.Play("Cavaliere_Damage_", 6);
				StartRenderShaking(8);
				MinusEnemyHP(800);
				HPSeverStackPlus(800);
				SetTimeScale(0.0f);
			});
		GetLevel()->TimeEvent.AddEvent(3.5f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				SetTimeScale(1.0f);
			});
	}

	if (DamageType::Stun == Data.DamageTypeValue)
	{
		AttackDelayCheck = 1.0f;
		StopTime(2.9f);
		return;
	}
}

void CavaliereAngelo::DamageCollisionCheck_Client(float _DeltaTime)
{
	if (FSM_State_CavaliereAngelo::CavaliereAngelo_Daeth_Front == EnemyFSM.GetCurState())
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
	PlayDamageSound(Data.SoundType);

	if (DamageType::VergilLight == Data.DamageTypeValue)
	{
		Data.DamageTypeValue = DamageType::Light;
	}

	StartRenderShaking(6);

	NetworkObjectBase* Obj = dynamic_cast<NetworkObjectBase*>(AttackCol->GetActor());

	if (DamageType::Buster != Data.DamageTypeValue && false == IsStun)
	{
		ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
		return;
	}

	if (DamageType::Stun == Data.DamageTypeValue)
	{
		AttackDelayCheck = 1.0f;
		ExcuteNetObjEvent(2, NetObjEventPath::PassiveToActive, { Obj });
		return;
	}
}

void CavaliereAngelo::RecognizeCollisionCheck(float _DeltaTime)
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

void CavaliereAngelo::ParryCheck()
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

void CavaliereAngelo::ParryCheck_Client()
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

		if (ParryStack <= 1)
		{
			ChangeState_Client(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01, Obj);
		}
		else if (ParryStack >= 2 && ParryStack < 5)
		{
			ChangeState_Client(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01, Obj);
		}
		else if (ParryStack >= 5)
		{
			ParryStack = 0;
			ChangeState_Client(FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill, Obj);
		}
	}
}

void CavaliereAngelo::ParryTime()
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

void CavaliereAngelo::BossTurn()
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

void CavaliereAngelo::PlayDamageSound(DamageSoundType _Type)
{
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////     FSM     ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CavaliereAngelo::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

void CavaliereAngelo::ChangeState_Client(int _StateValue, NetworkObjectBase* _Obj)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, _Obj);
}

void CavaliereAngelo::EnemyCreateFSM()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Idle     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 방어자세로 서있음
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Idle,
	.Start = [=] {
	IsRecognize = false;
	EnemyRenderer->ChangeAnimation("em5501_defense-Idle");
	},
	.Update = [=](float _DeltaTime) {

	if (3 <= ColliderStack)
	{
		IsRecognize = false;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider);
		return;
	}

	WaitTime += _DeltaTime;

	if (WaitTime >= 0.2f)
	{
		PlayerChase();
		return;
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Special     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 번개 충전 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Start,
	.Start = [=] {
	EffectRenderer_0->PlayFX("Cavalier_Charge_1.effect");
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_start");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Loop);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 번개 충전 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Loop,
	.Start = [=] {
	EffectRenderer_0->PlayFX("Cavalier_Charge_2.effect");
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_loop");
	IsCharge = true;
	},
	.Update = [=](float _DeltaTime) {

	WaitTime += _DeltaTime;

	if (WaitTime > 6.0f)
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_End);
		return;
	}

	},
	.End = [=] {
	IsCharge = false;
	ChargeDamageStack = 0;
	WaitTime = 0.0f;
	}
		});

	// 번개 충전 끝, 100프레임 on, 105프레임 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_End,
	.Start = [=] {
	IsPowerUp = true;
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, BOSS_HEAVY_DAMAGE);
	EffectRenderer_0->PlayFX("Cavalier_Charge_3.effect");
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_end");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	}
		});

	// 워프 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_Start,
	.Start = [=] {
	EffectRenderer_1->PlayFX("Cavalier_Warp.effect");
	EnemyRenderer->ChangeAnimation("em5501_warp_start");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		if (false == IsInit)
		{
			IsInit = true;
		}
		else
		{
			float PlayerXPos = Player->GetTransform()->GetWorldPosition().x;

			if (PlayerXPos >= -34650.0f)
			{
				PhysXCapsule->SetWorldPosition({ -36500, 1950, -365 });
				PhysXCapsule->SetWorldRotation({ 0.0f, 90.0f, 0.0f });
			}
			else
			{
				PhysXCapsule->SetWorldPosition({ -32200, 1950, -370 });
				PhysXCapsule->SetWorldRotation({ 0.0f, -90.0f, 0.0f });
			}
			BasePlayerActor::CameraLookBoss(PhysXCapsule->GetWorldPosition());
		}

		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Start);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 워프 끝->방어자세로 변경
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_warp_end");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 공격 도중 칼 위로 들어서 빠른 충전
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider,
	.Start = [=] {
	ColliderStack = 0;
	EffectRenderer_0->PlayFX("Cavalier_Colider.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_collider");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		float PlayerXPos = Player->GetTransform()->GetWorldPosition().x;

		if (PlayerXPos >= -34650.0f)
		{
			PhysXCapsule->SetWorldPosition({ -36500, 1950, -365 });
			PhysXCapsule->SetWorldRotation({ 0.0f, 90.0f, 0.0f });
		}
		else
		{
			PhysXCapsule->SetWorldPosition({ -32200, 1950, -370 });
			PhysXCapsule->SetWorldRotation({ 0.0f, -90.0f, 0.0f });
		}
		BasePlayerActor::CameraLookBoss(PhysXCapsule->GetWorldPosition());
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider_To_Dengeki);
		return;
	}
	},
	.End = [=] {
	}
		});

	{
		EnemyRenderer->SetAnimationStartEvent("em5501_Attack_collider_to_Dengeki", 70, [=] {
			RotationCheck();
			AllDirectSetting_Normal();
			SlerpCalculation();
			IsFastCharge = true; });
	}

	// 충전 완료 후 Dengeki 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider_To_Dengeki,
	.Start = [=] {
	IsRecognize = false;
	EffectRenderer_1->PlayFX("Cavalier_Collider_To_Dengeki.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_collider_to_Dengeki");
	},
	.Update = [=](float _DeltaTime) {

	if (70 < EnemyRenderer->GetCurFrame())
	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(180.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsRecognize = false;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop);
		return;
	}
	},
	.End = [=] {
	EffectRenderer_1->Off();
	SlerpTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Move     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 서있다가 방어자세 취하면서 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Attack_To_Guard_Start,
	.Start = [=] {
	IsRecognize = false;
	RotationCheck();
	AllDirectSetting_Normal();
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em5501_A-walk_start_L0");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime * 2.0f);
		AllDirectSetting_Normal();
		SetForwardMove(180.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// 방어자세 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Start,
	.Start = [=] {
	RotationCheck();
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em5501_guard_to_walk");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime * 2.0f);
		AllDirectSetting_Normal();
		SetForwardMove(180.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// 방어자세 걷기 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_loop");
	},
	.Update = [=](float _DeltaTime) {

	{
		BossTurn();
		SetForwardMove(280.0f);
	}

	if (true == IsRecognize)
	{
		IsRecognize = false;
		if (3 <= ColliderStack)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01);
		}
		return;
	}
	},
	.End = [=] {
	}
		});

	// 방어자세 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_End_L0");
	},
	.Update = [=](float _DeltaTime) {

	{
		AllDirectSetting_Normal();
		SetForwardMove(180.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// Dengeki 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Start,
	.Start = [=] {
	RotationCheck();
	AllDirectSetting_Normal();
	SlerpCalculation();
	EffectRenderer_0->PlayFX("Cavalier_Dengeki_Start.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Start");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime * 2.0f);
		AllDirectSetting_Normal();
		SetForwardMove(180.0f);
	}

	if (true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack04);
		return;
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	EffectRenderer_0->Off();
	}
		});

	// Dengeki 걷기 루프
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Light, BOSS_LIGHT_DAMAGE);
	WaitTime = 0.2f;
	EffectRenderer_0->PlayFX("Cavalier_Dengeki_Loop.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Loop");
	},
	.Update = [=](float _DeltaTime) {
	WaitTime -= _DeltaTime;
	if (WaitTime < 0)
	{
		if (nullptr == Player)
		{
			Electric->Shoot(GetTransform()->GetLocalForwardVector());
		}
		else
		{
			Electric->Shoot(Player);
		}
		WaitTime = 1.0f;
	}
	{
		BossTurn();
		SetForwardMove(240.0f);
	}

	if (true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_End);
		return;
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsFastCharge = false;
		ColliderStack = 0;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_End);
		return;
	}
	},
	.End = [=] {
	WaitTime = 0.0f;
	EffectRenderer_0->Off();
	}
		});

	// Dengeki 걷기 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_End,
	.Start = [=] {
	EffectRenderer_0->PlayFX("Cavalier_Dengeki_End.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_End");
	},
	.Update = [=](float _DeltaTime) {

	{
		BossTurn();
		SetForwardMove(200.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
		{
			std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
			bool Iscontact = PlayerContactCheck(_DeltaTime, Collision.get());

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
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop);
		}
		else
		{
			IsRecognize = false;
			if (3 <= ColliderStack)
			{
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider);
			}
			else
			{
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01);
			}
		}
		return;
	}
	},
	.End = [=] {
		EffectRenderer_0->Off();
	}
		});

	// 방어자세 도중 왼쪽으로 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_quick_turn_L");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 방어자세 도중 오른쪽으로 180도 회전
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_quick_turn_R");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Attack     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	{
		// Attack1 -> 2 연계
		EnemyRenderer->SetAnimationStartEvent("em5501_Attack01_R", 68, [=] {
			// ColliderStack이 다 찬 경우에는 25퍼 확률로 Idle로 이행 (그냥 return)
			if (3 <= ColliderStack + 1)
			{
				int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);
				if (0 == RandC)
				{
					return;
				}
			}

			// 근처에 플레이어가 있으면 후속타로 이행
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
					IsRecognize = false;
					PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
					Event01 = false;
					Normal01 = false;
					ParryStack = 0;
					++ColliderStack;
					SetMoveStop();
					ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02);
				}
				return;
			}
			else
			{
				IsRecognize = false;
			}
			});

		// Attack2 -> 3 연계
		EnemyRenderer->SetAnimationStartEvent("em5501_Attack02", 59, [=] {
			if (3 <= ColliderStack + 1)
			{
				int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);
				if (0 == RandC)
				{
					return;
				}
			}
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
					PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
					Event01 = false;
					Normal01 = false;
					SetMoveStop();
					ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03);
				}
				return;
			}
			else
			{
				IsRecognize = false;
			}
			});

		// Attack3 -> 4 연계
		EnemyRenderer->SetAnimationStartEvent("em5501_Attack03", 37, [=] {
			if (3 <= ColliderStack + 1)
			{
				int RandC = GameEngineRandom::MainRandom.RandomInt(0, 3);
				if (0 == RandC)
				{
					return;
				}
			}
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
					PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
					Event01 = false;
					Normal01 = false;
					SetMoveStop();
					ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack04);
				}
				return;
			}
			else
			{
				IsRecognize = false;
			}
			});
	}

	// 01(오중단) 공격, 끝나면 방어자세, 66프레임 on, 69프레임 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, BOSS_HEAVY_DAMAGE);
	RotationCheck();
	SlerpCalculation();
	AllDirectSetting_Normal();
	SlerpCalculation();
	SlerpTime = 0.0f;
	EffectRenderer_0->PlayFX("Cavalier_Attack_1.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack01_R");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();

		if (79 > EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}

		if (129 < EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}

		if (EnemyRenderer->GetCurFrame() < 64)
		{
			if (50 < EnemyRenderer->GetCurFrame())
			{
				RotationCheck();
				SlerpCalculation();
				SlerpTime = 0.0f;
				SlerpTurn(_DeltaTime * 10.0f);
			}
			else
			{
				SlerpTurn(_DeltaTime * 1.1f);
			}
		}
	}

	if (false == ParryOkay)
	{
		if (60 <= EnemyRenderer->GetCurFrame() && 69 > EnemyRenderer->GetCurFrame())
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

		if (ParryStack <= 1)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01);
		}
		else if (ParryStack >= 2 && ParryStack < 5)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01);
		}
		else if (ParryStack >= 5)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill);
		}
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ParryStack = 0;
		++ColliderStack;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	IsParryCheck = false;
	SlerpTime = 0.0f;
	}
		});

	// 02(오상단) 공격, 끝나면 방어자세, 44프레임 on, 47프레임 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, BOSS_HEAVY_DAMAGE);
	RotationCheck();
	SlerpCalculation();
	SlerpTime = 0.0f;
	EffectRenderer_1->PlayFX("Cavalier_Attack_2.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack02");
	if (true == Event01)
	{
		Event01 = false;
		EnemyRenderer->SetCurFrame(38);
		EffectRenderer_1->SetCurFrame(38);
	}
	if (true == Normal01)
	{
		Normal01 = false;
		EnemyRenderer->SetCurFrame(38);
		EffectRenderer_1->SetCurFrame(38);
	}
	},
	.Update = [=](float _DeltaTime) {

	{
		AllDirectSetting_Normal();

		if (57 > EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}

		if (117 < EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}
		if (EnemyRenderer->GetCurFrame() < 40)
		{
			if (30 < EnemyRenderer->GetCurFrame())
			{
				RotationCheck();
				SlerpCalculation();
				SlerpTime = 0.0f;
				SlerpTurn(_DeltaTime * 10.0f);
			}
			else
			{
				SlerpTurn(_DeltaTime * 1.36f);
			}
		}
	}

	if (false == ParryOkay)
	{
		if (38 <= EnemyRenderer->GetCurFrame() && 47 > EnemyRenderer->GetCurFrame())
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

		if (ParryStack <= 1)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01);
		}
		else if (ParryStack >= 2 && ParryStack < 5)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01);
		}
		else if (ParryStack >= 5)
		{
			ParryStack = 0;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill);
		}
		return;
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	IsParryCheck = false;
	}
		});

	// 03(왼중단) 공격, 끝나면 방어자세, 29프레임 on, 33프레임 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, BOSS_HEAVY_DAMAGE);
	RotationCheck();
	SlerpCalculation();
	SlerpTime = 0.0f;
	EffectRenderer_0->PlayFX("Cavalier_Attack_3.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack03");
	if (true == Event01)
	{
		Event01 = false;
		EnemyRenderer->SetCurFrame(23);
		EffectRenderer_0->SetCurFrame(23);
	}
	if (true == Normal01)
	{
		Normal01 = false;
		EnemyRenderer->SetCurFrame(23);
		EffectRenderer_0->SetCurFrame(23);
	}
	},
	.Update = [=](float _DeltaTime) {

	{
		AllDirectSetting_Normal();

		if (43 > EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}

		if (113 < EnemyRenderer->GetCurFrame())
		{
			SetForwardMove(120.0f);
		}

		if (EnemyRenderer->GetCurFrame() < 28)
		{
			if (20 < EnemyRenderer->GetCurFrame())
			{
				RotationCheck();
				SlerpCalculation();
				SlerpTime = 0.0f;
				SlerpTurn(_DeltaTime * 10.0f);
			}
			else
			{
				SlerpTurn(_DeltaTime * 2.0f);
			}
		}
	}

	if (false == ParryOkay)
	{
		if (23 <= EnemyRenderer->GetCurFrame() && 33 > EnemyRenderer->GetCurFrame())
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

		if (ParryStack <= 1)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01);
		}
		else if (ParryStack >= 2 && ParryStack < 5)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01);
		}
		else if (ParryStack >= 5)
		{
			ParryStack = 0;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill);
		}
		return;
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	IsParryCheck = false;
	}
		});

	// 플레이어 패리 성공 후 바로 공격
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01,
	.Start = [=] {
	ParryOkay = false;
	IsParryCheck = false;
	++ParryStack;
	ParryTime();
	EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		float4 Direction = MonsterAndPlayerCross();
		RotationCheck();
		AllDirectSetting_Normal();
		SlerpCalculation();
		if (Direction.y > 0)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_03);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_02);
		}
		return;
	}
	},
	.End = [=] {
	}
		});

	// 플레이어 패리 후 성공 후 약경직
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01,
	.Start = [=] {
	ParryOkay = false;
	IsParryCheck = false;
	++ParryStack;
	ParryTime();
	EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01");
		},
	.Update = [=](float _DeltaTime) {

	{
		AllDirectSetting_Normal();

		if (35 > EnemyRenderer->GetCurFrame())
		{
			SetBackMove(100.f);
		}
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		float4 Direction = MonsterAndPlayerCross();
		RotationCheck();
		AllDirectSetting_Normal();
		SlerpCalculation();
		SetMoveStop();
		if (Direction.y > 0)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_03);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_02);
		}
		return;
	}
	},
	.End = [=] {
	}
		});

	// 01 패리 후 바로 02 시작 전, 37프레임 스타트 (38 프레임으로 변경)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01_to_02");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Event01 = true;
		SetMoveStop();
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// 01 패리 후 바로 03 시작 전, 24 프레임 스타트 (23 프레임으로 변경)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01_to_04");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Event01 = true;
		SetMoveStop();
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// 01 패리 후 경직, 이후 02 시작 전, 37프레임 스타트 (38 프레임으로 변경)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_02");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Normal01 = true;
		SetMoveStop();
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// 01 패리 후 경직, 이후 03 시작 전, 24 프레임 스타트 (23 프레임으로 변경)
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_03");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		Normal01 = true;
		SetMoveStop();
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	// 04(양손으로 상단찍기) 공격, 끝나면 방어자세, 90프레임 on, 94프레임 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack04,
	.Start = [=] {
	MonsterAttackCollision->SetAttackData(DamageType::Heavy, BOSS_HEAVY_DAMAGE);
	RotationCheck();
	SlerpCalculation();
	SlerpTime = 0.0f;
	SetMoveStop();
	EffectRenderer_1->PlayFX("Cavalier_Attack_4.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack04");
	},
	.Update = [=](float _DeltaTime) {

	if (EnemyRenderer->GetCurFrame() < 90)
	{
		RotationCheck();
		SlerpCalculation();
		SlerpTime = 0.0f;
		SlerpTurn(_DeltaTime * 5.0f);
	}
	if (140 < EnemyRenderer->GetCurFrame())
	{
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (false == ParryOkay)
	{
		if (85 <= EnemyRenderer->GetCurFrame() && 94 > EnemyRenderer->GetCurFrame())
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

		if (ParryStack <= 1)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01);
		}
		else if (ParryStack >= 2 && ParryStack < 5)
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01);
		}
		else if (ParryStack >= 5)
		{
			ParryStack = 0;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill);
		}
		return;
	}
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		if (true == IsFastCharge)
		{
			IsFastCharge = false;
			ColliderStack = 0;
		}

		if (2 <= ColliderStack)
		{
			ColliderStack = 0;
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider);
		}
		else
		{
			ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		}
		return;
	}
	},
	.End = [=] {
	MonsterAttackCollision->Off();
	SlerpTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Damage     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 정면공격(드릴) 맞고 스턴 (스턴이 조금 더 김), 이후 자세 회복
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill,
	.Start = [=] {
	IsRecognize = false;
	Event01 = false;
	Normal01 = false;
	IsPowerUp = false;
	IsFastCharge = false;
	IsParryCheck = false;
	ParryOkay = false;
	IsStun = true;
	ColliderStack = 0;
	ParryStack = 0;
	ParryTime();
	RotationCheck();
	AllDirectSetting_Normal();
	EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em5501_Damage_Drill");
	},
	.Update = [=](float _DeltaTime) {

	if (85 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(160.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_Start);
		return;
	}
	},
	.End = [=] {
	IsStun = false;
	}
		});

	// 데미지 중첩 스턴 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Start,
	.Start = [=] {
	IsRecognize = false;
	Event01 = false;
	Normal01 = false;
	IsPowerUp = false;
	IsFastCharge = false;
	IsParryCheck = false;
	ParryOkay = false;
	IsStun = true;
	ParryStack = 0;
	ColliderStack = 0;
	ParryTime();
	RotationCheck();
	AllDirectSetting_Normal();
	EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em5501_stun_start");
	},
	.Update = [=](float _DeltaTime) {

	if (50 < EnemyRenderer->GetCurFrame() && 70 > EnemyRenderer->GetCurFrame())
	{
		SetBackMove(120.0f);
	}

	if (100 < EnemyRenderer->GetCurFrame() && 120 > EnemyRenderer->GetCurFrame())
	{
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Hukki);
		return;
	}
	},
	.End = [=] {
	IsStun = false;
	}
		});

	// 데미지 중첩 스턴 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Hukki,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_stun_hukki");
	IsSturn = false;
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
		return;
	}
	},
	.End = [=] {
	}
		});

	// 으앙쥬금
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Daeth_Front,
	.Start = [=] {
	EffectRenderer_0->Off();
	EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em5501_daeth_front");
	},
	.Update = [=](float _DeltaTime) {

	if (6 < EnemyRenderer->GetCurFrame() && 62 > EnemyRenderer->GetCurFrame())
	{
		float4 CurPos = EnemyRenderer->GetTransform()->GetWorldPosition();
		float4 BackMove = EnemyRenderer->GetTransform()->GetWorldBackVector();
		float4 PlusPos = BackMove * _DeltaTime * 200.0f;
		float4 NewPos = CurPos + PlusPos;
		EnemyRenderer->GetTransform()->SetWorldPosition(NewPos);
	}

	if (150 < EnemyRenderer->GetCurFrame() && 235 > EnemyRenderer->GetCurFrame())
	{
		float4 CurPos = EnemyRenderer->GetTransform()->GetWorldPosition();
		float4 BackMove = EnemyRenderer->GetTransform()->GetWorldBackVector();
		float4 PlusPos = BackMove * _DeltaTime * 150.0f;
		float4 NewPos = CurPos + PlusPos;
		EnemyRenderer->GetTransform()->SetWorldPosition(NewPos);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsBurn = true;
	}
	},
	.End = [=] {
	}
		});

	EnemyFSM.ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_Start);
}

void CavaliereAngelo::EnemyCreateFSM_Client()
{
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Idle,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_defense-Idle");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Start,
	.Start = [=] {
	BasePlayerActor::CameraLookBoss(PhysXCapsule->GetWorldPosition());
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_end");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_warp_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_warp_end");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_collider");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Collider_To_Dengeki,
	.Start = [=] {
	BasePlayerActor::CameraLookBoss(PhysXCapsule->GetWorldPosition());
	EnemyRenderer->ChangeAnimation("em5501_Attack_collider_to_Dengeki");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Attack_To_Guard_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_start_L0");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_to_walk");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Guard_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_A-walk_End_L0");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Loop");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_End,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_End");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Left,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_quick_turn_L");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Guard_Quick_Turn_Right,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_guard_quick_turn_R");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack01_R");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack03");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01");
		},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01_to_02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_Even01_To_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_even01_to_04");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_02,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_02");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Parry_normal01_to_03,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Parry_normal01_to_03");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack04,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Attack04");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_Damage_Drill");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_stun_start");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Stun_Hukki,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_stun_hukki");
	IsSturn = false;
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Daeth_Front,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_daeth_front");
	},
	.Update = [=](float _DeltaTime) {
	},
	.End = [=] {
	}
		});

	EnemyFSM.ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_Start);
}
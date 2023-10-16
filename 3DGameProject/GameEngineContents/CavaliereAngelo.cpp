#include "PrecompileHeader.h"
#include "CavaliereAngelo.h"

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
	EnemyCodeValue = EnemyCode::HellCaina;
	EnemyHP = 0;
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

	// PaperBurn, z값 변경하면 됨
	//if (nullptr == GameEngineTexture::Find("PaperBurnNoise.jpg"))
	//{
	//	GameEngineTexture::Load(GameEnginePath::GetFileFullPath("ContentResources",
	//		{
	//			"Texture", "BurnNoiseTexture"
	//		}, "PaperBurnNoise.jpg"));
	//}

	//EnemyRenderer->SetTexture("PaperBurnTexture", "PaperBurnNoise.jpg");

	//EnemyRenderer->GetTransform()->SetLocalScale({ 0.8f , 0.8f , 0.8f });
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
				std::bind(&GameEngineFSM::ChangeState, &EnemyFSM, std::placeholders::_1)
			},
			.CallBacks_float4 =
			{
			}
		}
	);

	// 이펙트 시스템 생성
	{
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
			EffectRenderer_0->CreateFX(FXData::Load(FXFiles[i].GetFullPath()));
			EffectRenderer_1->CreateFX(FXData::Find(FXFiles[i].GetFileName()));
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
	BindPhysicsWithNet(PhysXCapsule);

	// 랜더러 크기 설정
	EnemyRenderer->GetTransform()->AddLocalPosition({ 0.0f, -45.0f, 0.0f });

	// 콜리전 옵션, 크기 설정
	MonsterAttackCollision->SetAttackData(DamageType::Light, 0);
	MonsterAttackCollision->SetColType(ColType::OBBBOX3D);
	MonsterCollision->GetTransform()->SetLocalScale({ 80, 180, 70 });
	MonsterCollision->GetTransform()->SetLocalPosition({ 0, 50, 0 });
	MonsterCollision->SetColType(ColType::OBBBOX3D);
	RN_MonsterCollision->GetTransform()->SetLocalScale({ 1000, 0, 0 });
	RN_MonsterCollision->GetTransform()->SetLocalPosition({ 0, 80, 0 });

	ParryCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Null);
	ParryCollision->SetColType(ColType::OBBBOX3D);
	ParryCollision->GetTransform()->SetLocalScale({ 220, 220, 280 });
	ParryCollision->GetTransform()->SetLocalPosition({ 0, 150, 100 });

	// 기본 세팅
	FallDistance = 55.0f;
	AttackDelayCheck = (1.0f / 60.0f) * 5.0f;

	//EnemyRenderer->Off();
	//MonsterCollision->Off();
	//RN_MonsterCollision->Off();

	// 넷 오브젝트 타입 설정
	SetNetObjectType(Net_ActorType::HellCaina);
}

void CavaliereAngelo::Update(float _DeltaTime)
{
	// 싱글, 서버, 클라이언트 상관없이 계속해서 체크되어야 하는 함수들
	{
		_DeltaTime *= GetTimeScale();
		EnemyRenderer->SetTimeScale(GetTimeScale());
		EffectRenderer_0->TimeScale = GetTimeScale();
		EffectRenderer_1->TimeScale = GetTimeScale();

		if (GetTimeScale() == 0.0f)
		{
			MonsterAttackCollision->Off();
			PhysXCapsule->Off();
		}
		else if (false == PhysXCapsule->IsUpdate())
		{
			PhysXCapsule->On();
		}

		RenderShake(_DeltaTime);
		//MonsterSnatch(_DeltaTime);
		ParryCheck();
	}

	// 싱글 플레이일 때 실행
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		RecognizeCollisionCheck(_DeltaTime);
		DamageCollisionCheck(_DeltaTime);
		EnemyFSM.Update(_DeltaTime);
	}
	else
	{
		// 서버 플레이일 때 실행
		if (NetControllType::ActiveControll == GetControllType())
		{
			RecognizeCollisionCheck(_DeltaTime);
			DamageCollisionCheck(_DeltaTime);
			EnemyFSM.Update(_DeltaTime);
		}
		// 클라이언트 플레이일 때 실행
		else
		{
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

void CavaliereAngelo::PlayerChase(float _DeltaTime)
{
	RotationCheck();

	if (nullptr != RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D))
	{
		IsRecognize = true;
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

void CavaliereAngelo::PlayerAttack(float _DeltaTime)
{
	RotationCheck();
	AllDirectSetting();

	if (EnemyRotation::Left == EnemyRotationValue)
	{
		//ChangeState(FSM_State_HellCaina::HellCaina_Walk_Right_Start);
	}
	else if (EnemyRotation::Right == EnemyRotationValue)
	{
		//ChangeState(FSM_State_HellCaina::HellCaina_Walk_Left_Start);
	}
	else if (EnemyRotation::Left_180 == EnemyRotationValue || EnemyRotation::Right_180 == EnemyRotationValue)
	{
		PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		//ChangeState(FSM_State_HellCaina::HellCaina_Attack_Turn);
	}
	else
	{
		//RandomAttack();
	}
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

	if (DamageType::VergilLight == Data.DamageTypeValue)
	{
		IsVergilLight = true;
		Data.DamageTypeValue = DamageType::Light;
	}

	StartRenderShaking(6);

	switch (Data.DamageTypeValue)
	{
	case DamageType::None:
		return;
		break;
	case DamageType::Light:

		if (true == IsHeavyAttack || true == IsSlamAttack || true == IsBusterAttack)
		{
			return;
		}

		if (true == IsAirAttack)
		{
			StartRenderShaking(8);
			//ChangeState(FSM_State_HellCaina::HellCaina_Air_Damage_Under);
			return;
		}

		if (true == IsCollapse)
		{
			StartRenderShaking(8);
			return;
		}

		AttackCalculation();

		switch (EnemyHitDirValue)
		{
		case EnemyHitDirect::Forward:
			//ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Front);
			break;
		case EnemyHitDirect::Back:
			//ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Back);
			break;
		case EnemyHitDirect::Left:
			//ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Left);
			break;
		case EnemyHitDirect::Right:
			//ChangeState(FSM_State_HellCaina::HellCaina_Standing_Damage_Weak_Right);
			break;
		default:
			break;
		}
		break;

	case DamageType::Heavy:
		//IsCollapse = false;
		//IsHeavyAttack = true;
		//RotationCheck();
		//PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		//ChangeState(FSM_State_HellCaina::HellCaina_Blown_Back);
		break;
	case DamageType::Air:
		//IsCollapse = false;
		//IsAirAttack = true;
		//RotationCheck();
		//PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		//ChangeState(FSM_State_HellCaina::HellCaina_Blown_Up);
		break;
	case DamageType::Snatch:
		//IsCollapse = false;
		//IsAirAttack = true;
		//StartMonsterSnatch();
		//RotationCheck();
		//PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		//ChangeState(FSM_State_HellCaina::HellCaina_Snatch);
		break;
	case DamageType::Slam:
		//IsCollapse = false;
		//IsSlamAttack = true;
		//ChangeState(FSM_State_HellCaina::HellCaina_Slam_Damage);
		break;
	case DamageType::Buster:
		//IsCollapse = false;
		//IsBusterAttack = true;
		//BusterCalculation();
		//RotationCheck();
		//PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
		//PhysXCapsule->AddWorldRotation({ 0.f, 180.f, 0.f });
		//ChangeState(FSM_State_HellCaina::HellCaina_Buster_Start);
		break;
	case DamageType::Stun:
		break;
	default:
		break;
	}

	IsVergilLight = false;
	AttackDelayCheck = 0.0f;
}

void CavaliereAngelo::DamageCollisionCheck_Client(float _DeltaTime)
{

}

void CavaliereAngelo::RecognizeCollisionCheck(float _DeltaTime)
{
	if (true == IsRecognize || false == RN_MonsterCollision->IsUpdate())
	{
		return;
	}

	std::shared_ptr<GameEngineCollision> Collision = RN_MonsterCollision->Collision(CollisionOrder::Player, ColType::SPHERE3D, ColType::SPHERE3D);
	if (nullptr == Collision) { return; }

	PlayerContactCheck(_DeltaTime, Collision.get());
	IsRecognize = true;
}

void CavaliereAngelo::ParryCheck()
{
	if (false == IsParryCheck || true == ParryOkay)
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
	RotationCheck();

	if (-4.0f <= DotProductValue && 4.0f >= DotProductValue)
	{
		AllDirectSetting_Normal();
		return;
	}

	float4 CurRot = GetTransform()->GetWorldRotation();

	if (CurRot.y <= 0.0f)
	{
		CurRot.y += 360.f;
	}

	float4 Value = float4{ 0.0f, DotProductValue, 0.0f };

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

void CavaliereAngelo::ChangeState(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue);
}

void CavaliereAngelo::ChangeState_Client(int _StateValue)
{
	EnemyFSM.ChangeState(_StateValue);
	EnemyFSMValue = _StateValue;
	NetworkManager::SendFsmChangePacket(this, _StateValue, Player);
}

void CavaliereAngelo::EnemyCreateFSM()
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Idle     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 방어자세로 서있음
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Idle,
	.Start = [=] {
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
	if (WaitTime >= 0.6f)
	{
		PlayerChase(_DeltaTime);
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
	EffectRenderer_0->PlayFX("Cavalier_Charge_1.effect");
	EnemyRenderer->ChangeAnimation("em5501_dengeki_reload_loop");
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
	WaitTime = 0.0f;
	}
	});

	// 번개 충전 끝
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Dengeki_Reload_End,
	.Start = [=] {
	IsPowerUp = true;
	EffectRenderer_0->PlayFX("Cavalier_Charge_2.effect");
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
	}
	});

	// 워프 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Warp_Start,
	.Start = [=] {
	EnemyRenderer->ChangeAnimation("em5501_warp_start");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		PhysXCapsule->SetWorldPosition({ 0, 100, 0 });
		PhysXCapsule->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
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
	EnemyRenderer->ChangeAnimation("em5501_Attack_collider");
	},
	.Update = [=](float _DeltaTime) {
	if (true == EnemyRenderer->IsAnimationEnd())
	{
		PhysXCapsule->SetWorldPosition({ 0, 100, 0 });
		PhysXCapsule->SetWorldRotation({ 0.0f, 180.0f, 0.0f });
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
	EffectRenderer_0->PlayFX("Cavalier_Collider_To_Dengeki.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_collider_to_Dengeki");
	},
	.Update = [=](float _DeltaTime) {

	if (70 < EnemyRenderer->GetCurFrame())
	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
	}

	if (true == EnemyRenderer->IsAnimationEnd())
	{
		IsRecognize = false;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack_Dengeki_Loop);
		return;
	}
	},
	.End = [=] {
	SlerpTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Move     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 서있다가 방어자세 취하면서 걷기 시작
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Wark_Attack_To_Guard_Start,
	.Start = [=] {
	RotationCheck();
	AllDirectSetting_Normal();
	SlerpCalculation();
	EnemyRenderer->ChangeAnimation("em5501_A-walk_start_L0");
	},
	.Update = [=](float _DeltaTime) {

	{
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
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
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
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
		SetForwardMove(200.0f);
	}

	if (false == IsRecognize)
	{
		//ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01);
		//return;
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
		SetForwardMove(120.0f);
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
		SlerpTurn(_DeltaTime);
		AllDirectSetting_Normal();
		SetForwardMove(120.0f);
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
	EffectRenderer_0->PlayFX("Cavalier_Dengeki_Loop.effect");
	EnemyRenderer->ChangeAnimation("em5501_Attack_Dengeki_Loop");
	},
	.Update = [=](float _DeltaTime) {

	{
		BossTurn();
		SetForwardMove(200.0f);
	}

	if (true == IsRecognize)
	{
		IsRecognize = false;
		ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack04);
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
			IsRecognize = true;
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
				PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
				Event01 = false;
				Normal01 = false;
				ParryStack = 0;
				++ColliderStack;
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack02);
				return;
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
				PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
				Event01 = false;
				Normal01 = false;
				ParryStack = 0;
				++ColliderStack;
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack03);
				return;
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
				PhysXCapsule->AddWorldRotation({ 0.f, DotProductValue, 0.f });
				Event01 = false;
				Normal01 = false;
				ParryStack = 0;
				++ColliderStack;
				ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Attack04);
				return;
			}
			});
	}

	// 01(오중단) 공격, 끝나면 방어자세, 66프레임 on, 69프레임 off
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Attack01,
	.Start = [=] {
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
		ParryOkay = false;
		IsParryCheck = false;
		++ParryStack;

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
		ParryOkay = false;
		IsParryCheck = false;
		++ParryStack;

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
		ParryStack = 0;
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
		ParryOkay = false;
		IsParryCheck = false;
		++ParryStack;

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
		ParryStack = 0;
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
		ParryOkay = false;
		IsParryCheck = false;
		++ParryStack;

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
	SlerpTime = 0.0f;
	}
		});

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////     Damage     //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	// 정면공격(드릴) 맞고 스턴 (스턴이 조금 더 김), 이후 자세 회복
	EnemyFSM.CreateState({ .StateValue = FSM_State_CavaliereAngelo::CavaliereAngelo_Damage_Drill,
	.Start = [=] {
	ParryTime();
	RotationCheck();
	AllDirectSetting_Normal();
	EffectRenderer_0->PlayFX("Cavalier_Parry.effect");
	EffectRenderer_1->Off();
	EnemyRenderer->ChangeAnimation("em5501_Damage_Drill");
	ColliderStack = 0;
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
	ParryStack = 0;
	}
		});

	EnemyFSM.ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
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

	EnemyFSM.ChangeState(FSM_State_CavaliereAngelo::CavaliereAngelo_Idle);
}
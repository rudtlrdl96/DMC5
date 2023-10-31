#include "PrecompileHeader.h"
#include "BasePlayerActor.h"
#include <algorithm>
#include <GameEngineBase/GameEngineNet.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include "PlayerController.h"
#include "PlayerCamera.h"
#include "ServerWindow.h"
#include "ObjectUpdatePacket.h"
#include "ContentsEnum.h"
#include "BaseLog.h"
#include "AnimationEvent.h"
#include "NetworkManager.h"
#include "FsmChangePacket.h"
#include "AttackCollision.h"
#include "FXSystem.h"
#include "Item_RedOrb.h"
#include "EffectRenderer.h"
#include "PlayerHPUI.h"
#include "FadeEffect.h"
#include "BaseShopUI.h"
#include "BWColorEffect.h"
#include "TestStageLevel.h"
std::vector<BasePlayerActor*> BasePlayerActor::Players = std::vector<BasePlayerActor*>();
BasePlayerActor* BasePlayerActor::MainPlayer = nullptr;

void BasePlayerActor::CameraLookBoss(const float4& _Pos)
{
	for (BasePlayerActor* _Player : Players)
	{
		if (nullptr != _Player->Camera)
		{
			_Player->Camera->SetLookTarget(_Pos);
		}
	}
}

BasePlayerActor::BasePlayerActor()
{
	Players.push_back(this);
}

BasePlayerActor::~BasePlayerActor()
{
}

void BasePlayerActor::SetCutScene(const float4& _StartPos, const float4& _EndPos, const float4& _StartRot, const float4& _EndRot, float _Time, bool ControllOff /* = true*/, bool _UIOff /* = true*/)
{
	Controller->ResetKey();
	Controller->Off();
	FadeEffect::GetFadeEffect()->FadeOut(0.5f);
	GetLevel()->TimeEvent.AddEvent(0.6f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			if (true == _UIOff)
			{
				UIOff();
			}
			FadeEffect::GetFadeEffect()->FadeIn(0.5f);
			Camera->SetCameraCutscene(_StartPos, _EndPos, _StartRot, _EndRot, _Time);
		});
	GetLevel()->TimeEvent.AddEvent(_Time, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			FadeEffect::GetFadeEffect()->FadeOut(0.5f);
		});
	GetLevel()->TimeEvent.AddEvent(_Time + 0.6f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			if (true == _UIOff)
			{
				UIOn();
			}
			FadeEffect::GetFadeEffect()->FadeIn(0.5f);
		});
	if (true == ControllOff)
	{
		GetLevel()->TimeEvent.AddEvent(_Time + 1.2f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				Controller->On();
			});
	}
}

void BasePlayerActor::On()
{
	GameEngineActor::On();

	std::vector<BasePlayerActor*>::iterator FindIter = std::find(Players.begin(), Players.end(), this);
	if (Players.end() != FindIter)
		return;

	Players.push_back(this);
}

void BasePlayerActor::Off()
{
	GameEngineActor::Off();

	std::vector<BasePlayerActor*>::iterator FindIter = std::find(Players.begin(), Players.end(), this);
	if (Players.end() == FindIter)
		return;

	Players.erase(FindIter);
}

void BasePlayerActor::LookDir(const float4& _LookDir)
{
	float4 LocalForward = GetTransform()->GetWorldForwardVector().NormalizeReturn();
	float4 LookDir = _LookDir.NormalizeReturn();
	float Dot = float4::DotProduct3D(LocalForward, LookDir);
	if (1.0f < Dot || LocalForward == LookDir)
	{
		return;
	}
	if (Dot < -1.0f)
	{
		Rot.y += 180;
		PhysXCapsule->SetWorldRotation(Rot);
		return;
	}
	float Angle = acosf(Dot) * GameEngineMath::RadToDeg;
	float4 Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);

	if (std::isnan(Angle))
	{
		return;
	}

	if (Cross.y < 0.0f)
	{
		Rot.y -= Angle;
	}
	else
	{
		Rot.y += Angle;
	}
	PhysXCapsule->SetWorldRotation(Rot);
	return;
}

void BasePlayerActor::LookTarget()
{
	if (nullptr == LockOnEnemyTransform || LockOnEnemyTransform->GetWorldPosition() == GetTransform()->GetWorldPosition())
	{
		return;
	}

	float4 LocalForward = GetTransform()->GetWorldLeftVector().NormalizeReturn();
	float4 LookDir = LockOnEnemyTransform->GetWorldPosition() - GetTransform()->GetWorldPosition();
	LookDir.y = 0;
	LookDir.Normalize();
	float4 Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);
	float Dot = float4::DotProduct3D(LocalForward, LookDir);
	if (std::isnan(Dot))
	{
		return;
	}
	if (Cross.y <= 0.0f)
	{
		Rot.y += 180;
		PhysXCapsule->SetWorldRotation(Rot);
	}
	else
	{
		Rot.y += -GameEngineMath::RadToDeg * Dot;
		PhysXCapsule->SetWorldRotation(Rot);
	}
	return;
}

void BasePlayerActor::RotationToDir(const float4& _Dir, float _MaxValue)
{
	float4 LocalForward = GetTransform()->GetWorldForwardVector().NormalizeReturn();
	float4 LookDir = _Dir.NormalizeReturn();
	float Dot = float4::DotProduct3D(LocalForward, LookDir);
	if (1.0f < Dot || LocalForward == LookDir)
	{
		return;
	}
	if (Dot < -1.0f)
	{
		Rot.y += 180;
		PhysXCapsule->SetWorldRotation(Rot);
		return;
	}
	float Angle = acosf(Dot) * GameEngineMath::RadToDeg;
	float4 Cross = float4::Cross3DReturnNormal(LocalForward, LookDir);

	if (std::isnan(Angle))
	{
		return;
	}
	if (Cross.y < 0.0f)
	{
		Rot.y -= std::clamp(Angle, -_MaxValue, _MaxValue);
	}
	else
	{
		Rot.y += std::clamp(Angle, -_MaxValue, _MaxValue);
	}
	PhysXCapsule->SetWorldRotation(Rot);
	return;
}

void BasePlayerActor::RotationToTarget(float _MaxValue /* = 360.0f */)
{
	if (nullptr == LockOnEnemyTransform)
	{
		return;
	}
	float4 Dir = LockOnEnemyTransform->GetWorldPosition() - GetTransform()->GetWorldPosition();
	Dir.y = 0;
	Dir.Normalize();
	RotationToDir(Dir, _MaxValue);
}

void BasePlayerActor::RotationToMoveVector(float _MaxValue)
{
	const float4& MoveVector = Controller->GetMoveVector();
	if (MoveVector == float4::ZERO)
	{
		return;
	}
	RotationToDir(MoveVector, _MaxValue);
}

void BasePlayerActor::SetInvincibility(float _Time)
{
	InvincibilityTime = _Time;
}

void BasePlayerActor::SetBossCam(GameEngineTransform* _BossCam)
{
	Camera->SetBossCam(_BossCam);
	IsBossBattle = true;
}

void BasePlayerActor::CameraLookTarget(const float4& _TargetPos)
{
	Camera->SetLookTarget(_TargetPos);
}

void BasePlayerActor::Start()
{
	// PhysX 콜리전
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0.1f, 0.1f, 0.0f);
	PhysXCapsule->IsMainPlayerCapsule();
	PhysXCapsule->CreatePhysXActors({ 75, 75, 150 });
	PhysXCapsule->GetDynamic()->setMass(5.0f);
	PhysXCapsule->GetDynamic()->setLinearDamping(2.0f);
	BindPhysicsWithNet(PhysXCapsule);

	// 플레이어 충돌체
	Col_Player = CreateComponent<GameEngineCollision>(CollisionOrder::Player);
	Col_Player->GetTransform()->SetLocalPosition({ 0, 20, 0 });
	Col_Player->GetTransform()->SetLocalScale({ 100, 200, 100 });
	Col_Player->SetColType(ColType::AABBBOX3D);

	Col_Attack = CreateComponent<AttackCollision>(CollisionOrder::PlayerAttack);
	Col_Attack->Off();
	Col_Attack->SetColType(ColType::OBBBOX3D);
	Col_Attack->SetHitStopCallBack(std::bind(&BasePlayerActor::StopTime, this, std::placeholders::_1));
	Col_Attack->ParryAttackOn();

	LinkData_UpdatePacket<bool>(DTValue, [this](bool _BeforeData)
		{
			SetDT(DTValue);
		});

	SetFsmPacketCallBack(std::bind(&BasePlayerActor::SetFSMStateValue, this, std::placeholders::_1));

	Renderer = CreateComponent<GameEngineFBXRenderer>();
	EffectSystem = CreateComponent<FXSystem>();
	DTOffEffect = CreateComponent<FXSystem>();
	DTOffEffect->IsEndOff = true;
}

void BasePlayerActor::UIOn()
{
	HUD->On();
}

void BasePlayerActor::UIOff()
{
	HUD->Off();
}

void BasePlayerActor::NetControllLoad()
{
	PhysXCapsule->TurnOffGravity();
}

void BasePlayerActor::UserControllLoad()
{
	MainPlayer = this;
	// 유저 컨트롤 엑터인 경우 실행
	PhysXCapsule->SetMainPlayer();
	CustomCallback::SetMainPlayer(PhysXCapsule.get());

	Camera = GetLevel()->CreateActor<PlayerCamera>();
	Camera->SetPlayerTranform(GetTransform());

	// 플레이어 컨트롤러 (조작 체계)
	Controller = CreateComponent<PlayerController>();
	Controller->SetCameraTransform(Camera->GetTransform());
	Controller->CallBack_LockOnDown = std::bind(&BasePlayerActor::LockOn, this);
	Controller->CallBack_LockOnUp = std::bind(&BasePlayerActor::LockOff, this);

	Col_EnemyStepCheck = CreateComponent<GameEngineCollision>(CollisionOrder::Null);
	Col_EnemyStepCheck->GetTransform()->SetLocalPosition({ 0, -20, 0 });
	Col_EnemyStepCheck->GetTransform()->SetLocalScale({ 200, 175, 200 });
	Col_EnemyStepCheck->SetColType(ColType::AABBBOX3D);
	// 록온 용 충돌체
	Col_LockOn = CreateComponent<GameEngineCollision>(CollisionOrder::PlayerLockOn);
	Col_LockOn->GetTransform()->SetParent(Camera->GetCameraTarget());
	Col_LockOn->GetTransform()->SetLocalScale({ 2000, 5000, 5000 });
	Col_LockOn->GetTransform()->SetLocalPosition({ 0, 0, 2500 });
	Col_LockOn->SetColType(ColType::OBBBOX3D);
	Col_LockOn->Off();

	Col_Attack->SetIsPlayerCollision();
	Col_Attack->SetDamageCallBack([=](float _Value)
		{
			AddDTGauge(_Value);
		});

	LockOnRenderer = CreateComponent<EffectRenderer>();
	LockOnRenderer->GetTransform()->SetParent(GetLevel()->GetMainCamera()->GetTransform());
	LockOnRenderer->GetTransform()->SetLocalRotation({ 0, 0, 90 });
	LockOnRenderer->GetTransform()->SetWorldScale({ 0.4f, 0.4f, 0.4f });
	LockOnRenderer->SetFBXMesh("Effect_LockOn.fbx", "Effect_2D");
	LockOnRenderer->SetTexture("DiffuseTexture", "Effect_Texture_09.tga");
	LockOnRenderer->Off();
	//LockOnRenderer->LockRotation();
}


void BasePlayerActor::Update(float _DeltaTime)
{
	_DeltaTime *= GetTimeScale();
	Renderer->SetTimeScale(GetTimeScale());
	EffectSystem->TimeScale = GetTimeScale();
	TimeEvent.Update(_DeltaTime);
	Update_Character(_DeltaTime);
	Update_NetworkTrans(_DeltaTime);

	if (NetControllType::PassiveControll == GameEngineNetObject::GetControllType())
	{

	}
	else if (NetControllType::ActiveControll == GameEngineNetObject::GetControllType())
	{
		if (true == DTValue)
		{
			AddDTGauge(-0.5f * _DeltaTime);
		}
		if (true == IsShopOn && true == GameEngineInput::IsUp("UI_ESC"))
		{
			TestStageLevel::Inst->GetBWEffect()->BWColorEffectOff();
			Shop->ShopOff();
			ShopOff();
			IsShopOn = false;
			Controller->On();
			Camera->On();
		}

		if (IsDeath == true) { return; }

		if (nullptr != LockOnEnemy)
		{
			if (LockOnEnemy->GetHP() <= 0 || true == LockOnEnemy->IsDeath())
			{
				LockOff();
			}
			else
			{
				SetLockOnMark();
			}
		}
		InvincibilityTime -= _DeltaTime;
		DamageColCheck();
		OrbColCheck();
		ShopColCheck();
	}
}


void BasePlayerActor::LockOn()
{
	std::vector<std::shared_ptr<GameEngineCollision>> Cols;
	BaseEnemyActor* NearEnemy = nullptr;
	Col_LockOn->On();
	if (true == Col_LockOn->CollisionAll(CollisionOrder::Enemy, Cols))
	{
		float Min = 9999;
		for (std::shared_ptr<GameEngineCollision> Col : Cols)
		{
			BaseEnemyActor* Enemy = dynamic_cast<BaseEnemyActor*>(Col->GetActor());
			if (nullptr == Enemy) { continue; }
			if (Enemy->GetHP() <= 0) { continue; }
			// 체력이 0이하인 적은 제외

			float Length = (GetTransform()->GetWorldPosition() - Col->GetTransform()->GetWorldPosition()).Size();
			if (Length < Min)
			{
				Min = Length;
				NearEnemy = Enemy;
			}
		}
		if (NearEnemy == nullptr)
		{
			Col_LockOn->Off();
			return;
		}
		LockOnEnemy = NearEnemy;
		LockOnEnemyTransform = LockOnEnemy->GetTransform();
		Camera->SetTargetTranform(LockOnEnemyTransform);
		LockOnRenderer->On();
		SetLockOnMark();
	}
	Col_LockOn->Off();
}

void BasePlayerActor::LockOff()
{
	LockOnEnemy = nullptr;
	LockOnEnemyTransform = nullptr;
	Camera->SetTargetTranform(nullptr);
	LockOnRenderer->Off();
}

bool BasePlayerActor::FloorCheck()
{
	float4 Point;

	return GetLevel()->RayCast(GetTransform()->GetWorldPosition(), float4::DOWN, Point, 100.0f)
		|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldForwardVector() * 50), float4::DOWN, Point, 100.0f)
		|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldLeftVector() * 50), float4::DOWN, Point, 100.0f)
		|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldRightVector() * 50), float4::DOWN, Point, 100.0f)
		|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldBackVector() * 50), float4::DOWN, Point, 100.0f);
}

void BasePlayerActor::SetFloorPos()
{
	float4 Point;
	if (false == GetLevel()->RayCast(GetTransform()->GetWorldPosition(), float4::DOWN, Point, 120.0f)) { return; }

	Point += float4::UP * 75;
	SetWorldPosition(Point);
}

void BasePlayerActor::StopTime(float _Time)
{
	SetTimeScale(0.0f);
	GetLevel()->TimeEvent.AddEvent(_Time, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			Col_Attack->Off();
			SetTimeScale(1.0f);
		});
}

void BasePlayerActor::DamageColCheck()
{
	if (HP <= 0)
	{
		return;
	}
	std::shared_ptr<GameEngineCollision> Col = Col_Player->Collision(CollisionOrder::EnemyAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }

	// 무적시간 체크
	if (0 < InvincibilityTime) {
		if (false == IsEvade) { return; }
		// 회피 상황시 스톱타임
		SetTimeScale(0.1f);
		GetLevel()->TimeEvent.AddEvent(0.1f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				SetTimeScale(1.0f);
			});
		BaseEnemyActor* Enemy = dynamic_cast<BaseEnemyActor*>(Col->GetActor());
		if (nullptr != Enemy)
		{
			Enemy->SetTimeScale(0.1f);
			GetLevel()->TimeEvent.AddEvent(0.1f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
				{
					Enemy->SetTimeScale(1.0f);
				});
		}
		RankUI::GetRankInst()->AddRankScore(30);
		AddDTGauge(0.5f);
		IsEvade = false;
		return;
	}

	Sound.Play("Player_Damage");
	DamageData Data = AttackCol->GetDamage();
	if (DTValue == true)
	{
		Data.DamageValue /= 2;
	}
	HP -= std::max<int>(0, Data.DamageValue);
	RankUI::GetRankInst()->RankDisApper();
	switch (Data.DamageTypeValue)
	{
	case DamageType::None:
		break;
	case DamageType::Light:
		LightDamage();
		break;
	case DamageType::Heavy:
		HeavyDamage();
		break;
	case DamageType::Air:
		break;
	case DamageType::Snatch:
		break;
	case DamageType::Slam:
		break;
	case DamageType::Buster:
		break;
	case DamageType::Stun:
		break;
	default:
		break;
	}

	if (HP <= 0)
	{
		GetLevel()->TimeEvent.AddEvent(3.0f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				FadeEffect::GetFadeEffect()->FadeOut();
				_Manager->AddEvent(1.0f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
					{
						GameEngineCore::ChangeLevel("TitleLevel");
					});
			});
	}
}

void BasePlayerActor::OrbColCheck()
{
	std::shared_ptr<GameEngineCollision> Col = Col_Player->Collision(CollisionOrder::RedOrb);
	if (nullptr == Col) { return; }
	Item_RedOrb* Orb = dynamic_cast<Item_RedOrb*>(Col->GetActor());
	if (nullptr == Orb) { return; }
	Orb->Take(GetTransform());
	// 오브 획득
}

void BasePlayerActor::ShopColCheck()
{
	if (true == IsShopOn) { return; }
	if (nullptr == Col_Player->Collision(CollisionOrder::Shop))
	{
		Shop->ApperCusterWindow(false);
		return;
	}
	Shop->ApperCusterWindow(true);

	if (GameEngineInput::IsUp("Enter"))
	{
		TestStageLevel::Inst->GetBWEffect()->BWColorEffectOn();
		RankUI::GetRankInst()->RankOff();
		Shop->ApperCusterWindow(false);
		ShopOn();
		Controller->Off();
		//Camera->Off();
		Controller->ResetKey();
		IsShopOn = true;
		TimeEvent.AddEvent(0.02f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
			{
				Shop->ShopOn();
			});
	}
}

void BasePlayerActor::SetLockOnMark()
{
	// 위치 지정
	float4 Pos = LockOnEnemyTransform->GetWorldPosition();
	Pos.y += 60.0f;
	Pos += LockOnRenderer->GetTransform()->GetWorldBackVector() * 50.0f;
	LockOnRenderer->GetTransform()->SetWorldPosition(Pos);

	//LockOnEnemy
	LockOnRenderer->EffectOption.UVX = LockOnEnemy->GetHPRatio() - 1.0f;
}

void BasePlayerActor::SetForce(float4 _Value)
{
	float4 re = _Value.RotationYDegReturn(Rot.y);
	PhysXCapsule->SetForce(re);
}

void BasePlayerActor::SetPush(float4 _Value)
{
	PhysXCapsule->SetPush(_Value.RotationYDegReturn(Rot.y));
}

void BasePlayerActor::SetMove(float4 _Value)
{
	PhysXCapsule->SetMove(_Value.RotationYDegReturn(Rot.y));
}

void BasePlayerActor::SetWorldPosition(float4 _Value)
{
	PhysXCapsule->SetWorldPosition(_Value);
	GetTransform()->SetWorldPosition(_Value);
}

void BasePlayerActor::SetWorldRotation(float4 _Value)
{
	PhysXCapsule->SetWorldRotation(_Value);
	GetTransform()->SetWorldRotation(_Value);
	Rot.y = _Value.y;

	if (nullptr != Camera)
	{
		Camera->SetRotation(_Value);
	}
}
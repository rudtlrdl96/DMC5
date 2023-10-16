#include "PrecompileHeader.h"
#include "BasePlayerActor.h"
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
std::vector<BasePlayerActor*> BasePlayerActor::Players = std::vector<BasePlayerActor*>();

BasePlayerActor::BasePlayerActor()
{
	Players.push_back(this);
}

BasePlayerActor::~BasePlayerActor()
{
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
}

void BasePlayerActor::Start()
{
	// PhysX 콜리전
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0.1f, 0.1f, 0.0f);
	PhysXCapsule->IsMainPlayerCapsule();
	PhysXCapsule->CreatePhysXActors({ 100, 100, 150 });
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
	Col_Attack->SetIsPlayerCollision();
	Col_Attack->ParryAttackOn();

	LinkData_UpdatePacket<bool>(DTValue, [this](bool _BeforeData)
	{
		SetDT(DTValue);
	});

	SetFsmPacketCallBack(std::bind(&BasePlayerActor::SetFSMStateValue, this, std::placeholders::_1));

	Renderer = CreateComponent<GameEngineFBXRenderer>();
	EffectSystem = CreateComponent<FXSystem>();
}

void BasePlayerActor::NetControllLoad()
{
	PhysXCapsule->TurnOffGravity();
}

void BasePlayerActor::UserControllLoad()
{
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
		InvincibilityTime -= _DeltaTime;
		DamageColCheck();
		OrbColCheck();
	}
}


void BasePlayerActor::LockOn()
{
	std::vector<std::shared_ptr<GameEngineCollision>> Cols;
	std::shared_ptr<GameEngineCollision> MinCol = nullptr;
	Col_LockOn->On();
	if (true == Col_LockOn->CollisionAll(CollisionOrder::Enemy, Cols))
	{
		float Min = 9999;
		for (std::shared_ptr<GameEngineCollision> Col : Cols)
		{
			float Length = (GetTransform()->GetWorldPosition() - Col->GetTransform()->GetWorldPosition()).Size();
			if (Length < Min)
			{
				Min = Length;
				MinCol = Col;
			}
		}
		if (MinCol == nullptr)
		{
			Col_LockOn->Off();
			return;
		}
		LockOnEnemyTransform = MinCol->GetActor()->GetTransform();
		Camera->SetTargetTranform(LockOnEnemyTransform);
	}
	Col_LockOn->Off();
}

void BasePlayerActor::LockOff()
{
	LockOnEnemyTransform = nullptr;
	Camera->SetTargetTranform(nullptr);
}

bool BasePlayerActor::FloorCheck()
{
	float4 Point;
	
	return GetLevel()->RayCast(GetTransform()->GetWorldPosition(), float4::DOWN, Point, 100.0f)
		|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldForwardVector() * 75), float4::DOWN, Point, 100.0f)
		|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldLeftVector() * 75), float4::DOWN, Point, 100.0f)
		|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldRightVector() * 75), float4::DOWN, Point, 100.0f)
		|| GetLevel()->RayCast(GetTransform()->GetWorldPosition() + (GetTransform()->GetWorldBackVector() * 75), float4::DOWN, Point, 100.0f);
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
	// 무적시간 체크
	if (0 < InvincibilityTime) { return; }
	std::shared_ptr<GameEngineCollision> Col = Col_Player->Collision(CollisionOrder::EnemyAttack);
	if (nullptr == Col) { return; }

	std::shared_ptr<AttackCollision> AttackCol = std::dynamic_pointer_cast<AttackCollision>(Col);
	if (nullptr == AttackCol) { return; }
	DamageData Data = AttackCol->GetDamage();
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

}

void BasePlayerActor::OrbColCheck()
{
	std::shared_ptr<GameEngineCollision> Col = Col_Player->Collision(CollisionOrder::RedOrb);
	if (nullptr == Col) { return; }
	Item_RedOrb* Orb = dynamic_cast<Item_RedOrb*>(Col->GetActor());
	if (nullptr == Orb) { return; }
	Orb->Take();
	// 오브 획득
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
}
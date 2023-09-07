#include "PrecompileHeader.h"
#include "BasePlayerActor.h"
#include <GameEngineBase/GameEngineNet.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "PlayerController.h"
#include "PlayerCamera.h"
#include "ServerWindow.h"
#include "ObjectUpdatePacket.h"
#include "ContentsEnum.h"
#include "BaseLog.h"
#include "AnimationEvent.h"
#include "NetworkManager.h"
#include <GameEngineCore/PhysXCapsuleComponent.h>

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
	float4 LocalForward = GetTransform()->GetWorldLeftVector();
	//float4 LocalForward = Controller->GetMoveVector();

	float4 Cross = float4::Cross3DReturnNormal(LocalForward, _LookDir);
	//float4 Cross = float4::Cross3DReturnNormal(_LookDir, LocalForward);
	float Dot = float4::DotProduct3D(LocalForward, _LookDir);
	//float Dot = float4::DotProduct3D(_LookDir, LocalForward);

	if (Cross.y == -1.0f)
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

void BasePlayerActor::LookTarget()
{
	if (nullptr == LockOnEnemyTransform)
	{
		return;
	}
	LookDir((LockOnEnemyTransform->GetWorldPosition() - GetTransform()->GetWorldPosition()).NormalizeReturn());
}

void BasePlayerActor::RotationToDir(const float4& _Dir, float _MaxValue)
{
	float4 LocalForward = GetTransform()->GetWorldLeftVector();

	float4 Cross = float4::Cross3DReturnNormal(LocalForward, _Dir);
	float Dot = float4::DotProduct3D(LocalForward, _Dir);

	if (Cross.y == -1.0f)
	{
		Rot.y += std::min<float>(180.0f, _MaxValue);
		PhysXCapsule->SetWorldRotation(Rot);
	}
	else
	{
		float RotValue = -GameEngineMath::RadToDeg * Dot;
		if (RotValue < 0)
		{
			RotValue = std::max<float>(RotValue, -_MaxValue);
		}
		else
		{
			RotValue = std::min<float>(RotValue, _MaxValue);
		}
		Rot.y += RotValue;
		PhysXCapsule->SetWorldRotation(Rot);
	}
}

void BasePlayerActor::RotationToTarget(float _MaxValue /* = 360.0f */)
{
	if (nullptr == LockOnEnemyTransform)
	{
		return;
	}
	RotationToDir((LockOnEnemyTransform->GetWorldPosition() - GetTransform()->GetWorldPosition()).NormalizeReturn());
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

void BasePlayerActor::Start()
{
	// PhysX �ݸ���
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	PhysXCapsule->CreatePhysXActors({ 150, 100, 150 });
	PhysXCapsule->GetDynamic()->setMass(5.0f);
}

void BasePlayerActor::NetControllLoad()
{
	PhysXCapsule->TurnOffGravity();
}

void BasePlayerActor::UserControllLoad()
{
	// ���� ��Ʈ�� ������ ��� ����
	PhysXCapsule->SetMainPlayer();
	CustomCallback::SetMainPlayer(PhysXCapsule.get());

	Camera = GetLevel()->CreateActor<PlayerCamera>();
	Camera->SetPlayerTranform(GetTransform());

	// �÷��̾� ��Ʈ�ѷ� (���� ü��)
	Controller = CreateComponent<PlayerController>();
	Controller->SetCameraTransform(Camera->GetTransform());
	Controller->CallBack_LockOnDown = std::bind(&BasePlayerActor::LockOn, this);
	Controller->CallBack_LockOnUp = std::bind(&BasePlayerActor::LockOff, this);

	// �÷��̾� �浹ü
	PlayerCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Player);
	PlayerCollision->GetTransform()->SetLocalScale({ 100, 100, 100 });
	PlayerCollision->SetColType(ColType::OBBBOX3D);

	// �Ͽ� �� �浹ü
	LockOnCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Player);
	LockOnCollision->GetTransform()->SetLocalScale({ 1000, 500, 3000 });
	LockOnCollision->GetTransform()->SetLocalPosition({ 0, 0, 1500 });
	LockOnCollision->SetColType(ColType::OBBBOX3D);
}

void BasePlayerActor::Update_ProcessPacket()
{
	//��Ŷ�� �� ó���� �� ����
	while (GameEngineNetObject::IsPacket())
	{
		//���� ó���� ��Ŷ�� Ÿ���� �˾ƿɴϴ�
		PacketEnum Type = GameEngineNetObject::GetFirstPacketType<PacketEnum>();

		switch (Type)
		{
			//������Ʈ ��Ŷ�� ��쿣
		case PacketEnum::ObjectUpdatePacket:
		{
			//��Ŷ�� ���ø� �����ͷ� �����ɴϴ�(Enum���� �����Ͱ��� �°� ���ּž� �ϴ� �κ� ���Ǻ�Ź�����)
			std::shared_ptr<ObjectUpdatePacket> ObjectUpdate = PopFirstPacket<ObjectUpdatePacket>();

			//��Ŷ�� ������ ���� �ڽ��� �� ����
			Server_PrevPos = GetTransform()->GetWorldPosition();
			Server_NextPos = ObjectUpdate->Position;
			Server_Timer = 0.0f;
			PhysXCapsule->SetWorldRotation(ObjectUpdate->Rotation);
			ObjectUpdate->TimeScale;

			float TimeScale = ObjectUpdate->TimeScale;
			//unsigned int FsmState = ObjectUpdate->FsmState;
			//bool IsFsmForce = ObjectUpdate->IsFsmForce;
			break;
		}
		case PacketEnum::FsmChangePacket:
		{
			std::shared_ptr<FsmChangePacket> FsmChange = PopFirstPacket<FsmChangePacket>();
			SetFSMStateValue(FsmChange->FsmState);
			break;
		}
		default:
		{
			MsgAssert("ó������ ���ϴ� ��Ŷ�� �÷��̾�� ���ƿԽ��ϴ�.");
			return;
		}
		}
	}
}

void BasePlayerActor::Update(float _DeltaTime)
{
	Update_Character(_DeltaTime);

	if (NetControllType::NetControll == GameEngineNetObject::GetControllType())
	{
		Server_Timer += _DeltaTime;
		float Ratio = Server_Timer / Server_UpdateTime;
		PhysXCapsule->SetWorldPosition(float4::LerpClamp(Server_PrevPos, Server_NextPos, Ratio));
	}
	Update_SendPacket(_DeltaTime);
}

void BasePlayerActor::Update_SendPacket(float _DeltaTime)
{
	NetworkManager::PushUpdatePacket({ .ObjPtr = this, .TimeScale = 1.0f });
}

void BasePlayerActor::LockOn()
{
	std::vector<std::shared_ptr<GameEngineCollision>> Cols;
	std::shared_ptr<GameEngineCollision> MinCol = nullptr;
	if (true == LockOnCollision->CollisionAll(CollisionOrder::Enemy, Cols))
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
			return;
		}
		LockOnEnemyTransform = MinCol->GetActor()->GetTransform();
		Camera->SetTargetTranform(LockOnEnemyTransform);
		IsLockOn = true;
	}
}

void BasePlayerActor::LockOff()
{
	IsLockOn = false;
	LockOnEnemyTransform = nullptr;
	Camera->SetTargetTranform(nullptr);
}

bool BasePlayerActor::FloorCheck()
{
	float4 Point;
	return GetLevel()->RayCast(GetTransform()->GetWorldPosition(), float4::DOWN, Point, 100.0f) || PhysXCapsule->GetIsPlayerGroundTouch();
}

void BasePlayerActor::SetForce(float4 _Value)
{
	float4 re = _Value.RotaitonYDegReturn(Rot.y);
	PhysXCapsule->SetForce(re);
}

void BasePlayerActor::SetPush(float4 _Value)
{
	PhysXCapsule->SetPush(_Value.RotaitonYDegReturn(Rot.y));
}

void BasePlayerActor::SetMove(float4 _Value)
{
	PhysXCapsule->SetMove(_Value.RotaitonYDegReturn(Rot.y));
}

void BasePlayerActor::SetWorldPosition(float4 _Value)
{
	PhysXCapsule->SetWorldPosition(_Value);
}
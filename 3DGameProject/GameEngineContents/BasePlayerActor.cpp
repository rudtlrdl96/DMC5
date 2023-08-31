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

BasePlayerActor* BasePlayerActor::Instance = nullptr;

BasePlayerActor::BasePlayerActor()
{
	if (GameEngineNetObject::GetControllType() == NetControllType::UserControll)
	{
		Instance = this;
	}
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

void BasePlayerActor::LookTarget(const float4& _Target)
{
	LookDir((_Target - GetTransform()->GetWorldPosition()).NormalizeReturn());
}

void BasePlayerActor::Start()
{
	Camera = GetLevel()->CreateActor<PlayerCamera>();
	Camera->SetPlayerTranform(GetTransform());

	Controller = CreateComponent<PlayerController>();
	Controller->SetCameraTransform(Camera->GetTransform());
	Controller->CallBack_LockOnDown = std::bind(&BasePlayerActor::LockOn, this);
	Controller->CallBack_LockOnUp = std::bind(&BasePlayerActor::LockOff, this);

	PlayerCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Player);
	PlayerCollision->GetTransform()->SetLocalScale({ 100, 100, 100 });
	PlayerCollision->SetColType(ColType::OBBBOX3D);

	AttackCollision = CreateComponent<GameEngineCollision>(CollisionOrder::PlayerAttack);;

	physx::PxVec3 VecSclae = { 150, 100, 150 };

	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	PhysXCapsule->CreatePhysXActors(GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), VecSclae);
	PhysXCapsule->GetDynamic()->setMass(5.0f);
	PhysXCapsule->TurnOffGravity();

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
			if (true == ObjectUpdate->IsDeath)
			{
				Death();
				break;
			}

			//��Ŷ�� ������ ���� �ڽ��� �� ����
			GetTransform()->SetLocalPosition(ObjectUpdate->Position);
			GetTransform()->SetLocalRotation(ObjectUpdate->Rotation);
			ObjectUpdate->TimeScale;
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
	Update_ProcessPacket();
	Update_Character(_DeltaTime);

	if (NetControllType::UserControll == GameEngineNetObject::GetControllType())
	{
		//Update_Character(_DeltaTime);
	}
	Update_SendPacket(_DeltaTime);



}

void BasePlayerActor::Update_SendPacket(float _DeltaTime)
{
	NetworkManager::PushUpdatePacket(this, this, 1.f);
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


#include "PrecompileHeader.h"
#include "BaseEnemyActor.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "ContentsEnum.h"
#include "AttackCollision.h"
#include "BasePlayerActor.h"
#include "NetworkManager.h"

BaseEnemyActor::BaseEnemyActor()
{
}

BaseEnemyActor::~BaseEnemyActor()
{
}

void BaseEnemyActor::Update_ProcessPacket()
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
			Server_PrePosition = this->GetTransform()->GetWorldPosition();
			Server_NextPosition = ObjectUpdate->Position;
			Server_Rotation = ObjectUpdate->Rotation;
			Sever_Timeer = 0.0f;
			PhysXCapsule->SetWorldRotation(RotValue);
			ObjectUpdate->TimeScale;

			float TimeScale = ObjectUpdate->TimeScale;
			break;
		}
		case PacketEnum::FsmChangePacket:
		{
			//std::shared_ptr<FsmChangePacket> FsmChange = PopFirstPacket<FsmChangePacket>();
			//SetFSMStateValue(FsmChange->FsmState);
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

void BaseEnemyActor::Update_SendPacket(float _DeltaTime)
{
	NetworkManager::PushUpdatePacket({ .ObjPtr = this, .TimeScale = 1.0f, .UnionData = {0, } });
}

bool BaseEnemyActor::FloorCheck(float _Distance)
{
	float4 StartPosision = this->GetTransform()->GetWorldPosition();
	float4 Direction = float4::DOWN;
	float4 Results = float4::ZERO; // ���̰� ���� ����� �ñ��ϸ� �̰� ���
	float CheckDistance = _Distance;

	bool IsResults = this->GetLevel()->RayCast(StartPosision, float4::DOWN, Results, _Distance);

	return IsResults;
}

void BaseEnemyActor::Start()
{
	SetNetObjectType(Net_ActorType::HellCaina);

	//Render����
	EnemyRenderer = CreateComponent<GameEngineFBXRenderer>();
	//PhysX(�浹)
	PhysXCapsule = CreateComponent<PhysXCapsuleComponent>();
	PhysXCapsule->SetPhysxMaterial(0, 0, 0);
	PhysXCapsule->CreatePhysXActors({100, 70, 100});
	PhysXCapsule->SetWorldPosition({ 0, 100, 0 });

	//���� ������ Enemy Collision
	MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	MonsterCollision->GetTransform()->SetWorldScale(float4::ZERO);
	//�ֺ� �÷��̾ �ν��ϴ� Collision(1ȸ��)
	RN_MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	RN_MonsterCollision->GetTransform()->SetWorldScale(float4::ZERO);
	//������ ���ݹ��� Collision
	MonsterAttackRange = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	MonsterAttackRange->GetTransform()->SetWorldScale(float4::ZERO);
	
	//�ʱ�ȭ
	EnemyCodeValue = EnemyCode::None;
	EnemyTypeValue = EnemyType::None;
	EnemySizeValue = EnemySize::None;

	EnemyMeshLoad();
	EnemyTypeLoad();
	EnemyAnimationLoad();
	EnemyCreateFSM();
}


void BaseEnemyActor::Update(float _DeltaTime)
{
	NetControllType Type = GetControllType();
	switch (Type)
	{
	case NetControllType::UserControll:
		UserUpdate(_DeltaTime);
		break;
	case NetControllType::NetControll:
		ServerUpdate(_DeltaTime);
		break;
	default:
		break;
	}

	if (MonsterCollision->GetTransform()->GetWorldScale() == float4::ZERO)
	{
		MsgAssert("MonsterCollision�� ũ�⸦ ���������� �ʾҽ��ϴ�.");
	}
	if (RN_MonsterCollision->GetTransform()->GetWorldScale() == float4::ZERO)
	{
		MsgAssert("RN_MonsterCollision�� ũ�⸦ ���������� �ʾҽ��ϴ�.");
	}
	if (MonsterAttackRange->GetTransform()->GetWorldScale() == float4::ZERO)
	{
		MsgAssert("MonsterAttackRange�� ũ�⸦ ���������� �ʾҽ��ϴ�.");
	}
}

void BaseEnemyActor::UserUpdate(float _DeltaTime)
{
	if (true == EnemyFSM.IsValid())
	{
		EnemyFSM.Update(_DeltaTime);
	}
}

void BaseEnemyActor::ServerUpdate(float _DeltaTime)
{
}

void BaseEnemyActor::SuperArmorOn()
{
	IsSuperArmorValue = true;

	if (nullptr != SuperArmorOn_Callback) 
	{
		SuperArmorOn_Callback();
	}
}

void BaseEnemyActor::SuperArmorOff()
{
	IsSuperArmorValue = false;

	if (nullptr != SuperArmorOff_Callback)
	{
		SuperArmorOff_Callback();
	}
}

void BaseEnemyActor::ChasePlayer(float _DeltaTime)
{
	////�������� Player�� vector�� ���������. �� ���͸� ���鼭 ����
	//for (BasePlayerActor* Players : BasePlayerActor::GetPlayers())
	//{
	//	float4 EnemyPosition = EnemyRenderer->GetTransform()->GetWorldPosition();
	//	float4 PlayerPosition = Players->GetTransform()->GetWorldPosition();
	//	PhysXCapsule->SetMove((PlayerPosition - EnemyPosition));
	//	//�ڿ������� ������
	//	ColValue = ForWardCollision->Collision(CollisionOrder::RN_Player, ColType::OBBBOX3D, ColType::OBBBOX3D);
	//	if (nullptr == ColValue)
	//	{
	//		float4 EnemyForWardVector = EnemyRenderer->GetTransform()->GetWorldForwardVector();
	//		EnemyForWardVector.y = 0;
	//		EnemyForWardVector.Normalize();

	//		float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
	//		ToPlayerVector.y = 0;
	//		ToPlayerVector.Normalize();

	//		float4 CrossVector = float4::Cross3DReturnNormal(EnemyForWardVector, ToPlayerVector);
	//		if (CrossVector.y < 0){	RotationValue = -2;}
	//		else{RotationValue = 2;}

	//		EnemyRenderer->GetTransform()->AddLocalRotation({ 0,RotationValue,0 });
	//		ForWardCollision->GetTransform()->AddLocalRotation({ 0,RotationValue,0 });
	//	}
	//}
}

float4 BaseEnemyActor::CrossMonsterAndPlayer()
{
	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	BasePlayerActor* Player = Players[0];

	if (nullptr == Player)
	{
		return float4::ZERO;
	}

	float4 EnemyPosition = this->GetTransform()->GetWorldPosition();
	float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();
	float4 EnemyForWardVector = this->GetTransform()->GetWorldForwardVector();

	EnemyForWardVector.y = 0;
	EnemyForWardVector.Normalize();

	float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
	ToPlayerVector.y = 0;
	ToPlayerVector.Normalize();

	float4 CrossResult = float4::Cross3DReturnNormal(EnemyForWardVector, ToPlayerVector);

	return CrossResult;
}

float BaseEnemyActor::DotProductMonsterAndPlayer()
{
	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	BasePlayerActor* Player = Players[0];

	if (nullptr == Player)
	{
		return 0.0f;
	}

	float4 EnemyPosition = this->GetTransform()->GetWorldPosition();
	float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();
	float4 EnemyForWardVector = this->GetTransform()->GetWorldForwardVector();

	EnemyForWardVector.y = 0;
	EnemyForWardVector.Normalize();

	float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
	ToPlayerVector.y = 0;
	ToPlayerVector.Normalize();

	float DotProductResult = float4::DotProduct3D(EnemyForWardVector, ToPlayerVector);

	return DotProductResult;
}

float BaseEnemyActor::RotationToPlayerValue()
{
	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	BasePlayerActor* Player = Players[0];

	float RotationValue = 0.0f;

	if (nullptr == Player)
	{
		return RotationValue;
	}

	float4 EnemyPosition = this->GetTransform()->GetWorldPosition();
	float4 PlayerPosition = Player->GetTransform()->GetWorldPosition();
	float4 EnemyForWardVector = this->GetTransform()->GetWorldForwardVector();

	EnemyForWardVector.y = 0;
	EnemyForWardVector.Normalize();

	float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
	ToPlayerVector.y = 0;
	ToPlayerVector.Normalize();

	float4 CrossResult = CrossMonsterAndPlayer();

	float4 Direct = PlayerPosition - EnemyPosition;
	float4 RotationDirectNormal = Direct.NormalizeReturn();
	RotationValue = float4::GetAngleVectorToVectorDeg(EnemyForWardVector, RotationDirectNormal);

	if (CrossResult.y < 0)
	{
		RotationValue = -RotationValue;
	}

	return RotationValue;
}

void BaseEnemyActor::CheckHeadingRotationValue()
{
	float4 CrossResult = CrossMonsterAndPlayer();

	if (float4::ZERO == CrossResult)
	{
		return;
	}

	float DotProductResult = RotationToPlayerValue();

	if (0.0f == DotProductResult)
	{
		return;
	}

	if (0.0f <= DotProductResult && 15.0f >= DotProductResult)
	{
		EnemyRotationValue = EnemyRotation::Forward;
	}
	else if (CrossResult.y < 0)
	{
		if (-15.0f > DotProductResult && -165.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left;
		}
		else if (-165.0f > DotProductResult && -180.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left_180;
		}
	}
	else if (CrossResult.y > 0)
	{
		if (15.0f < DotProductResult && 165.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right;
		}
		else if (165.0f < DotProductResult && 180.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right_180;
		}
	}
	else
	{
		MsgAssert("ȸ���� ������ ������ �� �߸���");
	}

	RotationValue = DotProductResult;
}

void BaseEnemyActor::SlerpCalculation()
{
	CurRotation = GetTransform()->GetWorldRotation();

	if (CurRotation.y <= 0.0f)
	{
		CurRotation.y += 360.f;
	}

	float4 Value = float4{ 0.0f, RotationValue, 0.0f };

	GoalRotation = CurRotation + Value;

	if (GoalRotation.y <= 0.0f)
	{
		CurRotation.y += 360.f;
		GoalRotation = CurRotation + Value;
	}

	CurRotation.x = 0.0f;
	CurRotation.z = 0.0f;
	GoalRotation.x = 0.0f;
	GoalRotation.z = 0.0f;
}

void BaseEnemyActor::SlerpTurn(float _DeltaTime)
{
	SlerpTime += _DeltaTime;
	RotValue = float4::SLerpQuaternion(CurRotation, GoalRotation, SlerpTime);
	PhysXCapsule->SetWorldRotation(RotValue);
}
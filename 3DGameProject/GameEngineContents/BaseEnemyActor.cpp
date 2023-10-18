#include "PrecompileHeader.h"
#include "BaseEnemyActor.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "ContentsEnum.h"
#include "AttackCollision.h"
#include "BasePlayerActor.h"
#include "NetworkManager.h"
#include "FsmChangePacket.h"
#include "AttackCollision.h"

BaseEnemyActor::BaseEnemyActor()
{
}

BaseEnemyActor::~BaseEnemyActor()
{
}

void BaseEnemyActor::Start()
{
	if (false == GameEngineInput::IsKey("MonsterTest"))
	{
		GameEngineInput::CreateKey("MonsterTest", 'M');
	}

	//Render����
	EnemyRenderer = CreateComponent<GameEngineFBXRenderer>();
	EnemyRenderer->ShadowOn();
	EnemyRenderer->SetDynamic();

	//���� ������ Enemy Collision
	//Monster ��ü �ݸ���
	MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::Enemy);
	MonsterCollision->GetTransform()->SetWorldScale(float4::ZERO);
	//Monster ���� �ݸ���
	MonsterAttackCollision = CreateComponent<AttackCollision>(CollisionOrder::EnemyAttack);
	MonsterAttackCollision->GetTransform()->SetWorldScale(float4::ZERO);
	//Monster ���� ���� �ν� �ݸ���
	RN_MonsterCollision = CreateComponent<GameEngineCollision>(CollisionOrder::RN_Enemy);
	RN_MonsterCollision->GetTransform()->SetWorldScale(float4::ZERO);
	RN_MonsterCollision->SetColType(ColType::SPHERE3D);

	EnemyMeshLoad();
	EnemyTypeLoad();
	EnemyAnimationLoad();
	
	if (true == NetworkManager::IsClient())
	{
		EnemyCreateFSM_Client();
	}
	else if (true == NetworkManager::IsServer())
	{
		EnemyCreateFSM();
	}
	else
	{
		EnemyCreateFSM();
	}

	PlayerCheckInit();

	SetFsmPacketCallBack(std::bind(&BaseEnemyActor::SetFSMStateValue, this, std::placeholders::_1));
}

void BaseEnemyActor::Update(float _DeltaTime)
{
	AppearDelayTime += _DeltaTime;

	if (AppearDelayTime <= 1.0f)
	{
		return;
	}

	// �̱�, ����, Ŭ���̾�Ʈ ������� ����ؼ� üũ�Ǿ�� �ϴ� �Լ���
	{
		_DeltaTime *= GetTimeScale();
		EnemyRenderer->SetTimeScale(GetTimeScale());

		if (true == TimeStop)
		{
			TimeStop = false;
			if (true == IsTimeStop)
			{
				MonsterAttackCollision->Off();
				PhysXCapsule->Off();
			}
			else if (false == IsTimeStop)
			{
				PhysXCapsule->On();
			}
		}

		RendererBurn(_DeltaTime);
		RenderShake(_DeltaTime);
		MonsterSnatch(_DeltaTime);
	}

	// �̱� �÷����� �� ����
	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		RecognizeCollisionCheck(_DeltaTime);
		DamageCollisionCheck(_DeltaTime);
		EnemyFSM.Update(_DeltaTime);
	}
	else 
	{
		// ���� �÷����� �� ����
		if (NetControllType::ActiveControll == GetControllType())
		{
			RecognizeCollisionCheck(_DeltaTime);
			DamageCollisionCheck(_DeltaTime);
			EnemyFSM.Update(_DeltaTime);
		}
		// Ŭ���̾�Ʈ �÷����� �� ����
		else
		{
			Update_NetworkTrans(_DeltaTime);
			DamageCollisionCheck_Client(_DeltaTime);
			EnemyFSM.Update(_DeltaTime);
			MonsterAttackCollision->Off();
		}
	}
}

/// <summary>
/// ���� ��ġ���� �Ʒ� �������� Raycast�� �ǽ��Ͽ� �ٴ�(������ �浹ü)�� üũ�Ѵ�.
/// </summary>
/// <param name="_Distance :">Raycasting ����, ���͸��� �ٸ�</param>
bool BaseEnemyActor::FloorCheck(float _Distance)
{
	float4 StartPosision = this->GetTransform()->GetWorldPosition();
	float4 Direction = float4::DOWN;
	float4 Results = float4::ZERO; // ���̰� ���� ����� �ñ��ϸ� �̰� ���
	float CheckDistance = _Distance;

	bool IsResults = this->GetLevel()->RayCast(StartPosision, float4::DOWN, Results, _Distance);

	return IsResults;
}

/// <summary>
/// ���� ���� ���� �� Player ������ class BasePlayerActor* Player�� ����
/// </summary>
void BaseEnemyActor::PlayerCheckInit()
{
	std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
	size_t Playersize = Players.size();

	if (0 == Players.size())
	{
		Player = nullptr;
		return;
	}

	Player = Players[0];
	int PlayerID = Player->GetNetObjectID();

	if (nullptr == Player)
	{
		MsgAssert("Player ������ �� �߸���");
	}
}

/// <summary>
/// RN_MonsterCollision�� �浹�� �浹ü(Player) ������ class BasePlayerActor* Player�� ����
/// </summary>
/// <param name="_DeltaTime :">�ð�</param>
/// <param name="_Collision :">üũ�� �浹ü</param>
bool BaseEnemyActor::PlayerContactCheck(float _DeltaTime, GameEngineCollision* _Collision)
{
	//ContactDelayTime += _DeltaTime;

	//if (10.0f >= ContactDelayTime)
	//{
	//	return false;
	//}

	ContactDelayTime = 0.0f;

	GameEngineCollision* CheckCollision = _Collision;
	BasePlayerActor* ContactPlayer = dynamic_cast<BasePlayerActor*>(CheckCollision->GetActor());

	if (nullptr == ContactPlayer)
	{
		return false;
	}

	bool IsContact = false;

	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		//Player = ContactPlayer;
		IsContact = true;
	}
	else
	{
		int ContactID = ContactPlayer->GetNetObjectID();
		int PlayerID = Player->GetNetObjectID();

		if (ContactID == PlayerID)
		{
			Player = ContactPlayer;
			IsContact = true;
		}
		else
		{
			IsContact = false;
		}
	}

	return IsContact;
}

/// <summary>
/// RN_MonsterCollision�� �浹�� �浹ü(Player) ������ class BasePlayerActor* Player�� ����
/// </summary>
/// <param name="_DeltaTime :">�ð�</param>
/// <param name="_Collision :">üũ�� �浹ü</param>
bool BaseEnemyActor::PlayerContactCheck_Normal(GameEngineCollision* _Collision)
{
	GameEngineCollision* CheckCollision = _Collision;
	BasePlayerActor* ContactPlayer = dynamic_cast<BasePlayerActor*>(CheckCollision->GetActor());

	if (nullptr == ContactPlayer)
	{
		return false;
	}

	bool IsContact = false;

	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		//Player = ContactPlayer;
		IsContact = true;
	}
	else
	{
		int ContactID = ContactPlayer->GetNetObjectID();
		int PlayerID = Player->GetNetObjectID();

		if (ContactID == PlayerID)
		{
			IsContact = true;
		}
	}

	return IsContact;
}

/// <summary>
/// �ڽ��� ������ �浹ü(Player) ������ class BasePlayerActor* Player�� ����
/// </summary>
/// <param name="_Collision :">üũ�� �浹ü</param>
void BaseEnemyActor::PlayerAttackCheck(GameEngineCollision* _Collision)
{
	GameEngineCollision* CheckCollision = _Collision;
	BasePlayerActor* ContactPlayer = dynamic_cast<BasePlayerActor*>(CheckCollision->GetActor());

	if (nullptr == ContactPlayer)
	{
		return;
	}

	if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
	{
		Player = ContactPlayer;
	}
	else
	{
		int ContactID = ContactPlayer->GetNetObjectID();
		int PlayerID = Player->GetNetObjectID();

		if (ContactID == PlayerID)
		{
			Player = ContactPlayer;
		}
		else
		{
			std::vector<BasePlayerActor*>& Players = BasePlayerActor::GetPlayers();
			size_t Playersize = Players.size();

			for (size_t i = 0; i < Playersize; i++)
			{
				int PlayersID = Players[i]->GetNetObjectID();

				if (ContactID == PlayersID)
				{
					Player = Players[i];
				}
			}
		}
	}
}

/// <summary>
/// Player�� ���� ���� ������ ��ġ�� �����մϴ�. (���� �� ������ �����ϱ� ����)
/// </summary>
float4 BaseEnemyActor::MonsterAndPlayerCross()
{
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

/// <summary>
/// Player�� ���� ���� ������ ��ġ�� �����մϴ�. (�ش� �������� ��� ȸ���ؾ� �ϴ��� �˱� ����)
/// </summary>
float BaseEnemyActor::MonsterAndPlayerDotProduct()
{
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

/// <summary>
/// Player�� ���� ���� ������ ��ġ�� �����մϴ�. Degree�� ���� ��ȯ���ݴϴ�.
/// </summary>
float BaseEnemyActor::MonsterAndPlayerDotProductDegree()
{
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

	float4 CrossResult = MonsterAndPlayerCross();

	float4 Direct = PlayerPosition - EnemyPosition;
	float4 RotationDirectNormal = Direct.NormalizeReturn();
	RotationValue = float4::GetAngleVectorToVectorDeg(EnemyForWardVector, RotationDirectNormal);

	if (CrossResult.y < 0)
	{
		RotationValue = -RotationValue;
	}

	return RotationValue;
}

/// <summary>
/// Player�� ���� ���� ������ ��ġ�� ����, ������ �� �� � �������� ȸ���ؾ��ϴ��� EnemyRotation EnemyRotationValue�� ���� �����մϴ�.
/// </summary>
void BaseEnemyActor::RotationCheck()
{
	float4 CrossResult = MonsterAndPlayerCross();

	if (float4::ZERO == CrossResult)
	{
		return;
	}

	float DotProductResult = MonsterAndPlayerDotProductDegree();

	if (0.0f == DotProductResult)
	{
		return;
	}

	if (-20.0f <= DotProductResult && 20.0f >= DotProductResult)
	{
		EnemyRotationValue = EnemyRotation::Forward;
	}
	else if (CrossResult.y < 0)
	{
		if (-20.0f > DotProductResult && -75.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left;
		}
		else if (-75.0f > DotProductResult && -165.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left_90;
		}
		else if (-165.0f > DotProductResult && -180.0f <= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Left_180;
		}
	}
	else if (CrossResult.y > 0)
	{
		if (20.0f < DotProductResult && 75.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right;
		}
		else if (75.0f < DotProductResult && 165.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right_90;
		}
		else if (165.0f < DotProductResult && 180.0f >= DotProductResult)
		{
			EnemyRotationValue = EnemyRotation::Right_180;
		}
	}

	DotProductValue = DotProductResult;
}

/// <summary>
/// Player�� ���� ���� ������ ��ġ�� ����, ������ �� �� � ���⿡�� ���� �޾Ҵ����� EnemyHitDirect EnemyHitDirValue�� �����մϴ�.
/// </summary>
void BaseEnemyActor::AttackDirectCheck()
{
	float4 CrossResult = MonsterAndPlayerCross();

	if (float4::ZERO == CrossResult)
	{
		return;
	}

	float DotProductResult = MonsterAndPlayerDotProductDegree();

	if (0.0f == DotProductResult)
	{
		return;
	}

	if (-50.0f <= DotProductResult && 50.0f >= DotProductResult)
	{
		EnemyHitDirValue = EnemyHitDirect::Forward;
	}
	else if (CrossResult.y < 0)
	{
		if (-50.0f > DotProductResult && -130.0f <= DotProductResult)
		{
			EnemyHitDirValue = EnemyHitDirect::Left;
		}
		else if (-130.0f > DotProductResult && -180.0f <= DotProductResult)
		{
			EnemyHitDirValue = EnemyHitDirect::Back;
		}
	}
	else if (CrossResult.y > 0)
	{
		if (50.0f < DotProductResult && 130.0f >= DotProductResult)
		{
			EnemyHitDirValue = EnemyHitDirect::Right;
		}
		else if (130.0f < DotProductResult && 180.0f >= DotProductResult)
		{
			EnemyHitDirValue = EnemyHitDirect::Back;
		}
	}
}

/// <summary>
/// Slerp�� ȸ���ؾ� �ϴ� ��쿡 ���� ���� ����մϴ�. ��� ����� float4 CurRotation, float4 GoalRotation�� ����˴ϴ�. Y�࿡ ���� ȸ���� �����ϴ�.
/// </summary>
void BaseEnemyActor::SlerpCalculation()
{
	CurRotation = GetTransform()->GetWorldRotation();

	if (CurRotation.y <= 0.0f)
	{
		CurRotation.y += 360.f;
	}

	float4 Value = float4{ 0.0f, DotProductValue, 0.0f };

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
	//SlerpTime = 0.0f;
}

/// <summary>
/// ���� �ð� ���� SlerpCalculation()()�� ����  ���� ���� ���͸� Slerp��Ų��.
/// </summary>
/// <param name="_DeltaTime :">�ð�</param>
void BaseEnemyActor::SlerpTurn(float _DeltaTime)
{
	SlerpTime += _DeltaTime;
	RotationValue = float4::SLerpQuaternion(CurRotation, GoalRotation, SlerpTime);
	PhysXCapsule->SetWorldRotation(RotationValue);
}

/// <summary>
/// ������ ���� ��ġ�� ������ ���� ����, �Ĺ�, ����, ������ ���� �븻 ���Ͱ��� �������ش�. Phsysics�� Engine ������ 1������ ���̳��� ������ ���͸� ���Ƿ� ȸ����Ų��.
/// </summary>
void BaseEnemyActor::AllDirectSetting()
{
	PhysXCapsule->AddWorldRotation({ 0, DotProductValue, 0 });
	
	ForwardDirect = GetTransform()->GetWorldForwardVector().NormalizeReturn();
	BackDirect = GetTransform()->GetWorldBackVector().NormalizeReturn();
	RightDirect = GetTransform()->GetWorldRightVector().NormalizeReturn();
	LeftDirect = GetTransform()->GetWorldLeftVector().NormalizeReturn();

	// �������� ���̳��� ���������
	ForwardDirect.RotationYDeg(DotProductValue);
	BackDirect.RotationYDeg(DotProductValue);
	RightDirect.RotationYDeg(DotProductValue);
	LeftDirect.RotationYDeg(DotProductValue);
}

/// <summary>
/// ������ ���� ��ġ�� ������ ���� ����, �Ĺ�, ����, ������ ���� �븻 ���Ͱ��� �������ش�.
/// </summary>
void BaseEnemyActor::AllDirectSetting_Normal()
{
	ForwardDirect = GetTransform()->GetWorldForwardVector().NormalizeReturn();
	BackDirect = GetTransform()->GetWorldBackVector().NormalizeReturn();
	RightDirect = GetTransform()->GetWorldRightVector().NormalizeReturn();
	LeftDirect = GetTransform()->GetWorldLeftVector().NormalizeReturn();
}

/// <summary>
/// ������ ���� ��ġ�� ������ ���� ���ư� ������ PushDirect�� �븻���ͷ� �����Ѵ�.
/// </summary>
void BaseEnemyActor::PushDirectSetting()
{
	if (nullptr == Player)
	{
		return;
	}

	PushDirect = Player->GetTransform()->GetWorldForwardVector().NormalizeReturn();
}

/// <summary>
/// ���۾Ƹ� ���¿��� ��Ʈ �� �������� �¿�� ����.
/// </summary>
/// <param name="_DeltaTime :">0.025�ʸ��� 1ȸ ���� üũ�� ���� �ð�</param>
void BaseEnemyActor::RenderShake(float _DeltaTime)
{
	if (true == DeathValue)
	{
		return;
	}

	if (false == IsRenderShaking)
	{
		ShakingCameraSetting = 1;
		return;
	}

	if (1 == ShakingCameraSetting)
	{
		RenderShakeTime = 0.0f;
		ShakingCount = 0;
		ShakingCameraSetting = 0;
		CurRenderPosition = EnemyRenderer->GetTransform()->GetLocalPosition();
		EnemyRenderer->GetTransform()->SetLocalPosition(CurRenderPosition);
	}

	RenderShakeTime += _DeltaTime;

	if (0.025f <= RenderShakeTime)
	{
		RenderShakeTime = 0.0f;

		if (ShakingMaxCount == ShakingCount)
		{
			ShakingCount = 0;
			IsRenderShaking = false;
			ShakingCameraSetting = 1;
			RenderShakeTime = 0.0f;
			EnemyRenderer->GetTransform()->SetLocalPosition(CurRenderPosition);
		}
		else if (0 == ShakingCount % 2)
		{
			++ShakingCount;
			EnemyRenderer->GetTransform()->SetLocalPosition({ CurRenderPosition.x + 3 , CurRenderPosition.y, CurRenderPosition.z - 3 });
		}
		else if (1 == ShakingCount % 2)
		{
			++ShakingCount;
			EnemyRenderer->GetTransform()->SetLocalPosition({ CurRenderPosition.x - 3 , CurRenderPosition.y, CurRenderPosition.z + 3 });
		}
	}
}

/// <summary>
/// ������ ��Ʈ �� ���͸� Player FBX Bone�� � ��ġ�� �������� �����Ѵ�.
/// </summary>
/// <param name="_attachposition :">FBX Bone�� ������ ���¿����� ������ Position</param>
void BaseEnemyActor::BusterCalculation(float4 _attachposition)
{
	if (nullptr == Player)
	{
		return;
	}

	CurRenderPosition = EnemyRenderer->GetTransform()->GetLocalPosition();

	float4 Forword = Player->GetTransform()->GetWorldForwardVector() * 120.0f;
	float4 BusterPosition = Player->GetTransform()->GetWorldPosition() + Forword + float4{0.f, 40.f, 0.f};
	
	PhysXCapsule->SetWorldPosition(BusterPosition);
	Player->GetPlayerRenderer()->SetAttachTransform("R_Hand", EnemyRenderer->GetTransform(), _attachposition/*, float4(0.0f, 0.0f, 0.0f)*/);
}

/// <summary>
/// ������ ���� �� ���� �������� ��ġ�� ���� ��ġ�� �ǵ�����.
/// </summary>
void BaseEnemyActor::BusterEnd()
{
	if (nullptr == Player)
	{
		return;
	}

	Player->GetPlayerRenderer()->SetDettachTransform();
	EnemyRenderer->GetTransform()->SetLocalPosition(CurRenderPosition);
}

/// <summary>
/// ����ġ ��Ʈ �� Lerp�� ���Ϳ� Player�� ��ġ���� float4 SnatchStartPosition, float4 SnatchEndPosition�� ���� �����Ѵ�.
/// </summary>
void BaseEnemyActor::SnatchCalculation()
{
	if (nullptr == Player)
	{
		return;
	}

	SnatchStartPosition = this->GetTransform()->GetWorldPosition();
	float4 Forword = Player->GetTransform()->GetWorldForwardVector() * 120.0f;
	SnatchEndPosition = Player->GetTransform()->GetWorldPosition() + Forword + float4{0.f, -20.f, 0.f};
}

/// <summary>
/// ���� �ð�(1.0f) ���� ���� SnatchCalculation()�� ���� ���� ���� ���͸� Lerp��Ų��.
/// </summary>
/// <param name="_DeltaTime :">1.0�ʸ� üũ�� �ð�</param>
void BaseEnemyActor::MonsterSnatch(float _DeltaTime)
{
	if (true == DeathValue)
	{
		return;
	}

	if (false == IsSnatch)
	{
		return;
	}

	SnatchTime += _DeltaTime * 3.0f;
	float4 LerpPosition = float4::LerpClamp(SnatchStartPosition, SnatchEndPosition, SnatchTime);
	PhysXCapsule->SetWorldPosition(LerpPosition);

	if (1.0f <= SnatchTime)
	{
		SetMoveStop();
		IsSnatch = false;
		SnatchTime = 0.0f;
	}
}

/// <summary>
/// �Էµ� DamageType�� ���� �����ð����� Ÿ�ӽ������� ����ϴ�
/// </summary>
/// <param name="_Type :">DamageType</param>
void BaseEnemyActor::HitStop(DamageType _Type)
{
	float _TimeScale = 0.1f;
	float StopTime = 0.0f;

	switch (_Type)
	{
	case DamageType::VergilLight:
		StopTime = 0.01f;
		break;
	case DamageType::Light:
		StopTime = 0.05f;
		break;
	case DamageType::Heavy:
		StopTime = 0.08f;
		break;
	case DamageType::Air:
		StopTime = 0.05f;
		break;
	case DamageType::Slam:
		StopTime = 0.08f;
		break;
	case DamageType::Stun:
		return;
	default:
		break;
	}

	SetTimeScale(_TimeScale);

	GetLevel()->TimeEvent.AddEvent(StopTime, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
	{
		SetTimeScale(1.0f);
	});
}

/// <summary>
/// �����ð����� Ÿ�ӽ������� 0���� �մϴ�. _DeltaTime �� �� Ÿ�ӽ������� 1�� �˴ϴ�. CapsuleCollision�� Off�˴ϴ�
/// </summary>
/// <param name="_DeltaTime :">Ÿ�� �������� 1�� �Ǳ������ �ð��� üũ�ϱ� ����</param>
void BaseEnemyActor::StopTime(float _DeltaTime)
{
	TimeStop = true;
	IsTimeStop = true;
	SetTimeScale(0.0f);
	GetLevel()->TimeEvent.AddEvent(_DeltaTime, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
	{
		TimeStop = true;
		IsTimeStop = false;
		SetTimeScale(1.0f);
	});
}

void BaseEnemyActor::RendererBurn(float _DeltaTime)
{
	if (false == IsBurn)
	{
		return;
	}

	DeathColor += _DeltaTime;

	if (DeathColor <= 1.1f)
	{
		EnemyRenderer->SetBaseColor({ 0.0f, 0.0f, DeathColor });
	}

	if (DeathColor >= 1.0f)
	{
		// �̱� �÷����� �� ����
		if (false == NetworkManager::IsClient() && false == NetworkManager::IsServer())
		{
			Death();
		}
		else
		{
			if (NetControllType::ActiveControll == GetControllType())
			{
				Death();
			}
		}
	}
}

//void BaseEnemyActor::ChasePlayer(float _DeltaTime)
//{
//	//�������� Player�� vector�� ���������. �� ���͸� ���鼭 ����
//	for (BasePlayerActor* Players : BasePlayerActor::GetPlayers())
//	{
//		float4 EnemyPosition = EnemyRenderer->GetTransform()->GetWorldPosition();
//		float4 PlayerPosition = Players->GetTransform()->GetWorldPosition();
//		CapsulCol->SetMove((PlayerPosition - EnemyPosition));
//		//�ڿ������� ������
//		ColValue = ForWardCollision->Collision(CollisionOrder::RN_Player, ColType::OBBBOX3D, ColType::OBBBOX3D);
//		if (nullptr == ColValue)
//		{
//			float4 EnemyForWardVector = EnemyRenderer->GetTransform()->GetWorldForwardVector();
//			EnemyForWardVector.y = 0;
//			EnemyForWardVector.Normalize();
//			float4 ToPlayerVector = (PlayerPosition - EnemyPosition);
//			ToPlayerVector.y = 0;
//			ToPlayerVector.Normalize();
//			float4 CrossVector = float4::Cross3DReturnNormal(EnemyForWardVector, ToPlayerVector);
//			if (CrossVector.y < 0)
//			{
//				EnemyRenderer->GetTransform()->AddLocalRotation({ 0,-2,0 });
//				ForWardCollision->GetTransform()->AddLocalRotation({ 0,-2,0 });
//			}
//			else
//			{
//				EnemyRenderer->GetTransform()->AddLocalRotation({ 0,2,0 });
//				ForWardCollision->GetTransform()->AddLocalRotation({ 0,2,0 });
//			}
//		}
//
//	}
//}
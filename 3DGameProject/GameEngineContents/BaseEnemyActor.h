#pragma once
#include <GameEngineCore/GameEngineFSM.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>
#include "NetworkObjectBase.h"

enum class EnemyCode
{
	//�ʱ�ȭ��
	None = 100000,
	// �Ϲ� ����
	Empusa = 0,
	GreenEmpusa = 1,
	RedEmpusa = 2,
	HellCaina = 1000,
	HellAntenora = 1001,

	// ���� ����
	CavliereAngelo = 5000,
};

enum class EnemyHitDirect
{
	Forward,
	Back,
	Left,
	Right,
};

enum class EnemyRotation
{
	Forward,
	Left,
	Left_90,
	Left_180,
	Right,
	Right_90,
	Right_180,
};

class BaseEnemyActor : public GameEngineActor, public NetworkObjectBase
{
	friend class EnemySpawnArea;
public:
	BaseEnemyActor();
	~BaseEnemyActor();

	BaseEnemyActor(const BaseEnemyActor& _Other) = delete;
	BaseEnemyActor(BaseEnemyActor&& _Other) noexcept = delete;
	BaseEnemyActor& operator=(const BaseEnemyActor& _Other) = delete;
	BaseEnemyActor& operator=(BaseEnemyActor&& _Other) noexcept = delete;

	// ���� ������ ������Ʈ ����
	std::shared_ptr<class PhysXCapsuleComponent> GetPhysXComponent()
	{
		return PhysXCapsule;
	}

	// ��Ŷ���� ���۹��� FSM�� Ŭ���̾�Ʈ FSM�� ����
	void SetFSMStateValue(int _StateValue)
	{
		EnemyFSM.ChangeState(_StateValue);
	}

	void StartRenderShaking(short _ShakingMaxCount)
	{
		ShakingMaxCount = _ShakingMaxCount;
		IsRenderShaking = true;
	}

	void StartMonsterSnatch()
	{
		SnatchCalculation();
		IsSnatch = true;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	virtual void EnemyMeshLoad() = 0;
	virtual void EnemyTypeLoad() = 0;
	virtual void EnemyAnimationLoad() = 0;
	virtual void EnemyCreateFSM() = 0;
	virtual void EnemyCreateFSM_Client() = 0;
	virtual void DamageCollisionCheck(float _DeltaTime) = 0;
	virtual void DamageCollisionCheck_Client(float _DeltaTime) = 0;
	virtual void RecognizeCollisionCheck(float _DeltaTime) = 0;

	std::shared_ptr<class GameEngineFBXRenderer> EnemyRenderer = nullptr;     // ������
	std::shared_ptr<class PhysXCapsuleComponent> PhysXCapsule = nullptr;      // ������ ������Ʈ
	std::shared_ptr<class GameEngineCollision> MonsterCollision = nullptr;    // ���� ��ü �ݸ���
	std::shared_ptr<class AttackCollision> MonsterAttackCollision = nullptr;  // ���� ���� �ݸ���
	std::shared_ptr<class GameEngineCollision> RN_MonsterCollision = nullptr; // ���� ���� ���� �ν� �ݸ���

	//�������� ����������ϴ� Data��=====================================================
	EnemyCode EnemyCodeValue = EnemyCode::None;
	//HP
	float EnemyHP = 0.0f;
	// FSM ���
	int EnemyFSMValue = -1;
	//=================================================================================

	//====================================================
	// �÷��̾� ����
	void PlayerCheckInit();
	void PlayerContactCheck(float _DeltaTime, GameEngineCollision* _Collision);
	void PlayerAttackCheck(GameEngineCollision* _Collision);

	class BasePlayerActor* Player = nullptr;
	GameEngineFSM EnemyFSM;
	float AppearDelayTime = 0.0f;
	float ContactDelayTime = 0.0f;
	//====================================================

	//====================================================
	// ���� �ٴ� üũ (RayCast)
	bool FloorCheck(float _Distance);
	float FallDistance = 0.0f;     // FloorCheck() �� �ʿ�, ������ ���� Start �κп��� �� ����
	//====================================================

	//====================================================
	// ���� ����ũ
	float4 CurRenderPosition = float4::ZERO;
	bool IsRenderShaking = false;
	float RenderShakeTime = 0.0f;
	short ShakingCameraSetting = 0;
	short ShakingCount = 0;
	short ShakingMaxCount = 0;

	void RenderShake(float _DeltaTime);
	//====================================================

	//====================================================
	// Slerp ��꿡 ���
	EnemyRotation EnemyRotationValue = EnemyRotation::Forward;
	EnemyHitDirect EnemyHitDirValue = EnemyHitDirect::Forward;

	float4 CurRotation = float4::ZERO;
	float4 GoalRotation = float4::ZERO;
	float4 RotationValue = float4::ZERO;     // Slerp ���� �����
	float SlerpTime = 0.0f;
	float DotProductValue = 0.0f;            // CheckHeadingRotationValue() ���� �� ���� �����
	float AnimationRotation = 0.0f;
	float AnimationRotationTime = 0.0f;
	float CalFrame = 0.0f;
	UINT StartFrame = 0;
	UINT EndFrame = 0;
	bool AnimationTurnStart = false;

	float4 MonsterAndPlayerCross();			 // �÷��̾�� ���� ����
	float MonsterAndPlayerDotProduct();      // �÷��̾�� ���� ���� (�׳� ����)
	float MonsterAndPlayerDotProductDegree();// �÷��̾�� ���� ������ deg�� ��ȯ (������ +, ���� -, 180�� ����)
	void RotationCheck();            		 // ����, ���� �� � ������ ȸ������ ����
	void AttackDirectCheck();                // �÷��̾� ���� ���� üũ
	void SlerpCalculation();				 // slerp ���� ���
	void SlerpTurn(float _DeltaTime);        // slerp�� ��
	void AnimationSlerpCalculation();
	void AnimationSlerpTurn(float _DeltaTime);
	//====================================================

	//====================================================
	// ���� ����
	float4 ForwardDirect = float4::ZERO;
	float4 BackDirect = float4::ZERO;
	float4 RightDirect = float4::ZERO;
	float4 LeftDirect = float4::ZERO;
	float4 PushDirect = float4::ZERO;
	float4 SnatchStartPosition = float4::ZERO;
	float4 SnatchEndPosition = float4::ZERO;
	float SnatchTime = 0.0f;
	bool IsSnatch = false;

	void AllDirectSetting();
	void PushDirectSetting();
	void SnatchCalculation();
	void BusterCalculation(float4 _attachposition);
	void BusterEnd();
	void MonsterSnatch(float _DeltaTime);
	
	void SetAdvance(float _Value)
	{
		PhysXCapsule->SetPush(ForwardDirect * _Value);
	}

	void SetThrowback(float _Value)
	{
		PhysXCapsule->SetPush(BackDirect * _Value);
	}

	void SetPush(float _Value)
	{
		SetMoveStop();
		PhysXCapsule->SetPush(PushDirect * _Value);
	}

	void SetAir(float _Value)
	{
		PhysXCapsule->SetAirState(_Value);
	}

	void SetForwardMove(float _Value)
	{
		PhysXCapsule->SetMove(ForwardDirect * _Value);
	}

	void SetBackMove(float _Value)
	{
		PhysXCapsule->SetMove(BackDirect * _Value);
	}

	void SetRightMove(float _Value)
	{
		PhysXCapsule->SetMove(RightDirect * _Value);
	}

	void SetLeftMove(float _Value)
	{
		PhysXCapsule->SetMove(LeftDirect * _Value);
	}

	void SetMoveStop()
	{
		PhysXCapsule->SetLinearVelocityZero();
	}
	//====================================================
	// TimeScale 
	// �Էµ� DamageType�� ���� �����ð����� Ÿ�ӽ������� ����ϴ�
	void HitStop(DamageType _Type);
	// �����ð����� Ÿ�ӽ������� 0���� �մϴ� _Time �� �� Ÿ�ӽ������� 1�� �˴ϴ�. CapsuleCollision�� Off�˴ϴ�
	void StopTime(float _Time);

	//====================================================
	

	//====================================================

private:

};
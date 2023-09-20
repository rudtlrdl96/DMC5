#pragma once
#include <GameEngineCore/GameEngineFSM.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>


enum class EnemyCode
{
	//�ʱ�ȭ��
	None=100000,
	// �Ϲ� ����
	Empusa = 0,
	GreenEmpusa = 1,
	RedEmpusa = 2,
	HellCaina = 1000,
	HellAntenora = 1001,

	// ���� ����
	CavliereAngelo = 5000,
};

enum class EnemyType
{
	None,
	Normal,
	Fly,
	Boss,
};

enum class EnemySize
{
	None,
	Small,
	Middle,
	Large,
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

class BaseEnemyActor : public GameEngineActor, public GameEngineNetObject
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
		EnemyFSM_Client.ChangeState(_StateValue);
	}

	// ���� ���Ͱ� ���۾Ƹ� �������� ��ȯ�մϴ�. ���� ���۾Ƹ� ���¶�� �׷�, ������ ���ݿ� �鿪�̵˴ϴ�.
	inline bool IsSuperArmor() const
	{
		return IsSuperArmorValue;
	}

	// ������ ������ ��ȯ�մϴ�.
	inline EnemyCode GetEnemyCode() const
	{
		return EnemyCodeValue;
	}

	// ������ Ÿ���� ��ȯ�մϴ�. Normal,Fly, Boss �� ���� Ÿ���� �ֽ��ϴ�.
	inline EnemyType GetEnemyType() const
	{
		return EnemyTypeValue;
	}

	// ������ ũ�⸦ ��ȯ�մϴ�.
	inline EnemySize GetEnemySize() const
	{
		return EnemySizeValue;
	}

	void StartRenderShaking(short _ShakingMaxCount)
	{
		ShakingMaxCount = _ShakingMaxCount;
		IsRenderShaking = true;
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

	std::shared_ptr<class GameEngineFBXRenderer> EnemyRenderer = nullptr;     // ������
	std::shared_ptr<PhysXCapsuleComponent> PhysXCapsule = nullptr;            // ������ ������Ʈ
	std::shared_ptr<class GameEngineCollision> MonsterCollision = nullptr;    // ���� ��ü �ݸ���
	std::shared_ptr<class AttackCollision> MonsterAttackCollision = nullptr;  // ���� ���� �ݸ���
	std::shared_ptr<class GameEngineCollision> MonsterAttackRange = nullptr;  // ���� ���� ���� �ݸ���
	std::shared_ptr<class GameEngineCollision> RN_MonsterCollision = nullptr; // ���� �ν� ���� �ݸ���

	GameEngineFSM EnemyFSM;        // �̱�, ������ FSM
	GameEngineFSM EnemyFSM_Client; // Ŭ���̾�Ʈ�� FSM (��Ŷ ���� �޾Ƽ� �����)

	//�������� ����������ϴ� Data��=====================================================
	EnemyCode EnemyCodeValue = EnemyCode::None;
	EnemyType EnemyTypeValue  = EnemyType::None;
	EnemySize EnemySizeValue = EnemySize::None;

	//HP
	float EnemyHP = 0.0f;
	//Recognize(�νĹ���)
	float4 RN_Range = float4::ZERO;
	bool RN_Player = false;
	//���ݹ���
	float4 AttackRange = float4::ZERO;
	float MoveSpeed = 0.0f;

	// FSM ���
	int EnemyFSMValue = -1;
	//=================================================================================

	void SuperArmorOn();
	void SuperArmorOff();
	bool IsSuperArmorValue = false;

	std::function<void()> SuperArmorOn_Callback = nullptr;
	std::function<void()> SuperArmorOff_Callback = nullptr;
	
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

	void AllDirectSetting();
	void PushDirectSetting();

	void SetPush(float _Value)
	{
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

	//====================================================
	// ���� ��Ŷ ����
	float4 Server_PrePosition = float4::ZERO;
	float4 Server_NextPosition = float4::ZERO;
	float4 Server_Rotation = float4::ZERO;
	float Sever_Timer = 0.0f;

	void Update_ProcessPacket() override;
	void Update_SendPacket(float _DeltaTime) override;
	//====================================================

private:

};
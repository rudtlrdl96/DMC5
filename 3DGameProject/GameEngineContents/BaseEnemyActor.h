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
	CavaliereAngelo = 5000,
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

	void MinusEnemyHP(int _Value)
	{
		EnemyHP -= _Value;
	}

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

	virtual void EnemyTypeLoad() = 0;                                 // �⺻ ���� �Է� (HP, EnemyCode)
	virtual void EnemyMeshLoad() = 0;                                 // �Ž� �ε�, �ִϸ��̼� ���� �ǽ��Ѵ�.
	virtual void EnemyAnimationLoad() = 0;                            // �ִϸ��̼� �ε�, �Ž� �ε� ���� �ǽ��Ѵ�.
	virtual void EnemyCreateFSM() = 0;                                // ����, �̱ۿ� FSM ����
	virtual void EnemyCreateFSM_Client() = 0;                         // Ŭ���̾�Ʈ�� FSM ����
	virtual void DamageCollisionCheck(float _DeltaTime) = 0;          // ����, �̱ۿ��� ó���� ������ �ݸ��� üũ �Լ�
	virtual void DamageCollisionCheck_Client(float _DeltaTime) = 0;   // Ŭ���̾�Ʈ���� ó���� ������ �ݸ��� üũ �Լ�
	virtual void RecognizeCollisionCheck(float _DeltaTime) = 0;
	virtual void DeathCheck() = 0;

	std::shared_ptr<class GameEngineFBXRenderer> EnemyRenderer = nullptr;         // ������
																			      
	std::shared_ptr<class GameEngineFBXRenderer> LeftWeapon = nullptr;            // ���Ⱑ ������ �̿�
	std::shared_ptr<class GameEngineFBXRenderer> RightWeapon = nullptr;           // ���Ⱑ ������ �̿�
																			      
	std::shared_ptr<class PhysXCapsuleComponent> PhysXCapsule = nullptr;          // ������ ������Ʈ
	std::shared_ptr<class GameEngineCollision> MonsterCollision = nullptr;        // ���� ��ü �ݸ���
	std::shared_ptr<class AttackCollision> MonsterAttackCollision = nullptr;      // ���� ���� �ݸ���
	std::shared_ptr<class AttackCollision> MonsterAttackCollision_Two = nullptr;  // ���� ���� �ݸ���
	std::shared_ptr<class GameEngineCollision> RN_MonsterCollision = nullptr;     // ���� ���� ���� �ν� �ݸ���

	//�������� ����������ϴ� Data��=====================================================
	EnemyCode EnemyCodeValue = EnemyCode::None;
	//HP
	int EnemyHP = 0;
	// FSM ���
	int EnemyFSMValue = -1;

	bool DeathValue = false;
	bool DeathSettig = false;
	bool IsBurn = false;
	float DeathColor = 0.0f;

	void RendererBurn(float _DeltaTime);
	//=================================================================================

	//====================================================
	// �÷��̾� ����
	void PlayerCheckInit();                                                          // ���� ���� ���� �� �÷��̾� ������ class BasePlayerActor* Player�� ����
	bool PlayerContactCheck(float _DeltaTime, GameEngineCollision* _Collision);      // RN_MonsterCollision�� �浹�� �浹ü(�÷��̾�) ������ class BasePlayerActor* Player�� ����
	bool PlayerContactCheck_Normal(GameEngineCollision* _Collision);
	void PlayerAttackCheck(GameEngineCollision* _Collision);                         // �ڽ��� ������ �浹ü(�÷��̾�) ������ class BasePlayerActor* Player�� ����

	GameEngineFSM EnemyFSM;
	class BasePlayerActor* Player = nullptr;
	float AppearDelayTime = 0.0f;
	float ContactDelayTime = 0.0f;
	int ServerPlayerID = 0;
	bool IsClientHit = false;
	bool IsTimeStop = false;
	bool TimeStop = false;
	//====================================================

	//====================================================
	// ���� �ٴ� üũ (RayCast)
	bool FloorCheck(float _Distance);    // ���� ��ġ���� �Ʒ� �������� Raycast�� �ǽ��Ͽ� �ٴ�(������ �浹ü)�� üũ�Ѵ�.
	float FallDistance = 0.0f;           // FloorCheck() �� �ʿ�, ������ ���� Start �κп��� �� ����
	//====================================================

	//====================================================
	// ���� ����ũ
	float4 CurRenderPosition = float4::ZERO;
	bool IsRenderShaking = false;
	float RenderShakeTime = 0.0f;
	short ShakingCameraSetting = 0;
	short ShakingCount = 0;
	short ShakingMaxCount = 0;

	void RenderShake(float _DeltaTime);  // ���۾Ƹ� ���¿��� ��Ʈ �� �������� �¿�� ����.
	//====================================================

	//====================================================
	// Slerp ��꿡 ���
	EnemyRotation EnemyRotationValue = EnemyRotation::Forward;  // ���Ͱ� � �������� ȸ���ؾ� �ϴ����� ���� ��
	EnemyHitDirect EnemyHitDirValue = EnemyHitDirect::Forward;  //

	float4 CurRotation = float4::ZERO;                          // Slerp ���� �� ���� ����
	float4 GoalRotation = float4::ZERO;                         // Slerp ���� �� ��ǥ�� �ϴ� ����
	float4 RotationValue = float4::ZERO;                        // Slerp���� �� ���� �����(���� ����)
	float4 SnatchStartPosition = float4::ZERO;                  // Snatch ���� �� ���� ��ġ
	float4 SnatchEndPosition = float4::ZERO;				    // Snatch ���� �� ��ǥ�� �ϴ� ��ġ
	float SnatchTime = 0.0f;								    // Snatch ���� �� �ҿ�� �ð� (Lerp �ð�)
	float SlerpTime = 0.0f;									    // Slerp ���� �� �ҿ�� �ð� (Slerp �ð�)
	float DotProductValue = 0.0f;                               // MonsterAndPlayerDotProductDegree() ���� �� ���� �����
	bool IsSnatch = false;									    // true�� ���� �� Snatch �ǽ�, ������ false�� �����
	bool AnimationTurnStart = false;						    // true�� ���� �� �������� �� ���� �� RotationCheck()�� �ǽ����� ����, ��Ʈ �� false�� ����Ǹ� ���͸� �ش� ��ġ�� �ٷ� ȸ����Ŵ

	float4 MonsterAndPlayerCross();			         // Player�� ���� ���� ������ ��ġ�� �����մϴ�. (���� �� ������ �����ϱ� ����)
	float MonsterAndPlayerDotProduct();              // Player�� ���� ���� ������ ��ġ�� �����մϴ�. (�ش� �������� ��� ȸ���ؾ� �ϴ��� �˱� ����)
	float MonsterAndPlayerDotProductDegree();        // Player�� ���� ���� ������ ��ġ�� �����մϴ�. Degree�� ���� ��ȯ���ݴϴ�.
	void RotationCheck();            		         // Player�� ���� ���� ������ ��ġ�� ����, ������ �� �� � �������� ȸ���ؾ��ϴ����� EnemyRotation EnemyRotationValue�� �����մϴ�.
	void AttackDirectCheck();                        // Player�� ���� ���� ������ ��ġ�� ����, ������ �� �� � ���⿡�� ���� �޾Ҵ����� EnemyHitDirect EnemyHitDirValue�� �����մϴ�.
	void SlerpCalculation();				         // Slerp�� ȸ���ؾ� �ϴ� ��쿡 ���� ���� ����մϴ�. ��� ����� float4 CurRotation, float4 GoalRotation�� ����˴ϴ�. Y�࿡ ���� ȸ���� �����ϴ�.
	void SlerpTurn(float _DeltaTime);                // ���� �ð� ���� SlerpCalculation()()�� ����  ���� ���� ���͸� Slerp��Ų��. ���� �ǽð����� float4 RotationValue�� ����ȴ�.
	void BusterCalculation(float4 _attachposition);  // ������ ��Ʈ �� ���͸� Player FBX Bone�� � ��ġ�� �������� �����Ѵ�.
	void BusterEnd();                                // ������ ���� �� ���� �������� ��ġ�� ���� ��ġ�� �ǵ�����.
	void SnatchCalculation();                        // ����ġ ��Ʈ �� Lerp�� ���Ϳ� Player�� ��ġ���� float4 SnatchStartPosition, float4 SnatchEndPosition�� ���� �����Ѵ�.
	void MonsterSnatch(float _DeltaTime);            // ���� �ð�(1.0f) ���� SnatchCalculation()�� ���� ���� ���� ���͸� Lerp��Ų��.
	//====================================================

	//====================================================
	// TimeScale Action
public:
	void HitStop(DamageType _Type);  // �Էµ� DamageType�� ���� �����ð����� Ÿ�ӽ������� ����ϴ�
	void StopTime(float _DeltaTime); // �����ð����� Ÿ�ӽ������� 0���� �մϴ�. _Time �� �� Ÿ�ӽ������� 1�� �˴ϴ�. CapsuleCollision�� Off�˴ϴ�
	//====================================================
protected:
	//====================================================
	// ���� ����
	float4 ForwardDirect = float4::ZERO;  // ������ ����
	float4 BackDirect = float4::ZERO;     // ������ �Ĺ�
	float4 RightDirect = float4::ZERO;	  // ������ ����
	float4 LeftDirect = float4::ZERO;	  // ������ ����
	float4 PushDirect = float4::ZERO;	  // ������ ���ư� ����(�Ĺ�)

	void AllDirectSetting();              // ������ ���� ��ġ�� ������ ���� ����, �Ĺ�, ����, ������ ���� �븻 ���Ͱ��� �������ش�. Phsysics�� Engine ������ 1������ ���̳��� ������ ���͸� ���Ƿ� ȸ����Ų��.
	void AllDirectSetting_Normal();       // ������ ���� ��ġ�� ������ ���� ����, �Ĺ�, ����, ������ ���� �븻 ���Ͱ��� �������ش�.
	void PushDirectSetting();             // ������ ���� ��ġ�� ������ ���� ���ư� ������ PushDirect�� �븻���ͷ� �����Ѵ�.
	
	/// <summary>
	/// ���͸� �������� addforce �ǽ�
	/// </summary>
	void SetAdvance(float _Value)
	{
		PhysXCapsule->SetPush(ForwardDirect * _Value);
	}

	/// <summary>
	/// ���͸� �Ĺ����� addforce �ǽ� (BackDirect ���), �Ȱų� ȸ���� ���� �ڷ� �з��� �� ���
	/// </summary>
	void SetThrowback(float _Value)
	{
		SetMoveStop();
		PhysXCapsule->SetPush(BackDirect * _Value);
	}

	/// <summary>
	/// ���͸� �Ĺ����� addforce �ǽ� (PushDirect ���), ��Ʈ �� ������� �ش� ���� ���
	/// </summary>
	void SetPush(float _Value)
	{
		SetMoveStop();
		PhysXCapsule->SetPush(PushDirect * _Value);
	}

	/// <summary>
	/// ���߿��� Y+ �������� addforce �ǽ�, �ۿ� ���� ���� ����(�ӵ�)���� �ʱ�ȭ��Ų �� ����
	/// </summary>
	void SetAir(float _Value)
	{
		PhysXCapsule->SetAirState(_Value);
	}

	/// <summary>
	/// ���� �������� �̵�
	/// </summary>
	void SetForwardMove(float _Value)
	{
		PhysXCapsule->SetMove(ForwardDirect * _Value);
	}

	/// <summary>
	/// ���� �Ĺ����� �̵�
	/// </summary>
	void SetBackMove(float _Value)
	{
		PhysXCapsule->SetMove(BackDirect * _Value);
	}

	/// <summary>
	/// ���� �������� �̵�
	/// </summary>
	void SetRightMove(float _Value)
	{
		PhysXCapsule->SetMove(RightDirect * _Value);
	}

	/// <summary>
	/// ���� �������� �̵�
	/// </summary>
	void SetLeftMove(float _Value)
	{
		PhysXCapsule->SetMove(LeftDirect * _Value);
	}

	/// <summary>
	/// ��� ��, �ӵ����� 0���� ����
	/// </summary>
	void SetMoveStop()
	{
		PhysXCapsule->SetLinearVelocityZero();
	}

private:

};
#pragma once
#include <GameEngineCore/GameEngineFSM.h>
#include <GameEngineCore/PhysXCapsuleComponent.h>

///	<param name="[AttackerPos] : ">���� ������ ��ġ, ���� ZERO�� ��� ������ ������ �������� �ִϸ��̼� ���� </param>
///	<param name="[Type] : ">������ Ÿ�� ���� </param>
///	<param name="[Damage] : ">�÷��̾� ���ݷ� </param>
///	<param name="[ForceDir] : ">���� �������� ���� �ڵ����� Normalize�� ������ </param>
///	<param name="[ForcePower] : ">���� �������� ũ�� </param>
class EnemyHitData
{
public:
	float4 AttackerPos = float4::ZERO;	
	MonsterDamageType Type = MonsterDamageType::None;
	float Damage = 0.0f;
	float4 ForceDir = float4::ZERO;
	float ForcePower = 1.0f;
};


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

enum class EnemyHitDir
{
	None,
	Forward,
	Back,
	Left,
	Right,
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

	// ���� �ǰ� �Լ�(������ �� ȣ�����ֽø� �˴ϴ�)
	void MonsterHit(const EnemyHitData& _HitData);

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

	EnemyHitDir GetHitDir(const float4& _WolrdPos);
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	// Enemy �𵨸� ���� ������
	std::shared_ptr<class GameEngineFBXRenderer> EnemyRenderer = nullptr;

	// ���� ������Ʈ
	std::shared_ptr<PhysXCapsuleComponent> CapsulCol = nullptr;

	// Monster�� Collision
	//�����ν�
	std::shared_ptr<class GameEngineCollision> RN_MonsterCollision = nullptr;
	//��üCollision
	std::shared_ptr<class GameEngineCollision> MonsterCollision = nullptr;

	GameEngineFSM EnemyFSM;

	//Type+Data
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

	bool IsSuperArmorValue = false;

	std::function<void()> SuperArmorOn_Callback = nullptr;
	std::function<void()> SuperArmorOff_Callback = nullptr;

	void ChasePlayer();
	void SuperArmorOn();
	void SuperArmorOff();

	virtual void EnemyMeshLoad() = 0;
	virtual void EnemyTypeLoad() = 0;
	virtual void EnemyAnimationLoad() = 0;
	virtual void EnemyCreateFSM() = 0;

private:
	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

	void AddForceEnemy(const float4& _Dir, float _Power);
};
#pragma once
#include <GameEngineCore/GameEngineFSM.h>

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
	Normal,
	Boss,
};

enum class EnemySize
{
	Small,
	Middle,
	Large
};

enum class EnemyHitDir
{
	None,
	Forward,
	Back,
	Left,
	Right
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

	// ���� �ǰ� �Լ�
	void MonsterHit(const EnemyHitData& _HitData);

	// ���� ���Ͱ� ���۾Ƹ� �������� ��ȯ�մϴ�. ���� ���۾Ƹ� ���¶�� �׷�, ������ ���ݿ� �鿪�̵˴ϴ�.
	inline bool IsSuperArmor() const
	{
		return IsSpuerArmorValue;
	}

	// ������ ������ ��ȯ�մϴ�.
	inline EnemyCode GetEnemyCode() const
	{
		return EnemyCodeValue;
	}

	// ������ Ÿ���� ��ȯ�մϴ�. Normal, Boss �� ���� Ÿ���� �ֽ��ϴ�.
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

	GameEngineFSM EnemyFSM;

	EnemyCode EnemyCodeValue = EnemyCode::Empusa;
	EnemyType EnemyTypeValue  = EnemyType::Normal;
	EnemySize EnemySizeValue = EnemySize::Small;

	bool IsSpuerArmorValue = false;
	bool IsSuperArmorValue = false;

	std::function<void()> SpuerArmorOn_Callback = nullptr;
	std::function<void()> SpuerArmorOff_Callback = nullptr;

	void SpuerArmorOn();
	void SuperArmorOff();

	virtual void EnemyMeshLoad() = 0;
	virtual void EnemyTextureLoad() = 0;
	virtual void EnemyAnimationLoad() = 0;
	virtual void EnemyCreateFSM() = 0;

private:
	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);
};
#pragma once
#include <GameEngineCore/GameEngineFSM.h>

///	<param name="AttackerPos">���� ������ ��ġ, ���� ZERO�� ��� ������ ������ �������� �ִϸ��̼� ���� </param>
///	<param name="Type">������ Ÿ�� ���� </param>
///	<param name="Damage">�÷��̾� ���ݷ� </param>
///	<param name="ForceDir">���� �������� ���� �ڵ����� Normalize�� ������ </param>
///	<param name="ForcePower">���� �������� ũ�� </param>
class EnemyHitData
{
public:
	float4 AttackerPos = float4::ZERO;	
	MonsterDamageType Type = MonsterDamageType::None;
	float Damage = 0.0f;
	float4 ForceDir = float4::ZERO;
	float ForcePower = 1.0f;
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

class BaseEnemyActor : public GameEngineActor, public GameEngineNetObject
{
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

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	
	// Class
	GameEngineFSM EnemyFSM;

	// Enum
	EnemyType EnemyTypeValue  = EnemyType::Normal;
	EnemySize EnemySizeValue = EnemySize::Small;

	// Basic
	bool IsSpuerArmorValue = false;

	virtual void MeshLoad() = 0;
	virtual void TextureLoad() = 0;
	virtual void AnimationLoad() = 0;
	virtual void CreateFSM() = 0;

private:
	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);
};
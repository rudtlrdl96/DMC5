#pragma once
#include <GameEngineCore/GameEngineFSM.h>

class EnemyHitData
{
public:
	// �������� ���� ���� nullptr�� ��� ��ġ�� ������� �ʰ� ������ �������� ������
	GameEngineActor* AttackActor = nullptr;

	// ������ Ÿ�� ����
	MonsterDamageType Type = MonsterDamageType::None;

	// �÷��̾� ���ݷ�
	float Damage = 0.0f;

	// ���� �������� ���� �ڵ����� Normalize�� ������
	float4 ForceDir = float4::ZERO;

	// ���� �������� ũ��
	float ForcePower = 1.0f;

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

	void MonsterHit(const EnemyHitData& _HitData);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	
	virtual void MeshLoad() = 0;
	virtual void TextureLoad() = 0;
	virtual void AnimationLoad() = 0;
	virtual void CreateFSM() = 0;

private:
	void UserUpdate(float _DeltaTime);
	void ServerUpdate(float _DeltaTime);

	GameEngineFSM MonsterFSM;

};


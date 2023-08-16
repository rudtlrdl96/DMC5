#pragma once
#include <GameEngineCore/GameEngineFSM.h>

class EnemyHitData
{
public:
	// 공격자의 액터 만약 nullptr일 경우 위치를 계산하지 않고 정면을 기준으로 실행함
	GameEngineActor* AttackActor = nullptr;

	// 공격의 타입 설정
	MonsterDamageType Type = MonsterDamageType::None;

	// 플레이어 공격력
	float Damage = 0.0f;

	// 공격 물리량의 방향 자동으로 Normalize를 실행함
	float4 ForceDir = float4::ZERO;

	// 공격 물리량의 크기
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


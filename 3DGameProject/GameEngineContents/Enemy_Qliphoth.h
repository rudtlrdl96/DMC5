#pragma once
#include "Poolable.h"

enum FSM_State_Qliphoth
{
	Qliphoth_Appear,		// 등장
	Qliphoth_Idle,			// Idle
	Qliphoth_Attack,		// 공격
	Qliphoth_Damage,		// 대미지 경직
	Qliphoth_Dead,			// 사망
};

class Enemy_Qliphoth : public BaseEnemyActor, public Poolable<Enemy_Qliphoth>
{
public:
	// constructer destructer
	Enemy_Qliphoth();
	~Enemy_Qliphoth();

	// delete Function
	Enemy_Qliphoth(const Enemy_Qliphoth& _Other) = delete;
	Enemy_Qliphoth(Enemy_Qliphoth&& _Other) = delete;
	Enemy_Qliphoth& operator=(const Enemy_Qliphoth& _Other) = delete;
	Enemy_Qliphoth& operator=(Enemy_Qliphoth&& _Other) = delete;

protected:
	void Start() override;
	void EnemyMeshLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyTypeLoad() override;
	void EnemyCreateFSM() override;
	void EnemyCreateFSM_Client() override;
	void DamageCollisionCheck(float _DeltaTime) override;
	void DamageCollisionCheck_Client(float _DeltaTime) override;
	void RecognizeCollisionCheck(float _DeltaTime) override;

	void SlerpTurn(float _DeltaTime);
	void DeathCheck();
private:
	void ChangeState(int _StateValue);
	void ChangeState_Client(int _StateValue);

	std::shared_ptr<class GameEngineCollision> ColCheck();
	
	std::shared_ptr<class GameEngineCollision> MonsterCollision_0 = nullptr;
	std::shared_ptr<class GameEngineCollision> MonsterCollision_1 = nullptr;
	std::shared_ptr<class GameEngineCollision> MonsterCollision_2 = nullptr;
	std::shared_ptr<class GameEngineCollision> MonsterCollision_3 = nullptr;
	float AttackDelayCheck = 0.0f;
	bool IsRecognize = false;


};


#pragma once

class Enemy_Empusa : public BaseEnemyActor
{
public:
	// constrcuter destructer
	Enemy_Empusa();
	~Enemy_Empusa();

	// delete Function
	Enemy_Empusa(const Enemy_Empusa& _Other) = delete;
	Enemy_Empusa(Enemy_Empusa&& _Other) noexcept = delete;
	Enemy_Empusa& operator=(const Enemy_Empusa& _Other) = delete;
	Enemy_Empusa& operator=(Enemy_Empusa&& _Other) noexcept = delete;
	void RN_Idle();
	void Move(float _DeltaTime);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void EnemyMeshLoad() override;
	void EnemyTypeLoad() override;
	void EnemyAnimationLoad() override;
	void EnemyCreateFSM() override;
	void DamageCollisionCheck() override;

private:
	bool CheckBool = false;

	float MoveSpeed = 50.0f;

	std::vector<std::string> Vec_AttackName;
	int AttackValue = 0;
};


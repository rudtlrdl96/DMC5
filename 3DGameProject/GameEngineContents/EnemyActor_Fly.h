#pragma once

// Ό³Έν :
class EnemyActor_Fly
{
public:
	// constrcuter destructer
	EnemyActor_Fly();
	~EnemyActor_Fly();

	// delete Function
	EnemyActor_Fly(const EnemyActor_Fly& _Other) = delete;
	EnemyActor_Fly(EnemyActor_Fly&& _Other) noexcept = delete;
	EnemyActor_Fly& operator=(const EnemyActor_Fly& _Other) = delete;
	EnemyActor_Fly& operator=(EnemyActor_Fly&& _Other) noexcept = delete;

protected:

private:

};


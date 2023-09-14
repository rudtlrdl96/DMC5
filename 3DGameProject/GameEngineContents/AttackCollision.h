#pragma once
#include <GameEngineCore/GameEngineComponent.h>

class AttackColData
{

};

// Ό³Έν :
class AttackCollision : public GameEngineComponent
{
public:
	// constrcuter destructer
	AttackCollision();
	~AttackCollision();

	// delete Function
	AttackCollision(const AttackCollision& _Other) = delete;
	AttackCollision(AttackCollision&& _Other) noexcept = delete;
	AttackCollision& operator=(const AttackCollision& _Other) = delete;
	AttackCollision& operator=(AttackCollision&& _Other) noexcept = delete;

protected:

private:
	std::shared_ptr<class GameEngineCollision> Collision = nullptr;

};


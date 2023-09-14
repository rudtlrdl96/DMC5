#pragma once
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"

// Ό³Έν :
class AttackCollision : public GameEngineCollision
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

	DamageType GetDamageType()
	{
		return DamageTypeValue;
	}

	int GetDamage()
	{
		return DamageValue;
	}

	void SetAttackData(DamageType _DamageType, int _DamageValue)
	{
		DamageTypeValue = _DamageType;
		DamageValue = _DamageValue;
	}

protected:

private:
	DamageType DamageTypeValue = DamageType::None;
	int DamageValue = 0;
};


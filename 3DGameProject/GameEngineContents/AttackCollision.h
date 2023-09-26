#pragma once
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"

class DamageData
{
public:
	DamageType DamageTypeValue = DamageType::None;
	int DamageValue = 0;
};

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


	const DamageData& GetDamage()
	{
		if (nullptr != CallBack)
		{
			CallBack();
		}
		return Data;
	}

	void SetAttackData(DamageData _Data, std::function<void()> _CallBack = nullptr)
	{
		Data = _Data;
		CallBack = _CallBack;
	}

	void SetAttackData(DamageType _Type, int _DamageValue, std::function<void()> _CallBack = nullptr)
	{
		SetAttackData({ .DamageTypeValue = _Type, .DamageValue = _DamageValue }, _CallBack);
	}
	void SetAttackType(int _DamageType)
	{
		Data.DamageTypeValue = (DamageType)_DamageType;
	}
protected:

private:
	DamageData Data;
	std::function<void()> CallBack = nullptr;
};


#pragma once
#include <GameEngineCore/GameEngineCollision.h>
#include "ContentsEnum.h"

class DamageData
{
public:
	DamageType DamageTypeValue = DamageType::None;
	int DamageValue = 0;
};

// ���� :
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

	// �浹üũ�� �׻� ����Ǵ� �ݹ��Լ��� ����
	void SetHitStopCallBack(std::function<void(float)> _CallBack)
	{
		CallBack_HitStop = _CallBack;
	}

	const DamageData& GetDamage()
	{
		if (nullptr != CallBack)
		{
			CallBack();
		}
		if (nullptr != CallBack_HitStop)
		{
			CallBack_HitStop(HitStopTime);
		}
		return Data;
	}

	void SetAttackData(DamageData _Data, std::function<void()> _CallBack = nullptr, float _HitStopTime = 0)
	{
		Data = _Data;
		CallBack = _CallBack;
		HitStopTime = _HitStopTime;
	}

	void SetAttackData(DamageType _Type, int _DamageValue, std::function<void()> _CallBack, float _HitStopTime)
	{
		SetAttackData({ .DamageTypeValue = _Type, .DamageValue = _DamageValue }, _CallBack, _HitStopTime);
	}
	void SetAttackData(DamageType _Type, int _DamageValue, std::function<void()> _CallBack)
	{
		SetAttackData({ .DamageTypeValue = _Type, .DamageValue = _DamageValue }, _CallBack, 0);
		switch (_Type)
		{
		case DamageType::None:
			HitStopTime = 0.0f;
			break;
		case DamageType::VergilLight:
			HitStopTime = 0.01f;
			break;
		case DamageType::Light:
			HitStopTime = 0.05f;
			break;
		case DamageType::Heavy:
			HitStopTime = 0.08f;
			break;
		case DamageType::Air:
			HitStopTime = 0.05f;
			break;
		case DamageType::Snatch:
			HitStopTime = 0.0f;
			break;
		case DamageType::Slam:
			HitStopTime = 0.08f;
			break;
		case DamageType::Buster:
			HitStopTime = 0.0f;
			break;
		case DamageType::Stun:
			HitStopTime = 0.0f;
			break;
		default:
			break;
		}
	}
	void SetAttackData(DamageType _Type, int _DamageValue, float _HitStopTime)
	{
		SetAttackData({ .DamageTypeValue = _Type, .DamageValue = _DamageValue }, nullptr, _HitStopTime);
	}
	void SetAttackData(DamageType _Type, int _DamageValue)
	{
		SetAttackData({ .DamageTypeValue = _Type, .DamageValue = _DamageValue }, nullptr, 0);
		switch (_Type)
		{
		case DamageType::None:
			HitStopTime = 0.0f;
			break;
		case DamageType::VergilLight:
			HitStopTime = 0.01f;
			break;
		case DamageType::Light:
			HitStopTime = 0.05f;
			break;
		case DamageType::Heavy:
			HitStopTime = 0.08f;
			break;
		case DamageType::Air:
			HitStopTime = 0.05f;
			break;
		case DamageType::Snatch:
			HitStopTime = 0.0f;
			break;
		case DamageType::Slam:
			HitStopTime = 0.08f;
			break;
		case DamageType::Buster:
			HitStopTime = 0.0f;
			break;
		case DamageType::Stun:
			HitStopTime = 0.0f;
			break;
		default:
			break;
		}
	}
protected:

private:
	DamageData Data;
	std::function<void()> CallBack = nullptr;
	std::function<void(float)> CallBack_HitStop = nullptr;
	float HitStopTime = 0;
};


#pragma once
#include "NetworkObjectBase.h"
#include "SoundController.h"
// ���� :



class BasePlayerActor : public NetworkObjectBase
{
public:
	static std::vector<BasePlayerActor*>& GetPlayers()
	{
		return Players;
	}
	static BasePlayerActor* GetMainPlayer()
	{
		return MainPlayer;
	}
	
	// constrcuter destructer
	BasePlayerActor();
	~BasePlayerActor();

	// delete Function
	BasePlayerActor(const BasePlayerActor& _Other) = delete;
	BasePlayerActor(BasePlayerActor&& _Other) noexcept = delete;
	BasePlayerActor& operator=(const BasePlayerActor& _Other) = delete;
	BasePlayerActor& operator=(BasePlayerActor&& _Other) noexcept = delete;

	void SetCutScene(const float4& _StartPos, const float4& _EndPos, const float4& _StartRot, const float4& _EndRot, float _Time);

	void On() override;
	void Off() override;

	void LookDir(const float4& _LookDir);
	void LookTarget();
	void RotationToDir(const float4& _Dir , float _MaxValue = 360.0f);
	void RotationToTarget(float _MaxValue = 360.0f);
	void RotationToMoveVector(float _MaxValue = 360.0f);

	GameEngineTransform* LockOnEnemyTransform = nullptr;

	std::shared_ptr<class PhysXCapsuleComponent> GetPhysXComponent()
	{
		return PhysXCapsule;
	}

	std::shared_ptr<class GameEngineFBXRenderer> GetPlayerRenderer()
	{
		return Renderer;
	}

	std::shared_ptr<class AttackCollision> GetAttackCollision()
	{
		return Col_Attack;
	}

	void SetForce(float4 _Value);
	void SetPush(float4 _Value);
	void SetMove(float4 _Value);
	void SetWorldPosition(float4 _Value);

	// FSM���¸� �����ϴ� �Լ�
	virtual void SetFSMStateValue(int _StateValue) = 0;
	// DT ���� ��ȯ
	virtual void SetDT(unsigned int _DTValue) = 0;
	// Arm ���� ��ȯ
	virtual void SetArm(unsigned int _ArmValue) {}

	virtual void AddDTGauge(float _Value) = 0;


	void SetInvincibility(float _Time);

	void SetBossCam(GameEngineTransform* _BossCam);
protected:
	void Start() override;
	virtual void ShopOn() {}
	virtual void ShopOff() {}
	virtual void UIOn();
	virtual void UIOff();
	void NetControllLoad();
	void UserControllLoad();

	
	// BasePlayerActor�� ������Ʈ. ��ӹ��� Actor�� Update_Character�� �������̵��Ͽ� ����ϼ���.
	void Update(float _DeltaTime) override;
	// BasePlayerActor�� ��ӹ��� ĳ���Ͱ� �̿��� Update
	virtual void Update_Character(float _DeltaTime) {}

	void LockOn();
	void LockOff();

	bool FloorCheck();
	void SetFloorPos();

	virtual void LightDamage() {}
	virtual void HeavyDamage() {}

	void StopTime(float _Time);

	GameEngineTimeEvent TimeEvent;
	SoundController	Sound;

	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PlayerController> Controller = nullptr;
	std::shared_ptr<class PlayerCamera> Camera = nullptr;

	std::shared_ptr<class GameEngineCollision> Col_Player = nullptr;
	std::shared_ptr<class GameEngineCollision> Col_EnemyStepCheck = nullptr;
	std::shared_ptr<class AttackCollision> Col_Attack = nullptr;
	std::shared_ptr<class GameEngineCollision> Col_LockOn = nullptr;

	std::shared_ptr<class PhysXCapsuleComponent> PhysXCapsule = nullptr;
	std::shared_ptr<class FXSystem> EffectSystem = nullptr;
	std::shared_ptr<class FXSystem> DTOffEffect = nullptr;
	std::shared_ptr<class EffectRenderer> LockOnRenderer = nullptr;
	std::shared_ptr<class PlayerHPUI> HUD = nullptr;
	class BaseEnemyActor* LockOnEnemy = nullptr;

	std::shared_ptr<class BaseShopUI> Shop = nullptr;
	float4 Rot = float4::ZERO;

	const int MaxHP = 10000;
	int HP = 10000;
	int FSMValue = -1;
	int ArmValue;
	float DTGauge = 0.0f;
	bool DTValue = false;
	bool IsBossBattle = false;
	bool IsEvade = false;
	bool IsShopOn = false;
	bool IsDeath = false;
private:
	void DamageColCheck();
	void OrbColCheck();
	void ShopColCheck();
	void SetLockOnMark();
	static std::vector<BasePlayerActor*> Players;
	static BasePlayerActor* MainPlayer;
	float InvincibilityTime = 0.0f; // ���� ������
};



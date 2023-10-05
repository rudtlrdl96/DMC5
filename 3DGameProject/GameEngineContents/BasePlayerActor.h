#pragma once
#include "NetworkObjectBase.h"
// ���� :



class BasePlayerActor : public GameEngineActor, public NetworkObjectBase
{
public:
	static std::vector<BasePlayerActor*>& GetPlayers()
	{
		return Players;
	}
	
	// constrcuter destructer
	BasePlayerActor();
	~BasePlayerActor();

	// delete Function
	BasePlayerActor(const BasePlayerActor& _Other) = delete;
	BasePlayerActor(BasePlayerActor&& _Other) noexcept = delete;
	BasePlayerActor& operator=(const BasePlayerActor& _Other) = delete;
	BasePlayerActor& operator=(BasePlayerActor&& _Other) noexcept = delete;

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
	void SetInvincibility(float _Time);

protected:
	void Start() override;

	void NetControllLoad();
	void UserControllLoad();

	
	// BasePlayerActor�� ������Ʈ. ��ӹ��� Actor�� Update_Character�� �������̵��Ͽ� ����ϼ���.
	void Update(float _DeltaTime) override;
	// BasePlayerActor�� ��ӹ��� ĳ���Ͱ� �̿��� Update
	virtual void Update_Character(float _DeltaTime) {}

	void LockOn();
	void LockOff();

	bool FloorCheck();

	virtual void LightDamage() {}
	virtual void HeavyDamage() {}

	GameEngineTimeEvent TimeEvent;

	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PlayerController> Controller = nullptr;
	std::shared_ptr<class PlayerCamera> Camera = nullptr;

	std::shared_ptr<class GameEngineCollision> Col_Player = nullptr;
	std::shared_ptr<class GameEngineCollision> Col_EnemyStepCheck = nullptr;
	std::shared_ptr<class AttackCollision> Col_Attack = nullptr;
	std::shared_ptr<class GameEngineCollision> Col_LockOn = nullptr;

	std::shared_ptr<class PhysXCapsuleComponent> PhysXCapsule = nullptr;
	float4 Rot = float4::ZERO;

	int HP = 10000;
	int FSMValue = -1;
	int ArmValue;
	bool DTValue = false;
private:
	void DamageColCheck();

	static std::vector<BasePlayerActor*> Players;
	float InvincibilityTime = 0.0f; // ���� ������
};


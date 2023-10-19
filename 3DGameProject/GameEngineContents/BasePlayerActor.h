#pragma once
#include "NetworkObjectBase.h"
#include "SoundController.h"
// 설명 :



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

	std::shared_ptr<class AttackCollision> GetAttackCollision()
	{
		return Col_Attack;
	}

	void SetForce(float4 _Value);
	void SetPush(float4 _Value);
	void SetMove(float4 _Value);
	void SetWorldPosition(float4 _Value);

	// FSM상태를 지정하는 함수
	virtual void SetFSMStateValue(int _StateValue) = 0;
	// DT 상태 변환
	virtual void SetDT(unsigned int _DTValue) = 0;
	// Arm 상태 변환
	virtual void SetArm(unsigned int _ArmValue) {}

	virtual void AddDTGauge(float _Value) = 0;

	void SetInvincibility(float _Time);

	void SetBossCam(GameEngineTransform* _BossCam);
protected:
	void Start() override;

	void NetControllLoad();
	void UserControllLoad();

	
	// BasePlayerActor의 업데이트. 상속받은 Actor는 Update_Character를 오버라이드하여 사용하세요.
	void Update(float _DeltaTime) override;
	// BasePlayerActor를 상속받은 캐릭터가 이용할 Update
	virtual void Update_Character(float _DeltaTime) {}

	void LockOn();
	void LockOff();

	bool FloorCheck();

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
	float4 Rot = float4::ZERO;

	int HP = 10000;
	int FSMValue = -1;
	int ArmValue;
	float DTGauge = 0.0f;
	bool DTValue = false;
	bool IsBossBattle = false;
	bool IsEvade = false;
private:
	void DamageColCheck();
	void OrbColCheck();
	static std::vector<BasePlayerActor*> Players;
	float InvincibilityTime = 0.0f; // 무적 프레임
};


#pragma once
#include "NetworkObjectBase.h"
#include "SoundController.h"
// 설명 :



class BasePlayerActor : public NetworkObjectBase
{
public:
	static void LevelChangeClear(GameEngineLevel* _Level)
	{
		if (Players.size() == 0) { return; }
		std::vector<BasePlayerActor*>::iterator StartIter = Players.begin();
		std::vector<BasePlayerActor*>::iterator EndIter = Players.end();

		while (StartIter != EndIter)
		{
			BasePlayerActor* _Player = (*StartIter);
			// 현재 레벨과 다른 플레이어는 벡터에서 제거
			if (_Player->GetLevel()->GetName() != _Level->GetName())
			{
				StartIter = Players.erase(StartIter);
				if (Players.size() == 0) { return; }
				continue;
			}
			StartIter++;
		}
	}
	static std::vector<BasePlayerActor*>& GetPlayers()
	{
		return Players;
	}
	static BasePlayerActor* GetMainPlayer()
	{
		return MainPlayer;
	}
	static void CameraLookBoss(const float4& _Pos);

	// constrcuter destructer
	BasePlayerActor();
	~BasePlayerActor();

	// delete Function
	BasePlayerActor(const BasePlayerActor& _Other) = delete;
	BasePlayerActor(BasePlayerActor&& _Other) noexcept = delete;
	BasePlayerActor& operator=(const BasePlayerActor& _Other) = delete;
	BasePlayerActor& operator=(BasePlayerActor&& _Other) noexcept = delete;

	void SetCutScene(const float4& _StartPos, const float4& _EndPos, const float4& _StartRot, const float4& _EndRot, float _Time, bool ControllOff = true, bool _UIOff = true);

	void On() override;
	void Off() override;

	virtual void UIOn();
	virtual void UIOff();

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
	void SetWorldRotation(float4 _Value);

	// FSM상태를 지정하는 함수
	virtual void SetFSMStateValue(int _StateValue) = 0;
	// DT 상태 변환
	virtual void SetDT(unsigned int _DTValue) = 0;
	// Arm 상태 변환
	virtual void SetArm(unsigned int _ArmValue) {}

	virtual void AddDTGauge(float _Value) = 0;


	void SetInvincibility(float _Time);

	void SetBossCam(GameEngineTransform* _BossCam);
	void CameraLookTarget(const float4& _TargetPos);

protected:
	void Start() override;
	virtual void ShopOn() {}
	virtual void ShopOff() {}

	void NetControllLoad();
	void UserControllLoad();

	
	// BasePlayerActor의 업데이트. 상속받은 Actor는 Update_Character를 오버라이드하여 사용하세요.
	void Update(float _DeltaTime) override;
	// BasePlayerActor를 상속받은 캐릭터가 이용할 Update
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
	std::shared_ptr<class BaseEnemyActor> LockOnEnemy = nullptr;

	std::shared_ptr<class BaseShopUI> Shop = nullptr;
	float4 Rot = float4::ZERO;

	const int MaxHP = 10000;
	int HP = 10000;
	int FSMValue = -1;
	int ArmValue = 0;
	float DTGauge = 0.0f;
	bool DTValue = false;
	bool IsBossBattle = false;
	bool IsEvade = false;
	bool IsShopOn = false;
	bool IsDeath = false;
	
	virtual void Cheat();
	bool IsGod = false;
private:
	void DamageColCheck();
	void OrbColCheck();
	void ShopColCheck();
	void SetLockOnMark();
	static std::vector<BasePlayerActor*> Players;
	static BasePlayerActor* MainPlayer;
	float InvincibilityTime = 0.0f; // 무적 프레임
};



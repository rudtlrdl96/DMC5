#pragma once
// 설명 :



class BasePlayerActor : public GameEngineActor, public GameEngineNetObject
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

	void SetForce(float4 _Value);
	void SetPush(float4 _Value);
	void SetMove(float4 _Value);
	void SetWorldPosition(float4 _Value);

	// FSM상태를 지정하는 함수
	virtual void SetFSMStateValue(int _StateValue) = 0;



protected:
	void Start() override;

	void NetControllLoad();
	void UserControllLoad();

	//패킷을 처리하는 부분입니다. 모든 레벨의 맨 처음에 실행됩니다.
	void Update_ProcessPacket() override;
	// BasePlayerActor의 업데이트. 상속받은 Actor는 Update_Character를 오버라이드하여 사용하세요.
	void Update(float _DeltaTime) override;
	// BasePlayerActor를 상속받은 캐릭터가 이용할 Update
	virtual void Update_Character(float _DeltaTime) {}
	//업데이트 패킷을 전송하는 부분입니다. 모든 Update가 끝난 뒤에 실행됩니다
	void Update_SendPacket(float _DeltaTime) override;

	void LockOn();
	void LockOff();

	bool FloorCheck();

	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PlayerController> Controller = nullptr;
	std::shared_ptr<class PlayerCamera> Camera = nullptr;
	std::shared_ptr<class GameEngineCollision> PlayerCollision = nullptr;
	std::shared_ptr<class GameEngineCollision> AttackCollision = nullptr;
	std::shared_ptr<class GameEngineCollision> LockOnCollision = nullptr;
	std::shared_ptr<class PhysXCapsuleComponent> PhysXCapsule = nullptr;
	float4 Rot = float4::ZERO;
	int FSMValue = -1;
	bool FSMForce = false;
	bool IsLockOn = false;

private:
	static std::vector<BasePlayerActor*> Players;

	float4 Server_NextPos;
	float4 Server_PrevPos;
	float Server_Timer = 0.0f;
	const float Server_UpdateTime = 0.01f;
};


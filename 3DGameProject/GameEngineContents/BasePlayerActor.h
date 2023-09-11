#pragma once
// ���� :



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

	// FSM���¸� �����ϴ� �Լ�
	virtual void SetFSMStateValue(int _StateValue) = 0;



protected:
	void Start() override;

	void NetControllLoad();
	void UserControllLoad();

	//��Ŷ�� ó���ϴ� �κ��Դϴ�. ��� ������ �� ó���� ����˴ϴ�.
	void Update_ProcessPacket() override;
	// BasePlayerActor�� ������Ʈ. ��ӹ��� Actor�� Update_Character�� �������̵��Ͽ� ����ϼ���.
	void Update(float _DeltaTime) override;
	// BasePlayerActor�� ��ӹ��� ĳ���Ͱ� �̿��� Update
	virtual void Update_Character(float _DeltaTime) {}
	//������Ʈ ��Ŷ�� �����ϴ� �κ��Դϴ�. ��� Update�� ���� �ڿ� ����˴ϴ�
	void Update_SendPacket(float _DeltaTime) override;

	void LockOn();
	void LockOff();

	bool FloorCheck();

	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PlayerController> Controller = nullptr;
	std::shared_ptr<class PlayerCamera> Camera = nullptr;
	std::shared_ptr<class GameEngineCollision> PlayerCollision = nullptr;
	std::shared_ptr<class GameEngineCollision> EnemyStepCheckCollision = nullptr;
	std::shared_ptr<class GameEngineCollision> AttackCollision = nullptr;
	std::shared_ptr<class GameEngineCollision> LockOnCollision = nullptr;
	std::shared_ptr<class PhysXCapsuleComponent> PhysXCapsule = nullptr;
	float4 Rot = float4::ZERO;
	int FSMValue = -1;
private:
	static std::vector<BasePlayerActor*> Players;

	float4 Server_NextPos;
	float4 Server_PrevPos;
	float Server_Timer = 0.0f;
	const float Server_UpdateTime = 0.01f;
};


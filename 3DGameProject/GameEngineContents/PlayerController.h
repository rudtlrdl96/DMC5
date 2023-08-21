#pragma once
#include <queue>
class CommandRecord
{
public:
	CommandRecord(char _Key, float _Time, CommandRecord* _Front)
	{
		Key = _Key;
		Time = _Time;
		Front = _Front;
	}

	char GetKey()
	{
		return Key;
	}
	bool TimeCheck(float _LiveTime)
	{
		return 0.5f < _LiveTime - Time;
	}
	void FrontClear()
	{
		Front = nullptr;
	}
	CommandRecord* GetFront()
	{
		return Front;
	}
private:
	char Key = -1;
	float Time = -1.0f;

	CommandRecord* Front = nullptr;
};

// Ό³Έν :
class PlayerController : public GameEngineComponent
{
public:
	// constrcuter destructer
	PlayerController();
	~PlayerController();

	// delete Function
	PlayerController(const PlayerController& _Other) = delete;
	PlayerController(PlayerController&& _Other) noexcept = delete;
	PlayerController& operator=(const PlayerController& _Other) = delete;
	PlayerController& operator=(PlayerController&& _Other) noexcept = delete;

	float4 GetMoveVector()
	{
		return MoveVector;
	}
	void SetCameraTransform(GameEngineTransform* _Transform)
	{
		CameraTransform = _Transform;
	}

	char MoveVectorToChar(const float4& _Value);


protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void MoveInput();
	void InputRecord();
	void ActionInput();

private:
	float4 MoveVector;
	GameEngineTransform* PlayerTransform = nullptr;
	GameEngineTransform* CameraTransform = nullptr;
	std::queue<CommandRecord*> QCommand;


	bool InputCheck_BackFront();
	bool InputCheck_Dir(char _Dir);

public:
	// CALLBACK Sword
	std::function<void()> CallBack_BackFrontSword = nullptr;
	std::function<void()> CallBack_FrontSword = nullptr;
	std::function<void()> CallBack_BackSword = nullptr;
	std::function<void()> CallBack_Sword = nullptr;
	// CALLBACK Jump
	std::function<void()> CallBack_LeftJump = nullptr;
	std::function<void()> CallBack_RightJump = nullptr;
	std::function<void()> CallBack_Jump = nullptr;
	// CALLBACK Gun
	std::function<void()> CallBack_BackFrontGun = nullptr;
	std::function<void()> CallBack_FrontGun = nullptr;
	std::function<void()> CallBack_BackGun = nullptr;
	std::function<void()> CallBack_GunDown = nullptr;
	std::function<void()> CallBack_GunUp = nullptr;
	// CALLBACK Skill
	std::function<void()> CallBack_BackFrontSkill = nullptr;
	std::function<void()> CallBack_FrontSkill = nullptr;
	std::function<void()> CallBack_SkillDown = nullptr;
	std::function<void()> CallBack_SkillUp = nullptr;
	// CALLBACK Lock
	std::function<void()> CallBack_LockOnDown = nullptr;
	std::function<void()> CallBack_LockOnUp = nullptr;

	bool IsLockOn = false;
};


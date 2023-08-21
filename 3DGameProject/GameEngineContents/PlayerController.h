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
		return 1.0f < _LiveTime - Time;
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
	GameEngineTransform* PlayerTransform;
	GameEngineTransform* CameraTransform;
	std::queue<CommandRecord*> QCommand;
	//std::list<CommandRecord> QCommand;
	//std::linked_qu
	bool InputCheck_BackForward();
	bool InputCheck_Forward();
	bool InputCheck_Back();

	bool InputCheck_Left();
	bool InputCheck_Right();

};


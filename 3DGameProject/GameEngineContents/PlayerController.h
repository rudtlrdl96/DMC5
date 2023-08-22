#pragma once
#include <queue>
class CommandRecord
{
public:

	void AddKey(char _Key, float _Time)
	{
		if (0 < Size)
		{
			if (Keys[0] == _Key)
			{
				return;
			}
			if (Size == MaxSize)
			{
				DeleteKey();
			}
			MoveKey();
		}
		
 		Keys[0] = _Key;
		Times[0] = _Time;
		Size++;
	}

	char GetKey(int _Index)
	{
		return Keys[_Index];
	}

	void TimeCheck(float _LiveTime)
	{
		if (Size == 0) { return; }
		if (1.0f < _LiveTime - Times[Size - 1])
		{
			DeleteKey();
		}
	}

	void DeleteKey()
	{
		Keys[Size - 1] = 'n';
		Size--;
	}

	void MoveKey()
	{
		for (int i = Size; 0 < i; i--)
		{
			Keys[i] = Keys[i - 1];
			//Keys[i] = 'n';
			Times[i] = Times[i - 1];
		}
	}

	char Keys[5] = { 'n', 'n', 'n' };
	float Times[5] = { 0 };
	int Size = 0;
	const int MaxSize = 5;
private:
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
	CommandRecord Command;


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


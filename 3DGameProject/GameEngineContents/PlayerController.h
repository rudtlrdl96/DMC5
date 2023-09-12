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
		if (0.2f < _LiveTime - Times[Size - 1])
		{
			DeleteKey();
		}
	}

	void DeleteKey()
	{
		Keys[Size - 1] = 'n';
		Size--;
	}

	void ClearKey()
	{
		for (int i = 0; i < 5; i++)
		{
			Keys[i] = 'n';
			Times[i] = 0;
		}
		Size = 0;
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

// 설명 :
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

	// 벡터에 대한 방향을 8방향으로 알려주는 함수 (키패드 참조 8: 앞, 2 : 뒤)
	char MoveVectorToChar(const float4& _Value);

	// 벡터에 대한 방향을 4방향으로 알려주는 함수 (키패드 참조 8: 앞, 2 : 뒤)
	char MoveVectorToChar4(const float4& _Value);


protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void InputReset();
	void MoveInput();
	void InputRecord();
	void ActionInput();

private:
	float4 MoveVector;
	GameEngineTransform* PlayerTransform = nullptr;
	GameEngineTransform* CameraTransform = nullptr;
	class BasePlayerActor* Actor = nullptr;
	CommandRecord Command;

	// 검
	bool IsBackFrontSword = false;
	bool IsFrontSword = false;
	bool IsBackSword = false;
	bool IsSword = false;
	// 점프
	bool IsLeftJump = false;
	bool IsRightJump = false;
	bool IsJump = false;
	// 총
	bool IsBackFrontGun = false;
	bool IsFrontGun = false;
	bool IsBackGun = false;
	// 스킬
	bool IsBackFrontSkill = false;
	bool IsFrontSkill = false;
	bool IsLockOnSkill = false;
	bool IsSkill = false;

	bool IsSpecialMove = false;

	bool InputCheck_BackFront();
	bool InputCheck_Dir(char _Dir);

	bool IsLockOn = false;
public:
	// CALLBACK Lock
	std::function<void()> CallBack_LockOnDown = nullptr;
	std::function<void()> CallBack_LockOnUp = nullptr;

	
	bool GetGunUp() { return GameEngineInput::IsUp("Player_Gun"); }
	bool GetLockOnFree() { return GameEngineInput::IsFree("Player_LockOn"); }
	//bool GetJumpDown() { return GameEngineInput::IsDown("Player_Jump"); }

	bool GetIsSword() { return IsSword; }
	bool GetIsAnySword() { return IsBackFrontSword || IsFrontSword || IsBackSword || IsSword; }
	bool GetIsBackSword() { return IsBackSword; }
	bool GetIsFrontSword() { return IsFrontSword; }
	bool GetIsBackFrontSword() { return IsBackFrontSword; }
	bool GetIsAnyJump() { return IsJump || IsLeftJump || IsRightJump; }
	bool GetIsJump() { return IsJump; }
	bool GetIsLeftJump() { return IsLeftJump; }
	bool GetIsRightJump() { return IsRightJump; }
	bool GetIsSpecialMove() { return IsSpecialMove; }
	bool GetIsDevilTrigger() { return GameEngineInput::IsDown("Player_DT"); }
	bool GetIsGTBomb() { return GameEngineInput::IsDown("Player_GT_Bomb"); }
	bool GetIsProvocation() { return GameEngineInput::IsDown("Player_Provocation"); }
	bool GetIsSkill() { return IsSkill; }
	bool GetIsLockOnSkill() { return IsLockOnSkill; }
	bool GetIsLockOn() { return IsLockOn; }

};


#pragma once
#include <Windows.h>
#include <map>
#include <string>

// 설명 :
class GameEngineWindow;
class GameEngineInput
{
	friend GameEngineWindow;

private:
	class GameEngineKey 
	{
		friend GameEngineInput;

		bool Down = false; // 누른 한순간
		bool Press = false; // 계속 누르고 있다.
		bool Up = false; // 땐 한순간
		bool Free = true; // 안눌리고 있다.

		float PressTime; // 몇초간 눌렀다.
		int Key = -1;

		void Reset() 
		{
			Down = false; // 누른 한순간
			Press = false; // 계속 누르고 있다.
			Up = false; // 땐 한순간
			Free = true; // 안눌리고 있다.
		}

		bool KeyCheck() 
		{
			return 0 != GetAsyncKeyState(Key);
		}

		void Update(float _DeltaTime);
	};

public:

	// delete Function
	GameEngineInput(const GameEngineInput& _Other) = delete;
	GameEngineInput(GameEngineInput&& _Other) noexcept = delete;
	GameEngineInput& operator=(const GameEngineInput& _Other) = delete;
	GameEngineInput& operator=(GameEngineInput&& _Other) noexcept = delete;

	static void Update(float _DeltaTime);

	static void CreateKey(const std::string_view& _Name, int _Key);

	static bool IsKey(const std::string_view& _Name);

	static bool IsDown(const std::string_view& _Name);
	static bool IsUp(const std::string_view& _Name);
	static bool IsPress(const std::string_view& _Name);
	static bool IsFree(const std::string_view& _Name);
	static float GetPressTime(const std::string_view& _Name);

	static void AllReset();

	static void ShowMouseCursor(bool _Value);

	static bool IsAnyKey() 
	{
		return IsAnyKeyValue;
	}

	static float4 GetMousePosition() 
	{
		return MousePos;
	}

	static float4 GetMouseDirection()
	{
		return MouseDirection;
	}

	static float4 GetMouseDirectionNormal()
	{
		return MouseDirection.NormalizeReturn();
	}

	static void AddIgnoreKeys(std::string_view _name)
	{
		std::string Name = GameEngineString::ToUpper(_name.data());
		IgnoreKeys.insert(Name);
	}

protected:

private:
	// constrcuter destructer
	GameEngineInput();
	~GameEngineInput();

	static std::set<std::string> IgnoreKeys;

	//      PlayerJump       A
	static std::map<std::string, GameEngineKey> Keys;
	static bool IsAnyKeyValue;
	static bool IsFocus;
	static bool IsShowCursor;
	static float4 MousePos;
	static float4 PrevMousePos;
	static float4 MouseDirection;
	static float4 CenterPos;

	static void IsFocusOn()
	{
		IsFocus = true;
	}

	static void IsFocusOff()
	{
		IsFocus = false;
	}

	static void IsAnyKeyOn()
	{
		IsAnyKeyValue = true;
	}

	static void IsAnyKeyOff()
	{
		IsAnyKeyValue = false;
	}
};



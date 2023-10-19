#pragma once
#include <string>
#include <Windows.h>
#include <functional>
#include <GameEngineBase/GameEngineMath.h>

// 윈도우는 핸들 방식이라는것을 이용한다.
// 핸들방식이란 우리에게 os가 관리한다는 증명으로 숫자 1를 줍니다.
// 그 숫자를 핸들이라고 합니다.

// 설명 :
class GameEngineImage;
class GameEngineWindow
{
	static LRESULT CALLBACK MessageFunction(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	static std::function<LRESULT(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)> UserMessageFunction;

public:
	static void SetUserMessageFunction(std::function<LRESULT(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)> _UserMessageFunction)
	{
		UserMessageFunction = _UserMessageFunction;
	}

	// 윈도우를 만들어 주는 기능입니다.
	static void WindowCreate(HINSTANCE _hInstance, const std::string_view& _TitleName, float4 _Size, float4 _Pos, bool _IsFullScreen = false);

	static void SettingWindowSize(float4 _Size);
	static void SettingWindowPos(float4 _Pos);

	static float4 GetScreenSize()
	{
		return ScreenSize;
	}

	static HWND GetHWnd() 
	{
		return HWnd;
	}

	static HDC GetWindowBackBufferHdc()
	{
		return WindowBackBufferHdc;
	}

	static GameEngineImage* GetDoubleBufferImage()
	{
		return DoubleBufferImage;
	}

	static void AppOff()
	{
		IsWindowUpdate = false;
	}

	static void DoubleBufferClear();
	static void DoubleBufferRender();

	// 오직 나는 외부에서 오는게 실행시켜주기만 하면 되게 만드는것.
	// 그러면 다른 클래스나 컨텐츠와의 관련을 맺지 않고 오로지 시키는 일을 하는 클래스가 되는것
	// 남의 함수를 대신 실행시켜주는 이 함수포인터를 이용한 방식을 callback 방식이라고 합니다.
	// void(*Start)(), void(*Loop)(), void(*End)() 외부에서 함수포인터를 맡기는 방식.
	// => 컨텐츠와 기능을 분리하기 위해서
	static int WindowLoop(
		std::function<void()> _Start, 
		std::function<void()> _Loop, 
		std::function<void()> _End
	);

	static float4 GetMousePosition();

	static void Release();

	GameEngineWindow();
	~GameEngineWindow();

	// delete Function
	GameEngineWindow(const GameEngineWindow& _Other) = delete;
	GameEngineWindow(GameEngineWindow&& _Other) noexcept = delete;
	GameEngineWindow& operator=(const GameEngineWindow& _Other) = delete;
	GameEngineWindow& operator=(GameEngineWindow&& _Other) noexcept = delete;

	

protected:

private:
	static float4 WindowSize;
	static float4 ScreenSize;
	static float4 WindowPos;
	static HWND HWnd;
	static HDC WindowBackBufferHdc; // 윈도우에 그림을 그릴수 있는 권한.
	static GameEngineImage* BackBufferImage;
	static GameEngineImage* DoubleBufferImage;
	static bool IsWindowUpdate;
	static WNDCLASSEX wcex;
};


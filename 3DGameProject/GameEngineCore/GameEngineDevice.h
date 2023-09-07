#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEnginePlatform/GameEngineWindow.h>

// 설명 :
class GameEngineDevice
{
public:
	// delete Function
	GameEngineDevice(const GameEngineDevice& _Other) = delete;
	GameEngineDevice(GameEngineDevice&& _Other) noexcept = delete;
	GameEngineDevice& operator=(const GameEngineDevice& _Other) = delete;
	GameEngineDevice& operator=(GameEngineDevice&& _Other) noexcept = delete;

	static void Initialize();
	static void CreateSwapChain();
	static void RenderStart();
	static void RenderEnd();
	static void VidioRenderStart();
	static void VidioRenderEnd();
	static void Release();

	static ID3D11Device* GetDevice()
	{
		return Device;
	}

	static ID3D11DeviceContext* GetContext()
	{
		return Context;
	}

	static std::shared_ptr<class GameEngineRenderTarget> GetBackBufferTarget()
	{
		return BackBufferTarget;
	}

	static IDXGISwapChain* GetSwapChain()
	{
		return SwapChain;
	}

	// HWND
protected:

private:
	// 메인 랜더 타겟
	static std::shared_ptr<class GameEngineRenderTarget> BackBufferTarget;

	// 다이렉트9에서는 ID3D9Device 하나에 모든 기능을 지원했지만, 11부터는 ID3D11Device와 ID3D11DeviceContext로 나눴다.
	static ID3D11Device* Device;		 // 그래픽카드 RAM에 해당하는 부분에 대한 권한 담당
	static ID3D11DeviceContext* Context; // 랜더링에 관련된 연산 담당
	static IDXGISwapChain* SwapChain;    // WinAPI와 같이 화면 하나를 담당하는 역할 (== BackBufferDC)

	static IDXGIAdapter* GetHighPerformanceAdapter();
	
	GameEngineDevice();
	~GameEngineDevice();

};


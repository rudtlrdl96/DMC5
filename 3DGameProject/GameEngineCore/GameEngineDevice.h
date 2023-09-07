#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEnginePlatform/GameEngineWindow.h>

// ���� :
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
	// ���� ���� Ÿ��
	static std::shared_ptr<class GameEngineRenderTarget> BackBufferTarget;

	// ���̷�Ʈ9������ ID3D9Device �ϳ��� ��� ����� ����������, 11���ʹ� ID3D11Device�� ID3D11DeviceContext�� ������.
	static ID3D11Device* Device;		 // �׷���ī�� RAM�� �ش��ϴ� �κп� ���� ���� ���
	static ID3D11DeviceContext* Context; // �������� ���õ� ���� ���
	static IDXGISwapChain* SwapChain;    // WinAPI�� ���� ȭ�� �ϳ��� ����ϴ� ���� (== BackBufferDC)

	static IDXGIAdapter* GetHighPerformanceAdapter();
	
	GameEngineDevice();
	~GameEngineDevice();

};


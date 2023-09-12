#include "PrecompileHeader.h"
#include "GameEngineDevice.h"
#include <GameEngineBase/GameEngineDebug.h>
#include "GameEngineRenderTarget.h"

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "DXGI")

ID3D11Device* GameEngineDevice::Device = nullptr;
ID3D11DeviceContext* GameEngineDevice::Context = nullptr;
IDXGISwapChain* GameEngineDevice::SwapChain = nullptr;
std::shared_ptr<GameEngineRenderTarget> GameEngineDevice::BackBufferTarget = nullptr;

GameEngineDevice::GameEngineDevice() 
{
}

GameEngineDevice::~GameEngineDevice() 
{
}

// DirectX 11 Device ����
void GameEngineDevice::Initialize()
{
	if (nullptr == GameEngineWindow::GetHWnd())
	{
		MsgAssert("�����찡 ��������� �ʾҴµ� ����̽��� �ʰ�ȭ �Ҽ��� �����ϴ�.");
		return;
	}

	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG; // DirectX 11 ����� ��� ����

#endif
	D3D_FEATURE_LEVEL Level = D3D_FEATURE_LEVEL_11_0;
	IDXGIAdapter* Adapter = GetHighPerformanceAdapter(); // GetHighPerformanceAdapter() : �׷���ī�� ���� ��������

	if (nullptr == Adapter)
	{
		MsgAssert("�׷���ī�� ��ġ ��� �������̽��� �����µ� �����߽��ϴ�.");
		return;
	}

	// CPU�� �׷��޶� == D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_SOFTWARE
	// GPU�� �׷��޶� == D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE
	// D3D11_SDK_VERSION == �ش� �����쿡�� �����ϴ� SDK ������ Define

	// D3D11CreateDevice() == ����̽� ����
	HRESULT Result = D3D11CreateDevice(
		Adapter,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		iFlag,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&Device,
		&Level,
		&Context
	);

	if (S_OK != Result)
	{
		MsgAssert("����̽� ������ �����߽��ϴ�.");
		return;
	}

	if (nullptr != Adapter)
	{
		Adapter->Release();
		Adapter = nullptr;
	}

	// ���� ������ �Ϸ�Ǿ��ٸ�, D3D_FEATURE_LEVEL_11_0 ������ �������� ���̴�.
	if (Level != D3D_FEATURE_LEVEL_11_0)
	{
		MsgAssert("���̷�Ʈ 11�� �������� �ʴ� �׷���ī�� �Դϴ�");
		return;
	}

	// ���̷�Ʈ x���� ��Ƽ������ ����, ��Ƽ�����带 ����ϰڴٴ� ���� �ǽ�
	Result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// ������ �����(HDC)�� ����� �۾��̶�� ���� �ȴ�. ����ü�� ���� �ǽ�
	CreateSwapChain();
}

// �׷���ī�� ���� ��������
IDXGIAdapter* GameEngineDevice::GetHighPerformanceAdapter()
{
	IDXGIFactory* Factory = nullptr;
	IDXGIAdapter* Adapter = nullptr;

	// ���丮 ����, c++���� �����ϴ� Ŭ������ �����ϱ� ���� GUI�� ������ �����̴�.
	// ����̽������� ���ο� ������ �ִ� �����ͳ� �ɹ������� ��������
	// __uuidof(IDXGIAdapter) ���� GUID�� �־���� �Ѵ�.
	// �̰��� ���α׷��� ��Ʋ� �� 1���� �����ϴ� "Key"�� ����� ����̴�.
	// MIDL_INTERFACE("2411e7e1-12ac-4ccf-bd14-9798e8534dc0") << �̷� ���� ���� GUID ���̶�� ���� �ȴ�.
	HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory);

	if (nullptr == Factory)
	{
		MsgAssert("�׷���ī�忡�� ���丮 �������̽��� �������� ���߽��ϴ�.");
		return nullptr;
	}

	size_t prevAdapterVideoMemory = 0;

	for (UINT Adapterindex = 0; ; Adapterindex++)
	{
		IDXGIAdapter* CurAdapter = nullptr;

		// ���丮�� ���� ��͸� ���´�.
		Factory->EnumAdapters(Adapterindex, &CurAdapter);

		if (nullptr == CurAdapter)
		{
			break;
		}

		DXGI_ADAPTER_DESC Desc;

		// ���丮�� ���� ��͸� ���� DXGI_ADAPTER_DESC�� �� ����
		CurAdapter->GetDesc(&Desc);

		if (prevAdapterVideoMemory <= Desc.DedicatedVideoMemory)
		{
			prevAdapterVideoMemory = Desc.DedicatedVideoMemory;

			if (nullptr != Adapter)
			{
				Adapter->Release(); // ��� �� �ݵ�� Release ���ֵ��� �Ѵ�.
			}

			Adapter = CurAdapter;

			continue;
		}

		CurAdapter->Release(); // ��� �� �ݵ�� Release ���ֵ��� �Ѵ�.
	}

	Factory->Release(); // ��� �� �ݵ�� Release ���ֵ��� �Ѵ�.

	return Adapter;
}

// ����ü�� ����
void GameEngineDevice::CreateSwapChain()
{
	float4 ScreenSize = GameEngineWindow::GetScreenSize();

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {0,};

	SwapChainDesc.BufferCount = 2;                                                         // �⺻����
	SwapChainDesc.BufferDesc.Width = ScreenSize.uix();
	SwapChainDesc.BufferDesc.Height = ScreenSize.uiy();
	SwapChainDesc.OutputWindow = GameEngineWindow::GetHWnd();
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;                                  // ȭ�� ���ŷ�
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                          // �׷����̹��� ����
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                      // ���ȳ�
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT; // DXGI_USAGE_RENDER_TARGET_OUTPUT == �����쿡 ���𰡸� ���� �뵵 // DXGI_USAGE_SHADER_INPUT == ���̴��� �ش� �ɼ� ����Ѵ�.
	SwapChainDesc.SampleDesc.Quality = 0;                                                  // ��Ƽ����̾���� �ڵ����� ����
	SwapChainDesc.SampleDesc.Count = 1;                                                    // ���ȳ�
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;                          // ū �ǹ̴� ����, ȭ�� ������ ���� ���ɼ��� ���ϴ� ���ε�, �츮�� �Ⱦ� ����
	SwapChainDesc.Windowed = true;                                                         // false�� ��üȭ��

	// ���丮 ����, c++���� �����ϴ� Ŭ������ �����ϱ� ���� GUI�� ������ �����̴�.
	// ����̽������� ���ο� ������ �ִ� �����ͳ� �ɹ������� ��������
	// __uuidof(IDXGIAdapter) ���� GUID�� �־���� �Ѵ�.
	// �̰��� ���α׷��� ��Ʋ� �� 1���� �����ϴ� "Key"�� ����� ����̴�.
	// MIDL_INTERFACE("2411e7e1-12ac-4ccf-bd14-9798e8534dc0") << �̷� ���� ���� GUID ���̶�� ���� �ȴ�.

	IDXGIDevice*  SwapDevice = nullptr;
	IDXGIAdapter* SwapAdapter = nullptr;
	IDXGIFactory* SwapFactory = nullptr;

	// ����̽� �����������̽����� SwapDevice �� ����
	Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&SwapDevice));
	if (nullptr == SwapDevice)
	{
		MsgAssert("DXGI ����̽��� DirectX����̽����� ������ ���߽��ϴ�.");
		return;
	}

	// SwapDevice���� SwapAdapter �� ����
	SwapDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&SwapAdapter));
	if (nullptr == SwapAdapter)
	{
		MsgAssert("DXGI ����̽��� DirectX����̽����� ������ ���߽��ϴ�.");
		return;
	}

	// SwapAdapter���� SwapFactory ���� ������ ��, �׷κ��� ����ü���� ������
	SwapAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&SwapFactory));
	if (S_OK != SwapFactory->CreateSwapChain(Device, &SwapChainDesc, &SwapChain))
	{
		MsgAssert("����ü�� ������ �����߽��ϴ�.");
		return;
	}

	// ����� ���� Release�� �ǽ��Ͽ� �� ����
	SwapDevice->Release();
	SwapAdapter->Release();
	SwapFactory->Release();

	// ����Ÿ���� DC�� ��� ���� �ȴ�.
	ID3D11Texture2D* SwapBackBufferTexture = nullptr;

	// ����ü���� ���������� �����Ǿ��ٸ�, HRESULT�� S_OK�� ���޵ȴ�.
	HRESULT Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&SwapBackBufferTexture));
	if (S_OK != Result)
	{
		MsgAssert("����ü�� ������ �����߽��ϴ�.");
		return;
	}

	// ����ü��(�����) ���� ��, �ؽ��Ŀ��� ����Ÿ�ٺ�(RTV) ����
	std::shared_ptr<GameEngineTexture> BackBufferTexture = std::make_shared<GameEngineTexture>();
	BackBufferTexture->ResCreate(SwapBackBufferTexture);

	// ����Ÿ�ٺ� ���� ��, ���η���Ÿ�� ����
	BackBufferTarget = GameEngineRenderTarget::Create("MainBackBufferTarget", BackBufferTexture, {0.0f, 0.0f, 1.0f, 1.0f});
	BackBufferTarget->CreateDepthTexture();
}

void GameEngineDevice::RenderStart() 
{
	BackBufferTarget->Clear();
	BackBufferTarget->Setting();
}

void GameEngineDevice::RenderEnd() 
{
	HRESULT Result = SwapChain->Present(0, 0);
	if (Result == DXGI_ERROR_DEVICE_REMOVED || Result == DXGI_ERROR_DEVICE_RESET)
	{
		// ����̽� �ٽø����
		MsgAssert("����Ÿ�� ������ �����߽��ϴ�.");
		return;
	}
}

void GameEngineDevice::VidioRenderStart() 
{
	BackBufferTarget->Clear();
	BackBufferTarget->Reset();
}

void GameEngineDevice::VidioRenderEnd() 
{
	HRESULT Result = SwapChain->Present(0, 0);
}

void GameEngineDevice::Release()
{
	BackBufferTarget = nullptr;

	if (nullptr != SwapChain)
	{
		SwapChain->Release();
		SwapChain = nullptr;
	}

	if (nullptr != Context)
	{
		Context->Release();
		Context = nullptr;
	}

	if (nullptr != Device)
	{
		Device->Release();
		Device = nullptr;
	}
}
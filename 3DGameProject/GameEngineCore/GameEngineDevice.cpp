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

// DirectX 11 Device 생성
void GameEngineDevice::Initialize()
{
	if (nullptr == GameEngineWindow::GetHWnd())
	{
		MsgAssert("윈도우가 만들어지지 않았는데 디바이스를 초가화 할수는 없습니다.");
		return;
	}

	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG; // DirectX 11 디버그 기능 설정

#endif
	D3D_FEATURE_LEVEL Level = D3D_FEATURE_LEVEL_11_0;
	IDXGIAdapter* Adapter = GetHighPerformanceAdapter(); // GetHighPerformanceAdapter() : 그래픽카드 정보 가져오기

	if (nullptr == Adapter)
	{
		MsgAssert("그래픽카드 장치 어뎁터 인터페이스를 얻어오는데 실패했습니다.");
		return;
	}

	// CPU로 그려달라 == D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_SOFTWARE
	// GPU로 그려달라 == D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE
	// D3D11_SDK_VERSION == 해당 윈도우에서 지원하는 SDK 버전이 Define

	// D3D11CreateDevice() == 디바이스 생성
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
		MsgAssert("디바이스 생성에 실패했습니다.");
		return;
	}

	if (nullptr != Adapter)
	{
		Adapter->Release();
		Adapter = nullptr;
	}

	// 위의 설정이 완료되었다면, D3D_FEATURE_LEVEL_11_0 레벨이 설정됐을 것이다.
	if (Level != D3D_FEATURE_LEVEL_11_0)
	{
		MsgAssert("다이렉트 11을 지원하지 않는 그래픽카드 입니다");
		return;
	}

	// 다이렉트 x에서 멀티쓰레드 관련, 멀티스레드를 사용하겠다는 설정 실시
	Result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// 윈도우 백버퍼(HDC)를 만드는 작업이라고 보면 된다. 스왑체인 생성 실시
	CreateSwapChain();
}

// 그래픽카드 정보 가져오기
IDXGIAdapter* GameEngineDevice::GetHighPerformanceAdapter()
{
	IDXGIFactory* Factory = nullptr;
	IDXGIAdapter* Adapter = nullptr;

	// 팩토리 생성, c++에서 지원하는 클래스를 구분하기 위한 GUI를 얻어오는 과정이다.
	// 디바이스에서는 내부에 가지고 있는 포인터나 맴버변수를 얻어오려면
	// __uuidof(IDXGIAdapter) 같은 GUID를 넣어줘야 한다.
	// 이것은 프로그램을 통틀어서 단 1개만 존재하는 "Key"를 만드는 방법이다.
	// MIDL_INTERFACE("2411e7e1-12ac-4ccf-bd14-9798e8534dc0") << 이런 값이 고유 GUID 값이라고 보면 된다.
	HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&Factory);

	if (nullptr == Factory)
	{
		MsgAssert("그래픽카드에서 팩토리 인터페이스를 생성하지 못했습니다.");
		return nullptr;
	}

	size_t prevAdapterVideoMemory = 0;

	for (UINT Adapterindex = 0; ; Adapterindex++)
	{
		IDXGIAdapter* CurAdapter = nullptr;

		// 팩토리를 통해 어뎁터를 얻어온다.
		Factory->EnumAdapters(Adapterindex, &CurAdapter);

		if (nullptr == CurAdapter)
		{
			break;
		}

		DXGI_ADAPTER_DESC Desc;

		// 팩토리로 얻어온 어뎁터를 통해 DXGI_ADAPTER_DESC에 값 복사
		CurAdapter->GetDesc(&Desc);

		if (prevAdapterVideoMemory <= Desc.DedicatedVideoMemory)
		{
			prevAdapterVideoMemory = Desc.DedicatedVideoMemory;

			if (nullptr != Adapter)
			{
				Adapter->Release(); // 사용 후 반드시 Release 해주도록 한다.
			}

			Adapter = CurAdapter;

			continue;
		}

		CurAdapter->Release(); // 사용 후 반드시 Release 해주도록 한다.
	}

	Factory->Release(); // 사용 후 반드시 Release 해주도록 한다.

	return Adapter;
}

// 스왑체인 생성
void GameEngineDevice::CreateSwapChain()
{
	float4 ScreenSize = GameEngineWindow::GetScreenSize();

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {0,};

	SwapChainDesc.BufferCount = 2;                                                         // 기본정보
	SwapChainDesc.BufferDesc.Width = ScreenSize.uix();
	SwapChainDesc.BufferDesc.Height = ScreenSize.uiy();
	SwapChainDesc.OutputWindow = GameEngineWindow::GetHWnd();
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;                                  // 화면 갱신률
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                          // 그래픽이미지 포맷
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                      // 기억안남
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT; // DXGI_USAGE_RENDER_TARGET_OUTPUT == 윈도우에 무언가를 띄우는 용도 // DXGI_USAGE_SHADER_INPUT == 쉐이더도 해당 옵션 사용한다.
	SwapChainDesc.SampleDesc.Quality = 0;                                                  // 안티얼라이어싱을 자동으로 설정
	SwapChainDesc.SampleDesc.Count = 1;                                                    // 기억안남
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;                          // 큰 의미는 없음, 화면 사이즈 조정 가능성을 말하는 것인데, 우리는 안쓸 예정
	SwapChainDesc.Windowed = true;                                                         // false는 전체화면

	// 팩토리 생성, c++에서 지원하는 클래스를 구분하기 위한 GUI를 얻어오는 과정이다.
	// 디바이스에서는 내부에 가지고 있는 포인터나 맴버변수를 얻어오려면
	// __uuidof(IDXGIAdapter) 같은 GUID를 넣어줘야 한다.
	// 이것은 프로그램을 통틀어서 단 1개만 존재하는 "Key"를 만드는 방법이다.
	// MIDL_INTERFACE("2411e7e1-12ac-4ccf-bd14-9798e8534dc0") << 이런 값이 고유 GUID 값이라고 보면 된다.

	IDXGIDevice*  SwapDevice = nullptr;
	IDXGIAdapter* SwapAdapter = nullptr;
	IDXGIFactory* SwapFactory = nullptr;

	// 디바이스 쿼리인터페이스에서 SwapDevice 값 복사
	Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&SwapDevice));
	if (nullptr == SwapDevice)
	{
		MsgAssert("DXGI 디바이스를 DirectX디바이스에서 얻어오지 못했습니다.");
		return;
	}

	// SwapDevice에서 SwapAdapter 값 복사
	SwapDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&SwapAdapter));
	if (nullptr == SwapAdapter)
	{
		MsgAssert("DXGI 디바이스를 DirectX디바이스에서 얻어오지 못했습니다.");
		return;
	}

	// SwapAdapter에서 SwapFactory 값을 복사한 뒤, 그로부터 스왑체인을 생성함
	SwapAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&SwapFactory));
	if (S_OK != SwapFactory->CreateSwapChain(Device, &SwapChainDesc, &SwapChain))
	{
		MsgAssert("스왑체인 생성에 실패했습니다.");
		return;
	}

	// 사용한 것은 Release를 실시하여 릭 제거
	SwapDevice->Release();
	SwapAdapter->Release();
	SwapFactory->Release();

	// 랜더타겟은 DC의 라고 보면 된다.
	ID3D11Texture2D* SwapBackBufferTexture = nullptr;

	// 스왑체인이 정상적으로 생성되었다면, HRESULT로 S_OK가 전달된다.
	HRESULT Result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&SwapBackBufferTexture));
	if (S_OK != Result)
	{
		MsgAssert("스왑체인 생성에 실패했습니다.");
		return;
	}

	// 스왑체인(백버퍼) 생성 후, 텍스쳐에서 랜더타겟뷰(RTV) 생성
	std::shared_ptr<GameEngineTexture> BackBufferTexture = std::make_shared<GameEngineTexture>();
	BackBufferTexture->ResCreate(SwapBackBufferTexture);

	// 랜더타겟뷰 생성 후, 메인랜더타겟 생성
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
		// 디바이스 다시만들기
		MsgAssert("랜더타겟 생성에 실패했습니다.");
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
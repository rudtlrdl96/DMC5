#include "PrecompileHeader.h"
#include "GameEngineWindow.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineThread.h>
#include <GameEnginePlatform/GameEngineImage.h>
#include "GameEngineInput.h"

// LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM)

std::function<LRESULT(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)> GameEngineWindow::UserMessageFunction;
HWND GameEngineWindow::HWnd = nullptr;
HDC GameEngineWindow::WindowBackBufferHdc = nullptr;
float4 GameEngineWindow::WindowSize = {800, 600};
float4 GameEngineWindow::WindowPos = { 100, 100 };
float4 GameEngineWindow::ScreenSize = { 800, 600 };
GameEngineImage* GameEngineWindow::BackBufferImage = nullptr;
GameEngineImage* GameEngineWindow::DoubleBufferImage = nullptr;
bool GameEngineWindow::IsWindowUpdate = true;
WNDCLASSEX GameEngineWindow::wcex;


LRESULT CALLBACK GameEngineWindow::MessageFunction(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
    if (nullptr != UserMessageFunction)
    {
        if (0 != UserMessageFunction(_hWnd, _message, _wParam, _lParam))
        {
            return true;
        }
    }

    switch (_message)
    {
    case WM_SETFOCUS:
    {
        GameEngineInput::IsFocusOn();
        break;
    }
    case WM_KILLFOCUS:
    {
        GameEngineInput::IsFocusOff();
        break;
    }
    case WM_KEYDOWN:
    {
        GameEngineInput::IsAnyKeyOn();
        break;
    }
    case WM_DESTROY:
    {
        GameEngineThread::ThreadEnd();
        // Message함수가 0을 리턴하게 만들어라.
        // PostQuitMessage(0);
        IsWindowUpdate = false;
        break;
    }
    default:
        return DefWindowProc(_hWnd, _message, _wParam, _lParam);
    }

    return 0;
}

GameEngineWindow::GameEngineWindow() 
{
}

GameEngineWindow::~GameEngineWindow() 
{
}

void GameEngineWindow::WindowCreate(HINSTANCE _hInstance, const std::string_view& _TitleName, float4 _Size, float4 _Pos, bool _IsFullScreen /*= false*/)
{
    // 생성할 윈도우에 대한 설정
    {
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = &GameEngineWindow::MessageFunction;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = _hInstance;
        wcex.hIcon = nullptr;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = "GameEngineWindowDefault";
        wcex.hIconSm = nullptr;
    }

    // 위의 내용으로 윈도우 클래스를 GameEngineWindowDefault로 등록
    if (0 == RegisterClassEx(&wcex))
    {
        MsgAssert("윈도우 클래스 등록에 실패했습니다.");
        return;
    }

    // 핸들 생성
    HWnd = CreateWindow("GameEngineWindowDefault", _TitleName.data(), _IsFullScreen ? (WS_VISIBLE | WS_POPUP) : WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInstance, nullptr);

    if (nullptr == HWnd)
    {
        MsgAssert("윈도우 클래스 생성에 실패했습니다.");
        return;
    }

    WindowBackBufferHdc = GetDC(HWnd);
    if (true == _IsFullScreen)
    {
        SetWindowLong(HWnd, GWL_STYLE, 0);
    }
    ShowWindow(HWnd, SW_SHOW); // 창 표시 방법 제어, SW_SHOW는 창을 활성화하고 현재 크기와 위치에 표시
    UpdateWindow(HWnd);        // 창의 업데이트 영역이 비어 있지 않은 경우 창에 WM_PAINT 메시지를 보내 지정된 창의 클라이언트 영역을 업데이트

    SettingWindowSize(_Size);  // 생성할 윈도우 사이즈 설정
    SettingWindowPos(_Pos);    // 윈도우 생성 포지션 설정
    BackBufferImage = new GameEngineImage();
    BackBufferImage->ImageCreate(WindowBackBufferHdc);

    return;
}

void GameEngineWindow::DoubleBufferClear()
{
    DoubleBufferImage->ImageClear();
}

void GameEngineWindow::DoubleBufferRender()
{
    BackBufferImage->BitCopy(DoubleBufferImage, WindowSize.half(), WindowSize);
}

int GameEngineWindow::WindowLoop(
    std::function<void()> _Start,
    std::function<void()> _Loop,
    std::function<void()> _End
)
{
    if (nullptr != _Start)
    {
        _Start();
    }

    MSG msg = {};

    while (IsWindowUpdate)
    {
        // 비동기 함수
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {

            if (nullptr != _Loop)
            {
                _Loop();
                GameEngineInput::IsAnyKeyOff();
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        } 

        // 데드타임, 이때도 게임을 실행한다.
        if (nullptr != _Loop)
        {
            _Loop();
            GameEngineInput::IsAnyKeyOff();
        }
    }

    if (nullptr != _End)
    {
        _End();
    }

    if (nullptr != BackBufferImage)
    {
        delete DoubleBufferImage;
        DoubleBufferImage = nullptr;

        delete BackBufferImage;
        BackBufferImage = nullptr;
    }

    return (int)msg.wParam;
}

void GameEngineWindow::SettingWindowSize(float4 _Size)
{
    // 윈도우 창 설정은 타이틀바, 프레임의 크기를 고려하여 설정한다.
    //          위치      크기
    RECT Rc = { 0, 0, _Size.ix(), _Size.iy() };

    ScreenSize = _Size;

    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE); // 내가 원하는 크기를 넣으면 타이틀바까지 고려한 크기를 리턴주는 함수.

    WindowSize = { static_cast<float>(Rc.right - Rc.left), static_cast<float>(Rc.bottom - Rc.top)};
    // 0을 넣어주면 기존의 크기를 유지한다.
    SetWindowPos(HWnd, nullptr, WindowPos.ix(), WindowPos.iy(), WindowSize.ix(), WindowSize.iy(), SWP_NOZORDER);

    if (nullptr != DoubleBufferImage)
    {
        delete DoubleBufferImage;
        DoubleBufferImage = nullptr;
    }

    DoubleBufferImage = new GameEngineImage();
    DoubleBufferImage->ImageCreate(ScreenSize);
}

void GameEngineWindow::SettingWindowPos(float4 _Pos)
{
    WindowPos = _Pos;
    SetWindowPos(HWnd, nullptr, WindowPos.ix(), WindowPos.iy(), WindowSize.ix(), WindowSize.iy(), SWP_NOZORDER);
}

float4 GameEngineWindow::GetMousePosition()
{
    POINT MoniterPoint;
    LPPOINT PointPtr = &MoniterPoint;
    GetCursorPos(PointPtr);
    ScreenToClient(HWnd, PointPtr);

    return { static_cast<float>(MoniterPoint.x),static_cast<float>(MoniterPoint.y) };
}

void GameEngineWindow::Release()
{
    ::UnregisterClassA(wcex.lpszClassName, wcex.hInstance);
}
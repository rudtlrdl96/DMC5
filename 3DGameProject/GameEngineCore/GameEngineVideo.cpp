#include "PrecompileHeader.h"
#include "GameEngineVideo.h"
#include <GameEnginePlatform/GameEngineWindow.h>

GameEngineVideo* GameEngineVideo::CurVideo = nullptr;

GameEngineVideo::GameEngineVideo()
{

}

GameEngineVideo::~GameEngineVideo()
{
	Release();
}



void GameEngineVideo::ResLoad(const std::string_view& _Path)
{
	HWND MainHwnd = GameEngineWindow::GetHWnd();

	//(마지막 인자를 제외하고는 라이브러리에 있는 전역변수입니다)
	//IID_IGraphBuilder 인터페이스를 생성하고 핸들러를 받아오는듯합니다. 
	HRESULT Result = CoCreateInstance(
		CLSID_FilterGraph, 
		nullptr, 
		CLSCTX_INPROC_SERVER, 
		IID_IGraphBuilder, 
		reinterpret_cast<void**>(&GraphBuilderPtr));
	if (S_OK != Result)
	{
		MsgAssert("DirectShow IID_IGraphBuilder 생성에 실패했습니다");
		return;
	}


	//동영상 리소스 불러오기(.avi만 가능합니다)
	std::wstring UnicodePath = GameEngineString::AnsiToUniCode(_Path.data());
	Result = GraphBuilderPtr->RenderFile(UnicodePath.c_str(), nullptr);

	if (Result == VFW_E_UNSUPPORTED_STREAM)
	{
		MsgAssert(".AVI 동영상만 로드 가능합니다");
		return;
	}

	if (S_OK != Result)
	{
		MsgAssert("동영상 로드에 실패했습니다\n마소 공식 문서 RenderFile의 반환값을 확인해보세요");
		return;
	}

	
	//첫번째 인자들은 모두 라이브러리에 있는 전역변수입니다
	GraphBuilderPtr->QueryInterface(IID_IMediaControl, reinterpret_cast<void**>(&Controller));
	GraphBuilderPtr->QueryInterface(IID_IMediaEventEx, reinterpret_cast<void**>(&Eventer));
	GraphBuilderPtr->QueryInterface(IID_IMediaSeeking, reinterpret_cast<void**>(&Seeker));
	GraphBuilderPtr->QueryInterface(IID_IMediaPosition, reinterpret_cast<void**>(&MediaPos));

	GraphBuilderPtr->QueryInterface(IID_IVideoWindow, reinterpret_cast<void**>(&VideoWindowPtr));
	GraphBuilderPtr->QueryInterface(IID_IBasicVideo, reinterpret_cast<void**>(&BasicVideoPtr));

	GraphBuilderPtr->QueryInterface(IID_IBasicAudio, reinterpret_cast<void**>(&BasicAudioPtr));

	
	//이벤트 알림을 처리할 창을 등록합니다.
	Eventer->SetNotifyWindow(
		reinterpret_cast<OAHWND>(MainHwnd), 
		WM_USER + 13, 0);

	//동영상이 출력될 HWND를 설정합니다
	VideoWindowPtr->put_Owner(reinterpret_cast<OAHWND>(MainHwnd));
	//동영상 창의 스타일을 설정합니다
	VideoWindowPtr->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	//비디오 윈도우 크기와 위치를 초기화합니다
	RECT VideoRect = {};
	GetClientRect(MainHwnd, &VideoRect);
	VideoWindowPtr->SetWindowPosition(VideoRect.left, VideoRect.top, VideoRect.right, VideoRect.bottom);

	//프레임 스텝을 받아와 설정합니다
	GraphBuilderPtr->QueryInterface(__uuidof(IVideoFrameStep), reinterpret_cast<void**>(&FrameStep));
	if (nullptr == FrameStep)
	{
		MsgAssert("프레임 스텝 생성에 실패했습니다");
		return;
	}

	//지정된 필터의 단계별 실행 기능을 결정합니다.(0L은 그냥 0입니다)
	if (S_OK != FrameStep->CanStep(0L, nullptr))
	{
		FrameStep->Release();
		MsgAssert("프레임 스텝을 설정할 수 없습니다");
	}

	//비디오 재생 가능 상태
	CurState = VideoState::Init;
}

GameEngineVideo::VideoState GameEngineVideo::GetCurState()
{
	if (nullptr == CurVideo)
	{
		return GameEngineVideo::VideoState::UNKNOWN;
	}
	
	if (true == CurVideo->IsFinished())
	{
		CurVideo = nullptr;
		return GameEngineVideo::VideoState::Stop;
	}

	return GameEngineVideo::VideoState::Running;
}

void GameEngineVideo::Play()
{
	if (nullptr != CurVideo)
	{
		return;
	}

	CurVideo = this;

	if (VideoState::Stop == CurState)
	{
		MsgAssert("이미 종료된 비디오 클립입니다");
		return;
	}

	if (VideoState::Init != CurState)
	{
		MsgAssert("로드하지 않은 비디오클립을 재생하려고 했습니다");
		return;
	}

	HWND MainHwnd = GameEngineWindow::GetHWnd();

	//윈도우 활성화(혹시나 싶어서 넣는 함수들)
	ShowWindow(MainHwnd, SW_SHOWNORMAL);
	UpdateWindow(MainHwnd);
	SetForegroundWindow(MainHwnd);
	SetFocus(MainHwnd);

	HRESULT Result = Controller->Run();
	if (FAILED(Result))
	{
		MsgAssert("동영상 재생에 실패하였습니다");
		return;
	}

	CurState = VideoState::Running;
}

void GameEngineVideo::Stop()
{
	if (nullptr == Controller || nullptr == Seeker)
	{
		MsgAssert("비디오 컨트롤러와 비디오 탐색기가 존재하지 않습니다");
		return;
	}

	if (VideoState::Running != CurState)
	{
		MsgAssert("비디오를 재생하지 않았는데 정지하려고 했습니다");
		return;
	}

	Controller->Stop();
	CurState = VideoState::Stop;

	//비디오 감추기
	VideoWindowPtr->put_Visible(OAFALSE);
	HWND MainHwnd = GameEngineWindow::GetHWnd();
	VideoWindowPtr->put_Owner(reinterpret_cast<OAHWND>(MainHwnd));

	//이벤트 수신 중지
	Eventer->SetNotifyWindow(reinterpret_cast<OAHWND>(nullptr), 0, 0);

	//이 비디오 리소스는 다시 재생할 수 없습니다.
	Release();
}



bool GameEngineVideo::IsFinished()
{
	if (VideoState::Stop == CurState)
	{
		MsgAssert("이미 종료된 비디오 클립입니다");
	}

	if (VideoState::Running != CurState)
	{
		MsgAssert("재생하지도 않은 비디오의 종료 여부를 확인하려고 했습니다");
	}
	
	if (nullptr == Seeker)
	{
		MsgAssert("비디오를 탐색하는 Seeker가 존재하지 않습니다");
	}

	__int64 EndPos = 0;
	if (S_OK != Seeker->GetStopPosition(&EndPos))
	{
		MsgAssert("비디오의 종료지점을 찾지 못했습니다");
	}

	__int64 CurPos = 0;
	if (S_OK != Seeker->GetCurrentPosition(&CurPos))
	{
		MsgAssert("비디오의 현재지점을 찾지 못했습니다");
	}

	return (EndPos == CurPos);
}






void GameEngineVideo::Release()
{
	if (nullptr != VideoWindowPtr)
	{
		VideoWindowPtr->put_Visible(OAFALSE);
		VideoWindowPtr->put_Owner(reinterpret_cast<OAHWND>(GameEngineWindow::GetHWnd()));
	}

	if (nullptr != GraphBuilderPtr)
	{
		GraphBuilderPtr->Release();
		GraphBuilderPtr = nullptr;
	}

	if (nullptr != Controller)
	{
		Controller->Release();
		Controller = nullptr;
	}

	if (nullptr != Eventer)
	{
		Eventer->Release();
		Eventer = nullptr;
	}

	if (nullptr != MediaPos)
	{
		MediaPos->Release();
		MediaPos = nullptr;
	}

	if (nullptr != VideoWindowPtr)
	{
		VideoWindowPtr->Release();
		VideoWindowPtr = nullptr;
	}

	if (nullptr != BasicVideoPtr)
	{
		BasicVideoPtr->Release();
		BasicVideoPtr = nullptr;
	}

	if (nullptr != BasicAudioPtr)
	{
		BasicAudioPtr->Release();
		BasicAudioPtr = nullptr;
	}

	if (nullptr != FrameStep)
	{
		FrameStep->Release();
		FrameStep = nullptr;
	}

}
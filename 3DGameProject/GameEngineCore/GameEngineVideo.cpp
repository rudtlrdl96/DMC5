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

	//(������ ���ڸ� �����ϰ�� ���̺귯���� �ִ� ���������Դϴ�)
	//IID_IGraphBuilder �������̽��� �����ϰ� �ڵ鷯�� �޾ƿ��µ��մϴ�. 
	HRESULT Result = CoCreateInstance(
		CLSID_FilterGraph, 
		nullptr, 
		CLSCTX_INPROC_SERVER, 
		IID_IGraphBuilder, 
		reinterpret_cast<void**>(&GraphBuilderPtr));
	if (S_OK != Result)
	{
		MsgAssert("DirectShow IID_IGraphBuilder ������ �����߽��ϴ�");
		return;
	}


	//������ ���ҽ� �ҷ�����(.avi�� �����մϴ�)
	std::wstring UnicodePath = GameEngineString::AnsiToUniCode(_Path.data());
	Result = GraphBuilderPtr->RenderFile(UnicodePath.c_str(), nullptr);

	if (Result == VFW_E_UNSUPPORTED_STREAM)
	{
		MsgAssert(".AVI ������ �ε� �����մϴ�");
		return;
	}

	if (S_OK != Result)
	{
		MsgAssert("������ �ε忡 �����߽��ϴ�\n���� ���� ���� RenderFile�� ��ȯ���� Ȯ���غ�����");
		return;
	}

	
	//ù��° ���ڵ��� ��� ���̺귯���� �ִ� ���������Դϴ�
	GraphBuilderPtr->QueryInterface(IID_IMediaControl, reinterpret_cast<void**>(&Controller));
	GraphBuilderPtr->QueryInterface(IID_IMediaEventEx, reinterpret_cast<void**>(&Eventer));
	GraphBuilderPtr->QueryInterface(IID_IMediaSeeking, reinterpret_cast<void**>(&Seeker));
	GraphBuilderPtr->QueryInterface(IID_IMediaPosition, reinterpret_cast<void**>(&MediaPos));

	GraphBuilderPtr->QueryInterface(IID_IVideoWindow, reinterpret_cast<void**>(&VideoWindowPtr));
	GraphBuilderPtr->QueryInterface(IID_IBasicVideo, reinterpret_cast<void**>(&BasicVideoPtr));

	GraphBuilderPtr->QueryInterface(IID_IBasicAudio, reinterpret_cast<void**>(&BasicAudioPtr));

	
	//�̺�Ʈ �˸��� ó���� â�� ����մϴ�.
	Eventer->SetNotifyWindow(
		reinterpret_cast<OAHWND>(MainHwnd), 
		WM_USER + 13, 0);

	//�������� ��µ� HWND�� �����մϴ�
	VideoWindowPtr->put_Owner(reinterpret_cast<OAHWND>(MainHwnd));
	//������ â�� ��Ÿ���� �����մϴ�
	VideoWindowPtr->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	//���� ������ ũ��� ��ġ�� �ʱ�ȭ�մϴ�
	RECT VideoRect = {};
	GetClientRect(MainHwnd, &VideoRect);
	VideoWindowPtr->SetWindowPosition(VideoRect.left, VideoRect.top, VideoRect.right, VideoRect.bottom);

	//������ ������ �޾ƿ� �����մϴ�
	GraphBuilderPtr->QueryInterface(__uuidof(IVideoFrameStep), reinterpret_cast<void**>(&FrameStep));
	if (nullptr == FrameStep)
	{
		MsgAssert("������ ���� ������ �����߽��ϴ�");
		return;
	}

	//������ ������ �ܰ躰 ���� ����� �����մϴ�.(0L�� �׳� 0�Դϴ�)
	if (S_OK != FrameStep->CanStep(0L, nullptr))
	{
		FrameStep->Release();
		MsgAssert("������ ������ ������ �� �����ϴ�");
	}

	//���� ��� ���� ����
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
		MsgAssert("�̹� ����� ���� Ŭ���Դϴ�");
		return;
	}

	if (VideoState::Init != CurState)
	{
		MsgAssert("�ε����� ���� ����Ŭ���� ����Ϸ��� �߽��ϴ�");
		return;
	}

	HWND MainHwnd = GameEngineWindow::GetHWnd();

	//������ Ȱ��ȭ(Ȥ�ó� �; �ִ� �Լ���)
	ShowWindow(MainHwnd, SW_SHOWNORMAL);
	UpdateWindow(MainHwnd);
	SetForegroundWindow(MainHwnd);
	SetFocus(MainHwnd);

	HRESULT Result = Controller->Run();
	if (FAILED(Result))
	{
		MsgAssert("������ ����� �����Ͽ����ϴ�");
		return;
	}

	CurState = VideoState::Running;
}

void GameEngineVideo::Stop()
{
	if (nullptr == Controller || nullptr == Seeker)
	{
		MsgAssert("���� ��Ʈ�ѷ��� ���� Ž���Ⱑ �������� �ʽ��ϴ�");
		return;
	}

	if (VideoState::Running != CurState)
	{
		MsgAssert("������ ������� �ʾҴµ� �����Ϸ��� �߽��ϴ�");
		return;
	}

	Controller->Stop();
	CurState = VideoState::Stop;

	//���� ���߱�
	VideoWindowPtr->put_Visible(OAFALSE);
	HWND MainHwnd = GameEngineWindow::GetHWnd();
	VideoWindowPtr->put_Owner(reinterpret_cast<OAHWND>(MainHwnd));

	//�̺�Ʈ ���� ����
	Eventer->SetNotifyWindow(reinterpret_cast<OAHWND>(nullptr), 0, 0);

	//�� ���� ���ҽ��� �ٽ� ����� �� �����ϴ�.
	Release();
}



bool GameEngineVideo::IsFinished()
{
	if (VideoState::Stop == CurState)
	{
		MsgAssert("�̹� ����� ���� Ŭ���Դϴ�");
	}

	if (VideoState::Running != CurState)
	{
		MsgAssert("��������� ���� ������ ���� ���θ� Ȯ���Ϸ��� �߽��ϴ�");
	}
	
	if (nullptr == Seeker)
	{
		MsgAssert("������ Ž���ϴ� Seeker�� �������� �ʽ��ϴ�");
	}

	__int64 EndPos = 0;
	if (S_OK != Seeker->GetStopPosition(&EndPos))
	{
		MsgAssert("������ ���������� ã�� ���߽��ϴ�");
	}

	__int64 CurPos = 0;
	if (S_OK != Seeker->GetCurrentPosition(&CurPos))
	{
		MsgAssert("������ ���������� ã�� ���߽��ϴ�");
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
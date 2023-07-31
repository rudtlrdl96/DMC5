#pragma once
#include "GameEngineResource.h"

#include <dshow.h>
#pragma comment(lib, "Strmiids.lib")

class GameEngineVideo : public GameEngineResource<GameEngineVideo>
{
public:
	enum class VideoState
	{
		Init,
		Running,
		Stop,

		UNKNOWN,
	};

	GameEngineVideo();
	~GameEngineVideo() override;

	GameEngineVideo(const GameEngineVideo& _Other) = delete;
	GameEngineVideo(GameEngineVideo&& _Other) noexcept = delete;
	GameEngineVideo& operator=(const GameEngineVideo& _Other) = delete;
	GameEngineVideo& operator=(const GameEngineVideo&& _Other) noexcept = delete;


	static std::shared_ptr<GameEngineVideo> Load(const std::string_view& _Path)
	{
		GameEnginePath Path = GameEnginePath(std::string(_Path.data()));
		return Load(_Path, Path.GetFileName());
	}

	static std::shared_ptr<GameEngineVideo> Load(const std::string_view& _Path, const std::string_view& _Name)
	{
		std::shared_ptr<GameEngineVideo> Res = GameEngineResource::Create(_Name);
		Res->ResLoad(_Path);
		return Res;
	}

	void Play();
	void Stop();
	bool IsFinished();

	static VideoState GetCurState();

protected:


private:
	static GameEngineVideo* CurVideo;

	VideoState CurState;

	//이 인터페이스는 애플리케이션이 필터 그래프를 작성할 수 있도록 하는 메소드를 제공합니다(마소 공식 문서)
	//Dx의 Factory와 비슷한 개념으로 보입니다.
	IGraphBuilder* GraphBuilderPtr = nullptr;

	//로드한 동영상의 컨트롤러 같습니다.
	IMediaControl* Controller = nullptr;

	//이벤트 처리용이라는 데 사실 잘 모르겠습니다
	IMediaEventEx* Eventer = nullptr;

	//동영상 탐색할 때 쓰입니다?
	IMediaSeeking* Seeker = nullptr;

	//동영상이 어디를 진행중인지 알기위한 값 같습니다
	IMediaPosition* MediaPos = nullptr;

	//비디오 창의 속성을 설정하는데 쓰입니다.
	IVideoWindow* VideoWindowPtr = nullptr;

	//비디어의 속성을 설정하기 위해 쓰이는 값입니다.
	IBasicVideo* BasicVideoPtr = nullptr;
	//오디오의 속성을 설정하기 위해 쓰이는 값입니다.
	IBasicAudio* BasicAudioPtr = nullptr;

	//이 인터페이스를 사용하면 동영상을 한 번에 한 프레임씩 느리게 비디오 스트림을 진행할 수 있습니다
	IVideoFrameStep* FrameStep = nullptr;

	//동영상을 로드합니다
	void ResLoad(const std::string_view& _Path);

	void Release();
};


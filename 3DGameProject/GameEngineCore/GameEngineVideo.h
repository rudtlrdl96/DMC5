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

	//�� �������̽��� ���ø����̼��� ���� �׷����� �ۼ��� �� �ֵ��� �ϴ� �޼ҵ带 �����մϴ�(���� ���� ����)
	//Dx�� Factory�� ����� �������� ���Դϴ�.
	IGraphBuilder* GraphBuilderPtr = nullptr;

	//�ε��� �������� ��Ʈ�ѷ� �����ϴ�.
	IMediaControl* Controller = nullptr;

	//�̺�Ʈ ó�����̶�� �� ��� �� �𸣰ڽ��ϴ�
	IMediaEventEx* Eventer = nullptr;

	//������ Ž���� �� ���Դϴ�?
	IMediaSeeking* Seeker = nullptr;

	//�������� ��� ���������� �˱����� �� �����ϴ�
	IMediaPosition* MediaPos = nullptr;

	//���� â�� �Ӽ��� �����ϴµ� ���Դϴ�.
	IVideoWindow* VideoWindowPtr = nullptr;

	//������ �Ӽ��� �����ϱ� ���� ���̴� ���Դϴ�.
	IBasicVideo* BasicVideoPtr = nullptr;
	//������� �Ӽ��� �����ϱ� ���� ���̴� ���Դϴ�.
	IBasicAudio* BasicAudioPtr = nullptr;

	//�� �������̽��� ����ϸ� �������� �� ���� �� �����Ӿ� ������ ���� ��Ʈ���� ������ �� �ֽ��ϴ�
	IVideoFrameStep* FrameStep = nullptr;

	//�������� �ε��մϴ�
	void ResLoad(const std::string_view& _Path);

	void Release();
};


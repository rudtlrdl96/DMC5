#pragma once
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineGUI.h>
#include "AnimationEvent.h"
// 설명 : 애니메이션에 이벤트를 게임 화면에서 구현하기 위해 사용되는 ImGUI 윈도우
class AnimationToolWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	AnimationToolWindow();
	~AnimationToolWindow();

	// delete Function
	AnimationToolWindow(const AnimationToolWindow& _Other) = delete;
	AnimationToolWindow(AnimationToolWindow&& _Other) noexcept = delete;
	AnimationToolWindow& operator=(const AnimationToolWindow& _Other) = delete;
	AnimationToolWindow& operator=(AnimationToolWindow&& _Other) noexcept = delete;

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;
	void FileLoad(std::shared_ptr<GameEngineLevel> Level);
	void AnimationTimeLine();

	void FrameEvent();

	void FileSave();

	void MeshCreate(std::shared_ptr<GameEngineLevel> Level);
	void AnimationCreate(std::shared_ptr<GameEngineLevel> Level);
private:
	std::shared_ptr<GameEngineActor> Actor = nullptr;
	std::shared_ptr<class GameEngineFBXRenderer> Renderer = nullptr;
	AnimationEvent AnimEvent;
	std::list<EventData*> CurFrameEvents;

	GameEnginePath MeshFilePath;
	GameEnginePath AnimFBXFilePath;
	GameEnginePath AnimFilePath;
	std::string MeshName = "";
	std::string AnimationFBXName = "";
	std::string AnimationName = "";
	int CurrentFrame = 0;
	int FrameSize = 0;
	float AnimSpeed = 3.0f;
	bool IsStop = false;
};


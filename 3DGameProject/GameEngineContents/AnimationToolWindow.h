#pragma once
#include <GameEngineBase/GameEnginePath.h>
#include <GameEngineCore/GameEngineGUI.h>
// ���� : �ִϸ��̼ǿ� �̺�Ʈ�� ���� ȭ�鿡�� �����ϱ� ���� ���Ǵ� ImGUI ������
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
	void FileLoad();
	void AnimationTimeLine();
	void FileSave();

private:
	GameEnginePath FBXFilePath;
	std::string AnimationName = "";
	int CurrentFrame = 0;
	int FrameSize = 20;
};


#include "PrecompileHeader.h"
#include "AnimationToolLevel.h"
#include "AnimationToolWindow.h"
#include "FreeCameraActor.h"
AnimationToolLevel::AnimationToolLevel() 
{
}

AnimationToolLevel::~AnimationToolLevel() 
{
}

void AnimationToolLevel::Start()
{
	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -500 });
}

void AnimationToolLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("ReturnToMainLevel"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
	}
}

void AnimationToolLevel::LevelChangeStart()
{
	if (false == IsMessage)
	{
		IsMessage = true;
		MsgTextBox("CurrentLevel is AnimationToolLevel");
	}
	if (AnimWindow == nullptr)
	{
		AnimWindow = std::dynamic_pointer_cast<AnimationToolWindow>(GameEngineGUI::GUIWindowCreate<AnimationToolWindow>("AnimationToolWindow"));
		CreateActor<FreeCameraActor>();
	}
}

void AnimationToolLevel::LevelChangeEnd()
{
	IsMessage = false;
}


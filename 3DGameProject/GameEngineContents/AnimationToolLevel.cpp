#include "PrecompileHeader.h"
#include "AnimationToolLevel.h"
#include <GameEngineCore/EngineGrid.h>
#include <GameEngineCore/PhysXBoxComponent.h>
#include "AnimationToolWindow.h"
#include "FreeCameraActor.h"
#include "Plane.h"
AnimationToolLevel::AnimationToolLevel()
{
}

AnimationToolLevel::~AnimationToolLevel() 
{
}

void AnimationToolLevel::Start()
{
	CreateScene(GetName());
	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -500 });

	GetDirectionalLight()->GetTransform()->SetLocalRotation({ 0, 180, 0 });
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
	SetLevelSceneGravity(0);
	//std::shared_ptr<Plane> Flat = CreateActor<Plane>();
	//Flat->Component->GetDynamic()->setGlobalPose({ 0, -500, 0 });
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


#include "PrecompileHeader.h"
#include "EffectToolLevel.h"
#include "EffectToolWindow.h"
#include "FreeCameraActor.h"
EffectToolLevel::EffectToolLevel() 
{
}

EffectToolLevel::~EffectToolLevel() 
{
}

void EffectToolLevel::Start()
{
	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -500 });
	CreateActor<GameEngineLight>();
	GetDirectionalLight()->GetTransform()->SetLocalRotation({ 0, 180, 0 });
}

void EffectToolLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("ReturnToMainLevel"))
	{
		GameEngineCore::ChangeLevel("MainLevel");
	}
}

void EffectToolLevel::LevelChangeStart()
{
	if (ToolWindow == nullptr)
	{
		ToolWindow = std::dynamic_pointer_cast<EffectToolWindow>(GameEngineGUI::GUIWindowCreate<EffectToolWindow>("AnimationToolWindow"));
		CreateActor<FreeCameraActor>();
	}
}

void EffectToolLevel::LevelChangeEnd()
{
}

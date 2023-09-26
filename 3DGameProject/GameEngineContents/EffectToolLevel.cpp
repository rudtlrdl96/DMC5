#include "PrecompileHeader.h"
#include "EffectToolLevel.h"
#include <GameEngineCore/EngineGrid.h>
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
		CreateActor<EngineGrid>();
	}

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Effect");
	if (nullptr == GameEngineFBXMesh::Find("Effect_Mesh_01.fbx"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}
	if (nullptr == GameEngineTexture::Find("Effect_Texture_01.tga"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".tga" });
		for (GameEngineFile File : Files)
		{
			GameEngineTexture::Load(File.GetFullPath());
		}
	}

	NewDir.Move("Texture");
	if (nullptr == GameEngineSprite::Find("Effect_Impact.tga"))
	{
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Impact.tga").GetFullPath(), 8, 8);
	}
}

void EffectToolLevel::LevelChangeEnd()
{
}

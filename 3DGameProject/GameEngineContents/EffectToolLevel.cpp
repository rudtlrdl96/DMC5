#include "PrecompileHeader.h"
#include "EffectToolLevel.h"
#include <GameEngineCore/EngineGrid.h>
#include "EffectToolWindow.h"
#include "FreeCameraActor.h"
#include "SkyBox.h"
#include "JudgementCut.h"
#include "DistortionEffect.h"
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

	{
		std::shared_ptr<DistortionEffect> Distortion = GetCamera(0)->GetCamTarget()->CreateEffect<DistortionEffect>();
		Distortion->SetMaskTexture(GetCamera(0)->GetCamAlphaTarget(), 1);
		Distortion->SetDistortionValue(8, 4.5);
	}
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
		GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
		GameEngineDirectory Dir = GameEnginePath::GetFileFullPath
		(
			"ContentResources",
			{
				"Map", "TestMap"
			}
		);
		if (nullptr == GameEngineFBXMesh::Find("SkyBox.fbx"))
		{
			GameEngineFBXMesh::Load(Dir.GetPlusFileName("SkyBox.fbx").GetFullPath());
		}

		std::shared_ptr<SkyBox> Sky = CreateActor<SkyBox>();
		Sky->CreateSkyBox(this, "SkyBox.fbx");
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
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Impact_01.tga").GetFullPath(), 8, 4);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Muzzle_03.tga").GetFullPath(), 2, 1);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Spark_02.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Magic_01.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Fire_01.tga").GetFullPath(), 8, 4);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Fire_02.tga").GetFullPath(), 8, 4);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Fire_03.tga").GetFullPath(), 8, 4);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Fire_04.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Fire_05.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Explosion_01.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Explosion_02.tga").GetFullPath(), 8, 8);
	}
}

void EffectToolLevel::LevelChangeEnd()
{
}

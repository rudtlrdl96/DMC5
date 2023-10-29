#include "PrecompileHeader.h"
#include "ResultLevel.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "ResultActor.h"
#include "FXAA_Effect.h"
#include "FadeEffect.h"
#include "BGMPlayer.h"

ResultLevel::ResultLevel()
{
}

ResultLevel::~ResultLevel()
{
}

void ResultLevel::Start()
{
	GetMainCamera()->SetProjectionType(CameraType::Perspective);

	//GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	//GetLastTarget()->CreateEffect<FadeEffect>();
}

void ResultLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

}

void ResultLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();
	GetDirectionalLight()->SetLightColor({0.9f, 0.9f, 1.0f});
	GetDirectionalLight()->SetLightPower(2.0f);
	GetDirectionalLight()->SetDifLightPower(0.7f);
	GetDirectionalLight()->SetAmbLightPower(5.0f);
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 20, 0, 0 });
	GameEngineTime::GlobalTime.SetGlobalTimeScale(1.0f);
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Texture");
		NewDir.Move("UI");
		NewDir.Move("ResultLevel");
		if (nullptr == GameEngineTexture::Find("a_albm.texout.tga"))
		{
			std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".tga", ".png"});
			for (GameEngineFile File : Files)
			{
				GameEngineTexture::Load(File.GetFullPath());
			}
		}
	}

	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Mesh");
		NewDir.Move("UIMesh");
		NewDir.Move("ResultMesh");
		if (nullptr == GameEngineFBXMesh::Find("shape_plane.fbx"))
		{
			std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
			for (GameEngineFile File : Files)
			{
				GameEngineFBXMesh::Load(File.GetFullPath());
			}
		}
	}
	if (nullptr == GameEngineFBXMesh::Find("l02_SkyBox.fbx"))
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Map");
		NewDir.Move("Location2");
		GameEngineFBXMesh::Load(NewDir.GetPlusFileName("l02_SkyBox.fbx").GetFullPath());
	}

	BGMPlayer::BGMLoad();
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Sound");
		NewDir.Move("SFX");
		NewDir.Move("Result");
		if (nullptr == GameEngineSound::Find("Result_SFX_0.wav"))
		{
			std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".wav" });
			for (GameEngineFile File : Files)
			{
				GameEngineSound::Load(File.GetFullPath());
			}
		}
	}

	BGMPlayer::SetClearBGM();

	Actor = CreateActor<ResultActor>();
}

void ResultLevel::LevelChangeEnd()
{
	GameEngineLevel::LevelChangeEnd();
}

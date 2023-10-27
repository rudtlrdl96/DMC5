#include "PrecompileHeader.h"
#include "ResultLevel.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "ResultActor.h"
#include "FXAA_Effect.h"
#include "FadeEffect.h"
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
	GetDirectionalLight()->SetLightPower(9.0f);
	GetDirectionalLight()->SetDifLightPower(0.3f);
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 20, 0, 0 });
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Mesh");
		NewDir.Move("UIMesh");
		NewDir.Move("ResultMesh");
		if (nullptr == GameEngineFBXMesh::Find("shape_plane.fbx"))
		{
			GameEngineFBXMesh::Load(NewDir.GetPlusFileName("shape_plane.fbx").GetFullPath());
		}
	}

	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Texture");
		NewDir.Move("UI");
		NewDir.Move("ResultLevel");
		if (nullptr == GameEngineTexture::Find("Result_ScreenShot.jpg"))
		{
			std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".jpg", ".tga", ".png"});
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
	Actor = CreateActor<ResultActor>();

	GameEngineSound::Play("Result_SFX_0.wav");
}

void ResultLevel::LevelChangeEnd()
{
	GameEngineLevel::LevelChangeEnd();
}

#include "PrecompileHeader.h"
#include "ResultLevel.h"
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
	//GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -450.1f });
	//GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -540.1f });

	GetCamera(0)->GetCamTarget()->CreateEffect<FXAA_Effect>();
	GetLastTarget()->CreateEffect<FadeEffect>();
}

void ResultLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

}

void ResultLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();

	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Texture");
		NewDir.Move("UI");
		NewDir.Move("ResultLevel");
		if (nullptr == GameEngineTexture::Find("Result_ScreenShot.jpg"))
		{
			GameEngineTexture::Load(NewDir.GetPlusFileName("Result_ScreenShot.jpg").GetFullPath());
		}
	}
	Actor = CreateActor<ResultActor>();
}

void ResultLevel::LevelChangeEnd()
{
	GameEngineLevel::LevelChangeEnd();
}

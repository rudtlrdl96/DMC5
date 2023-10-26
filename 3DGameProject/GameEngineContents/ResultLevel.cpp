#include "PrecompileHeader.h"
#include "ResultLevel.h"
#include "ResultActor.h"
ResultLevel::ResultLevel()
{
}

ResultLevel::~ResultLevel()
{
}

void ResultLevel::Start()
{
	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });
}

void ResultLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

}

void ResultLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();
	Actor = CreateActor<ResultActor>();
	//{
	//	GameEngineDirectory NewDir;
	//	NewDir.MoveParentToDirectory("ContentResources");
	//	NewDir.Move("ContentResources");
	//	NewDir.Move("Texture");
	//	NewDir.Move("UI");
	//	NewDir.Move("PlayLevelUI");
	//	if (nullptr == GameEngineSprite::Find("HPGaugeAni.png"))
	//	{
	//		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("HPGaugeAni.png").GetFullPath(), 4, 4);
	//		GameEngineTexture::Load(NewDir.GetPlusFileName("BossGaugeBase.png").GetFullPath());
	//		GameEngineTexture::Load(NewDir.GetPlusFileName("BossHitGauge.png").GetFullPath());
	//		GameEngineTexture::Load(NewDir.GetPlusFileName("123123123.png").GetFullPath());
	//	}
	//}
}

void ResultLevel::LevelChangeEnd()
{
	GameEngineLevel::LevelChangeEnd();
}

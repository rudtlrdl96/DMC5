#include "PrecompileHeader.h"
#include "PlayUITestLevel.h"
#include "PlayerHPUI.h"
#include "NeroItemGlass.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineFont.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include "NeroLoading.h"
#include "C_ChoiceBG.h"
#include "BossHpBar.h"
#include "RankUI.h"
#include "DistortionEffect.h"

PlayUITestLevel::PlayUITestLevel()
{
}

PlayUITestLevel::~PlayUITestLevel()
{
}

void PlayUITestLevel::Start()
{

}

void PlayUITestLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

}

void PlayUITestLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();

	std::shared_ptr<DistortionEffect> Distortion = GetCamera(100)->GetCamTarget()->CreateEffect<DistortionEffect>();
	Distortion->SetMaskTexture(GetCamera(100)->GetCamAlphaTarget(), 1);
	Distortion->SetDistortionValue(8, 4.5);
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Texture");
		NewDir.Move("UI");
		NewDir.Move("PlayLevelUI");
		if (nullptr == GameEngineSprite::Find("HPGaugeAni.png"))
		{
			GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("HPGaugeAni.png").GetFullPath(), 4, 4);
			GameEngineTexture::Load(NewDir.GetPlusFileName("BossGaugeBase.png").GetFullPath());
			GameEngineTexture::Load(NewDir.GetPlusFileName("BossHitGauge.png").GetFullPath());
			GameEngineTexture::Load(NewDir.GetPlusFileName("123123123.png").GetFullPath());


		}
	}
}

void PlayUITestLevel::LevelChangeEnd()
{
	GameEngineLevel::LevelChangeEnd();
}
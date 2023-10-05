#include "PrecompileHeader.h"
#include "PlayUITestLevel.h"
#include "NeroHPUI.h"
#include "NeroItemGlass.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineFont.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include "NeroLoading.h"
#include "C_ChoiceBG.h"
#include "BossHpBar.h"
PlayUITestLevel::PlayUITestLevel()
{
}

PlayUITestLevel::~PlayUITestLevel()
{
}

void PlayUITestLevel::Start()
{
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
		}

	}

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });
	//std::shared_ptr<NeroLoading> NeroHPUIPtr = CreateActor<NeroLoading>();
	//std::shared_ptr<NeroItemGlass> NeroItemGlassPtr = CreateActor<NeroItemGlass>();
	std::shared_ptr<C_ChoiceBG> C_ChoiceBGPtr = CreateActor<C_ChoiceBG>();
	std::shared_ptr<NeroHPUI> NeroHPUIPtr = CreateActor<NeroHPUI>();
	std::shared_ptr<BossHpBar> BossHpBarPtr = CreateActor<BossHpBar>();

	
}

void PlayUITestLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

}

void PlayUITestLevel::LevelChangeStart()
{

	GameEngineLevel::LevelChangeStart();
}

void PlayUITestLevel::LevelChangeEnd()
{
	GameEngineLevel::LevelChangeEnd();
}
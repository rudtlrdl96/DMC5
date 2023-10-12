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
}

void PlayUITestLevel::LevelChangeEnd()
{
	GameEngineLevel::LevelChangeEnd();
}
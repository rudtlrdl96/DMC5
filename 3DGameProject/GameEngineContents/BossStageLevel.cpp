#include "PrecompileHeader.h"
#include "BossStageLevel.h"
#include "NetworkManager.h"

BossStageLevel::BossStageLevel()
{
}

BossStageLevel::~BossStageLevel()
{
}

void BossStageLevel::Start()
{
	SetCamera({ 0,0,-500 });
}

void BossStageLevel::Update(float _DeltaTime)
{
	StartProcess(this);
}


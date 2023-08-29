#include "PrecompileHeader.h"
#include "BossStageLevel.h"
#include "NetworkManager.h"

BossStageLevel* BossStageLevel::Inst = nullptr;

BossStageLevel::BossStageLevel()
{
	Inst = this;
}

BossStageLevel::~BossStageLevel()
{
	Inst = nullptr;
}

void BossStageLevel::Start()
{
	StageBaseLevel::Start();
	SetCamera({ 0,0,-500 });
}

void BossStageLevel::Update(float _DeltaTime)
{
	StageBaseLevel::Update(_DeltaTime);
}


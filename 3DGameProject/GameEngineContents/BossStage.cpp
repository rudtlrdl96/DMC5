#include "PrecompileHeader.h"
#include "BossStage.h"
#include "NetworkManager.h"

BossStage::BossStage()
{
}

BossStage::~BossStage()
{
}

void BossStage::Start()
{
	SetCamera({ 0,0,-500 });
}

void BossStage::Update(float _DeltaTime)
{
	StartProcess(this);
}


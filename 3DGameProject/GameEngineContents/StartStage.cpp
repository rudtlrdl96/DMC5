#include "PrecompileHeader.h"
#include "StartStage.h"
#include "NetworkManager.h"

#include "StageBase.h"

StartStage::StartStage()
{
}

StartStage::~StartStage()
{
}

void StartStage::Start()
{
	SetCamera({ 0,0,-500 });
}

void StartStage::Update(float _DeltaTime)
{
	StartProcess(this);
}


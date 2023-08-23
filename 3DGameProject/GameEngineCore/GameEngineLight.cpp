#include "PrecompileHeader.h"
#include "GameEngineLight.h"
#include "GameEngineLevel.h"

// 쉐이더에서 Light.fx를 이용하는 경우 이녀석은 빛을 사용하는 녀석이다

GameEngineLight::GameEngineLight()
{
}

GameEngineLight::~GameEngineLight()
{
}


void GameEngineLight::Start()
{
	GetLevel()->PushLight(DynamicThis<GameEngineLight>());
}
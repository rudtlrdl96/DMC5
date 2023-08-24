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

void GameEngineLight::Update(float _DeltaTime)
{
	//LightDataValue.LightPos = GetTransform()->GetWorldPosition();
	//LightDataValue.LightDir = GetTransform()->GetLocalForwardVector();
}

void GameEngineLight::LightUpdate(GameEngineCamera* _Camera, float _DeltaTime)
{
	LightDataValue.LightPos = GetTransform()->GetWorldPosition();
	LightDataValue.LightDir = GetTransform()->GetLocalForwardVector();
	LightDataValue.ViewLightPos = LightDataValue.LightPos * _Camera->GetView();
	LightDataValue.ViewLightDir = LightDataValue.LightDir * _Camera->GetView();
	LightDataValue.ViewLightRevDir = -LightDataValue.ViewLightDir;
}
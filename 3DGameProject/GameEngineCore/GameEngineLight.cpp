#include "PrecompileHeader.h"
#include "GameEngineLight.h"
#include "GameEngineLevel.h"

// ���̴����� Light.fx�� �̿��ϴ� ��� �̳༮�� ���� ����ϴ� �༮�̴�

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
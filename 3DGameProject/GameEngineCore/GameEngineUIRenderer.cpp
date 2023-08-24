#include "PrecompileHeader.h"
#include "GameEngineUIRenderer.h"
#include "GameEngineLevel.h"

GameEngineUIRenderer::GameEngineUIRenderer() 
{
	SetName("GameEngineUIRenderer");
}

GameEngineUIRenderer::~GameEngineUIRenderer() 
{
}

void GameEngineUIRenderer::Start() 
{
	PushCameraRender(100);
	SpriteRenderInit();
}

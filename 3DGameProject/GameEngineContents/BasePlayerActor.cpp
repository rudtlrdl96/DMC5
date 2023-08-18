#include "PrecompileHeader.h"
#include "BasePlayerActor.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
BasePlayerActor* BasePlayerActor::MainPlayer = nullptr;

BasePlayerActor::BasePlayerActor() 
{
}

BasePlayerActor::~BasePlayerActor() 
{
}

void BasePlayerActor::Start()
{
	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("Nero.FBX", "MeshTexture");

	
}

void BasePlayerActor::Update(float _DeltaTime)
{
}


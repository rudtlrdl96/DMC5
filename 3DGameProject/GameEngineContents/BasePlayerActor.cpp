#include "PrecompileHeader.h"
#include "BasePlayerActor.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "PlayerController.h"
#include "PlayerCamera.h"
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

	Controller = CreateComponent<PlayerController>();
	//Camera = CreateComponent< PlayerCamera>();
}

void BasePlayerActor::Update(float _DeltaTime)
{
	GetTransform()->AddLocalPosition(Controller->GetMoveVector() * _DeltaTime * 100);
}


#include "PrecompileHeader.h"
#include "BasePlayerActor.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include "PlayerController.h"
#include "PlayerCamera.h"
#include "ServerWindow.h"
#include <GameEngineBase/GameEngineNet.h>
#include "ObjectUpdatePacket.h"
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
	Renderer->SetFBXMesh("House1.FBX", "MeshTexture");
	//Renderer->SetFBXMesh("Nero.FBX", "MeshTexture");

	Camera = CreateComponent<PlayerCamera>();
	Controller = CreateComponent<PlayerController>();
}

void BasePlayerActor::Update(float _DeltaTime)
{
	if (Controller == nullptr)
	{
		return;
	}
	GetTransform()->AddLocalPosition(Controller->GetMoveVector() * _DeltaTime * 100);


	static float Delta = 0.0f;
	Delta += _DeltaTime;
	if (Delta <= 1.0f / 60.0f)
	{
		return;
	}

	Delta -= 1.0f / 60.0f;
	if (true == IsNet())
	{
		std::shared_ptr<ObjectUpdatePacket> NewPacket = std::make_shared<ObjectUpdatePacket>();
		NewPacket->SetObjectID(GetNetObjectID());
		NewPacket->Position = GetTransform()->GetWorldPosition();
		NewPacket->Rotation = GetTransform()->GetWorldPosition();
		GetNet()->SendPacket(NewPacket);
	}
}


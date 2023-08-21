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
	MainPlayer = this;
}

BasePlayerActor::~BasePlayerActor()
{
}

void BasePlayerActor::Start()
{
	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.FBX", "MeshTexture");
	Renderer->GetTransform()->SetLocalRotation({ 0, 90, 0 });
	//Renderer->SetFBXMesh("Nero.FBX", "MeshTexture");

	Camera = GetLevel()->CreateActor<PlayerCamera>();
	Camera->SetPlayerTranform(GetTransform());
	Controller = CreateComponent<PlayerController>();
	Controller->SetCameraTransform(Camera->GetTransform());
}

void BasePlayerActor::Update(float _DeltaTime)
{
	NetControllType Type = GetControllType();
	GetTransform()->AddLocalPosition(Controller->GetMoveVector() * _DeltaTime * 100);
	if (Controller->GetMoveVector() != float4::ZERO)
	{
		float4 LocalForward = Controller->GetMoveVector();
		float4 PlayerForward = GetTransform()->GetWorldForwardVector();

		float4 Cross = float4::Cross3DReturnNormal(LocalForward, PlayerForward);
		float Dot = float4::DotProduct3D(LocalForward, PlayerForward);

		if (Cross.y < -0.99f)
		{
			GetTransform()->AddLocalRotation({ 0, 180, 0 });
		}
		else
		{
			GetTransform()->AddLocalRotation({ 0, GameEngineMath::RadToDeg * Dot, 0 });
		}

	}


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


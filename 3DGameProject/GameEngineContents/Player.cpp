#include "PrecompileHeader.h"
#include "Player.h"

#include <GameEngineBase/GameEngineNet.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "NetworkManager.h"

#include "ObjectUpdatePacket.h"
#include "ServerWindow.h"
#include "ContentsEnum.h"

Player* Player::MainPlayer = nullptr;


Player::Player()
{
	MainPlayer = this;
}


Player::~Player()
{

}


void Player::Start()
{
	SetNetObjectType(Net_ActorType::Nero);

	if (false == GameEngineInput::IsKey("MoveLeft"))
	{
		GameEngineInput::CreateKey("MoveLeft", 'A');
		GameEngineInput::CreateKey("MoveRight", 'D');
		GameEngineInput::CreateKey("MoveUp", 'Q');
		GameEngineInput::CreateKey("MoveDown", 'E');
		GameEngineInput::CreateKey("MoveForward", 'W');
		GameEngineInput::CreateKey("MoveBack", 'S');

		//GameEngineInput::CreateKey("RotY+", VK_NUMPAD1);
		//GameEngineInput::CreateKey("RotY-", VK_NUMPAD2);
		//GameEngineInput::CreateKey("RotZ+", VK_NUMPAD4);
		//GameEngineInput::CreateKey("RotZ-", VK_NUMPAD5);
		//GameEngineInput::CreateKey("RotX+", VK_NUMPAD7);
		//GameEngineInput::CreateKey("RotX-", VK_NUMPAD8);
	}

	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("Map11_Collapse.FBX", "NoneAlphaMesh");
	Renderer->GetTransform()->SetWorldScale({100.f, 100.f, 100.f});
	Renderer->SetName("this");
	// Renderer->SetFBXMesh("AnimMan.FBX", "MeshTexture", 0, 0);
	// Renderer->SetFBXMesh("AnimMan.FBX", "MeshTexture", 0, 2);

}

void Player::Update(float _DeltaTime)
{
	NetControllType Type = GetControllType();
	switch (Type)
	{
	case NetControllType::UserControll:
		UserUpdate(_DeltaTime);
		break;
	case NetControllType::NetControll:
		ServerUpdate(_DeltaTime);
		break;
	}
}

void Player::UserUpdate(float _DeltaTime)
{
	float Speed = 200.0f;

	if (true == GameEngineInput::IsPress("MoveLeft"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldLeftVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveRight"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldRightVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveUp"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldUpVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveDown"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldDownVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveForward"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldForwardVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveBack"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldBackVector() * Speed * _DeltaTime);
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
		NewPacket->Rotation = GetTransform()->GetWorldRotation();
		GetNet()->SendPacket(NewPacket);
	}
}

void Player::ServerUpdate(float _DeltaTime)
{
	if (false == IsPacket())
	{
		return;
	}

	while (IsPacket())
	{
		PacketEnum Type = GetFirstPacketType<PacketEnum>();

		switch (Type)
		{
		case PacketEnum::ObjectUpdatePacket:
		{
			std::shared_ptr<ObjectUpdatePacket> ObjectUpdate = PopFirstPacket<ObjectUpdatePacket>();
			GetTransform()->SetLocalPosition(ObjectUpdate->Position);
			GetTransform()->SetLocalRotation(ObjectUpdate->Rotation);
			break;
		}
		default:
		{
			MsgAssert("처리하지 못하는 패킷이 플레이어로 날아왔습니다.");
			return;
		}
		}
	}
}


void Player::Update_SendPacket(float _DeltaTime)
{
	NetworkManager::SendUpdatePacket(this, GetTransform(), 1.f);
}
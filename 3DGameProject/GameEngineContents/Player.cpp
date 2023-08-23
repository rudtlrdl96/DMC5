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

	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.FBX", "NoneAlphaMesh");
	//Renderer->GetTransform()->SetWorldScale({100.f, 100.f, 100.f});

	// Renderer->SetFBXMesh("AnimMan.FBX", "MeshTexture", 0, 0);
	// Renderer->SetFBXMesh("AnimMan.FBX", "MeshTexture", 0, 2);

	GameEngineInput::CreateKey("TestDestroy", 'N');
}

void Player::Update_ProcessPacket()
{
	if (false == NetworkManager::IsClient())
		return;

	while (GameEngineNetObject::IsPacket())
	{
		PacketEnum Type = GameEngineNetObject::GetFirstPacketType<PacketEnum>();

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

void Player::Update(float _DeltaTime)
{
	if (false == NetworkManager::IsServer())
		return;

	if (false == GameEngineInput::IsDown("TestDestroy"))
		return;

	//추가로 여기에 NetObejct자료구조에서 빼야함
	//Death();
	TestNetDisConnect();
}

void Player::Update_SendPacket(float _DeltaTime)
{
	if (false == NetworkManager::IsServer())
		return;

	NetworkManager::SendUpdatePacket(this, GetTransform(), 1.f);
}
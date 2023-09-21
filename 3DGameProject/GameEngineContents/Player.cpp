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
	//SetNetObjectType(Net_ActorType::Nero);

	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.FBX", "FBX");
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
			//GetTransform()->SetLocalPosition(ObjectUpdate->Position);
			//GetTransform()->SetLocalRotation(ObjectUpdate->Rotation);
			break;
		}
		default:
		{
			MsgAssert("ó������ ���ϴ� ��Ŷ�� �÷��̾�� ���ƿԽ��ϴ�.");
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

	//�߰��� ���⿡ NetObejct�ڷᱸ������ ������
	//Death();
	//TestNetDisConnect();
}

void Player::Update_SendPacket(float _DeltaTime)
{
	/*if (false == NetworkManager::IsServer())
		return;

	NetworkManager::PushUpdatePacket(UpdatePacketParameter
		{
			.ObjPtr = this,
			.FsmState = 0,
			.IsFsmForce = false,
			.TimeScale = 1.f
		});*/
}
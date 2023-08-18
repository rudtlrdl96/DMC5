#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"


////////
//		Ŭ�� ��Ŷ �ʱ�ȭ
////////


void NetworkManager::ClientPacketInit()
{
	//ConnectIDPacket ó��
	NetInst->Dispatcher.AddHandler<ConnectIDPacket>
		([=](std::shared_ptr<ConnectIDPacket> _Packet)
	{
		//ó�� ���� �ߴٸ� Player�� ������ ���� ����
		//Player::MainPlayer->InitNetObject(_Packet->GetObjectID(), ServerWindow::NetInst);
	});


	//ObjectUpdatePacket ó��
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		//�ش� NetObejctID�� ��ü�� �������� �ʴٸ� �����
		if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
		{
			//std::shared_ptr<Player> NewPlayer = NowLevel->CreateActor<Player>();
			//NewPlayer->InitNetObject(_Packet->GetObjectID(), _Net);
		}

		//Player�� ������ ó���� �� �ְ� �ڷᱸ���� ����
		GameEngineNetObject::PushNetObjectPacket(_Packet);
	});
}
#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"

////////
//		���� ��Ŷ �ʱ�ȭ
////////

void NetworkManager::AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server)
{
	// ������ ������Ը� ����, ConnectIDPacket��Ŷ
	std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();

	int ID = GameEngineNetObject::CreateServerID();
	Packet->SetObjectID(ID);

	GameEngineSerializer Ser;
	Packet->SerializePacket(Ser);

	//������ ID�� ������ �������� ����
	_Server->AddUser(ID, _Socket);

	// �����ϰ� �ѹ� �� ���� ������ �Ἥ �������Ҷ�.
	GameEngineNet::Send(_Socket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
}


void NetworkManager::ServerPacketInit()
{
	//ObjectUpdatePacket ó��
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		//�ش� NetObejctID�� ��ü�� �������� �ʴٸ� �����
		/*if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
		{
			std::shared_ptr<Player> NewPlayer = NowLevel->CreateActor<Player>();
			NewPlayer->InitNetObject(_Packet->GetObjectID(), _Net);
		}*/

		//Player�� ������ ó���� �� �ְ� �ڷᱸ���� ����
		GameEngineNetObject::PushNetObjectPacket(_Packet);

		//������ ��쿣 ���Ź��� Ư�� ������Ʈ�� ��Ŷ�� �ٸ� Ŭ�� �� �ѷ��� �Ѵ�
		NetInst->SendPacket(_Packet, _Packet->GetObjectID());
	});



}


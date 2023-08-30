#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "NetworkGUI.h"
#include "ContentsEnum.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"

////////
//		Ŭ�� ��Ŷ �ʱ�ȭ
////////


void NetworkManager::ClientPacketInit()
{
	//ConnectIDPacket ó��
	NetInst->Dispatcher.AddHandler<ConnectIDPacket>
		([=](std::shared_ptr<ConnectIDPacket> _Packet)
	{
		//�� Ŭ���̾�Ʈ�� ��Ʈ��ũ ���̵� ����
		NetID = _Packet->GetObjectID();
		NetworkGUI::GetInst()->InitName(NetID);

		//��Ŷ�� ��ܿ� ������Ʈ���̵�� �÷��̾� ĳ���� ����
		const std::vector<unsigned int>& AllObjectID = _Packet->AllObjectID;
		if (AllObjectID.size() != AllBattleLevels.size())
		{
			MsgAssert("��Ŷ���� ���� ������ƮID�� ������ ������ ����Ǵ� ������ ������ �ٸ��ϴ�");
			return;
		}

		for (size_t i = 0; i < AllObjectID.size(); ++i)
		{
			GameEngineLevel* Level = AllBattleLevels[i];
			int ID = static_cast<int>(AllObjectID[i]);
			CreateLocalPlayer(Level, ID);
		}
	});


	//ObjectUpdatePacket ó��
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		unsigned int ObjID = _Packet->GetObjectID();

		//�ش� NetObejctID�� ��ü�� �������� �ʴٸ� ����ٸ� ���⼭ ����������
		if (false == GameEngineNetObject::IsNetObject(ObjID))
		{
			std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
			NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, ObjID);
			NewNetObj->SetControll(NetControllType::NetControll);
		}

		//��Ŷ�� Deathó�� �� ���
		if (true == _Packet->IsDeath)
		{
			//��Ʈ��ũ�� ���� ����
			GameEngineNetObject* NetObject = nullptr;
			NetObject = GameEngineNetObject::GetNetObject(ObjID);
			NetObject->NetDisconnect();

			GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(NetObject);
			if (nullptr == ActorPtr)
			{
				MsgAssert("�ش� NetObejct�� GameEngineActorŸ������ ĳ�������� ���ؼ� Deathó���� �� �� �����ϴ�.");
				return;
			}
			ActorPtr->Death();
		}

		//��Ŷ�� Deathó�� ���� ���� ��쿡�� �����ʿ� ��Ŷ ����
		else
		{
			//Player�� ������ ó���� �� �ְ� �ڷᱸ���� ����
			GameEngineNetObject::PushNetObjectPacket(_Packet);
		}
	});



	//MessageChatPacket ó��
	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[](std::shared_ptr<MessageChatPacket> _Packet)
	{
		//NetworkGUI::GetInst()->PrintLog(_Packet->Message);
	});
	
}
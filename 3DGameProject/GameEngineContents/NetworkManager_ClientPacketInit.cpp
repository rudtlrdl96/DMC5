#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "NetworkGUI.h"
#include "NetworkObjectBase.h"

#include "ContentsEnum.h"
#include "PacketEnum.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"
#include "LinkObjectPacket.h"
#include "FsmChangePacket.h"

////////
//		Ŭ�� ��Ŷ �ʱ�ȭ
////////


void NetworkManager::ClientPacketInit()
{
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//ConnectIDPacket ó��
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	NetInst->Dispatcher.AddHandler<ConnectIDPacket>
		([=](std::shared_ptr<ConnectIDPacket> _Packet)
	{
		//�� Ŭ���̾�Ʈ�� ��Ʈ��ũ ���̵� ����
		NetID = _Packet->GetObjectID();

		if (nullptr != ConnectCallBack)
		{
			ConnectCallBack(NetID);
			ConnectCallBack = nullptr;
		}
		
		//���� ���� �޼��� ������
		const std::string& ClientName = NetworkGUI::GetInst()->GetNickName();
		std::string ChatMsg = ClientName + " Connect Server";
		PushChatPacket(ChatMsg, float4::GREEN);
	});



	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//ObjectUpdatePacket ó��
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	NetInst->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
	{
		if (Net_LevelType::UNKNOWN == _Packet->LevelType)
		{
			MsgAssert("���ŵ� UpdatePacket�� ���� Ÿ���� UNKNOWN�Դϴ�.");
			return;
		}

		//������ ���� ���� ��Ŷ�� ���� ������ �ٸ� ���
		if (CurLevelType != _Packet->LevelType)
		{
			NetworkGUI::GetInst()->PrintLog("UpdatePacket Receive To Other Level", float4::RED);
			return;
		}

		unsigned int ObjID = _Packet->GetObjectID();

		//�ش� NetObejctID�� ��ü�� �������� �ʴٸ� ����ٸ� ���⼭ ����������
		if (false == GameEngineNetObject::IsNetObject(ObjID))
		{
			std::shared_ptr<NetworkObjectBase> NewNetObj = nullptr;
			NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, nullptr, ObjID);
			NewNetObj->SetControll(NetControllType::PassiveControll);

			//����׿� Ÿ�� ����
			if (NetworkObjectBase::DebugType == NewNetObj->GetNetObjectType())
			{
				NetworkObjectBase::DebugTarget = NewNetObj.get();
			}


			//� Ÿ���� ���Ͱ� �����Ǿ����� GUI�� ���
			NetworkGUI::GetInst()->PrintLog("Create Object From UpdatePacket", float4::GREEN);
		}


		NetworkObjectBase* NetObject = nullptr;
		NetObject = NetworkObjectBase::GetNetObj(ObjID);

		//��Ŷ�� Deathó�� �� ���
		if (true == _Packet->IsDeath)
		{
			//��Ʈ��ũ�� ���� ����
			NetObject->NetDisconnect();

			GameEngineActor* ActorPtr = dynamic_cast<GameEngineActor*>(NetObject);
			if (nullptr == ActorPtr)
			{
				MsgAssert("�ش� NetObejct�� GameEngineActorŸ������ ĳ�������� ���ؼ� Deathó���� �� �� �����ϴ�.");
				return;
			}
			ActorPtr->Death();
			NetworkGUI::GetInst()->PrintLog("Destroy Actor From UpdatePacket", float4::GREEN);
		}

		//��Ŷ�� Deathó�� ���� ���� ��쿡�� �����ʿ� ��Ŷ ����
		else
		{
			NetObject->SetNetwortTransData(_Packet->Position, _Packet->Rotation);
			NetObject->ActorTimeScale = _Packet->TimeScale;
			NetObject->SetOnOffState(_Packet->IsUpdate);
			NetObject->SetUpdateArrData(_Packet);

			//Player�� ������ ó���� �� �ְ� �ڷᱸ���� ����
			GameEngineNetObject::PushNetObjectPacket(_Packet);
		}
	});



	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//MessageChatPacket ó��
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[](std::shared_ptr<MessageChatPacket> _Packet)
	{
		NetworkGUI::GetInst()->PrintLog(_Packet->Message, _Packet->Color);
	});
	


	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//LinkObjectPacket ó��
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	NetInst->Dispatcher.AddHandler<LinkObjectPacket>(
		[](std::shared_ptr<LinkObjectPacket> _Packet)
	{
		GameEngineNetObject* ObjPtr = reinterpret_cast<GameEngineNetObject*>(_Packet->Ptr);
		ObjPtr->InitNetObject(_Packet->GetObjectID(), NetInst);
		ObjPtr->SetUserControllType();
	});



	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//FsmChangePacket ó��
	//----------------------------------------------------------------------------------------------------------------------------------------------------

	NetInst->Dispatcher.AddHandler<FsmChangePacket>(
		[](std::shared_ptr<FsmChangePacket> _Packet)
	{
		unsigned int ObjID = _Packet->GetObjectID();

		//�ش� NetObejctID�� ��ü�� �������� �ʴ� �������� ����
		if (false == GameEngineNetObject::IsNetObject(ObjID))
			return;


		//Ŭ��� ���� ���� ��Ŷ�� ���� ������ �ٸ� ���
		if (CurLevelType != _Packet->LevelType)
			return;

		//��Ʈ��ũ ������ ���� ��쿡�� �������� ����
		GameEngineNetObject* NetObj = nullptr;
		NetObj = GameEngineNetObject::GetNetObject(ObjID);
		if (true == NetObj->IsNetDisconnected())
			return;

		//���� ������ ó���� �� �ְ� �ڷᱸ���� ����
		GameEngineNetObject::PushNetObjectPacket(_Packet);
	});
}
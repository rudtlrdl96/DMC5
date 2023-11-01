#include "PrecompileHeader.h"
#include "NetworkManager.h"

#include "NetworkGUI.h"
#include "NetworkObjectBase.h"

#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "MessageChatPacket.h"
#include "LinkObjectPacket.h"
#include "FsmChangePacket.h"
#include "NetEventPacket.h"

////////
//		���� ��Ŷ �ʱ�ȭ
////////

void NetworkManager::AcceptCallback(SOCKET _Socket, GameEngineNetServer* _Server)
{
	// ������ ������Ը� ����, ConnectIDPacket��Ŷ
	std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();
	++ClientCount;
	Packet->IsObserver = (1 < ClientCount);

	//��� ���� Ŭ���̾�Ʈ�� ��Ʈ��ũ ���̵� ����
	int ClientID = GameEngineNetObject::CreateServerID();
	Packet->SetObjectID(ClientID);

	//������ ID�� ������ �������� ����
	_Server->AddUser(ClientID, _Socket);

	//��� ���� Ŭ���̾�Ʈ���Ը� Ŀ��Ʈ ��Ŷ ������
	GameEngineSerializer Ser;
	Packet->SerializePacket(Ser);
	GameEngineNet::Send(_Socket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
}


void NetworkManager::ServerPacketInit()
{
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

		//Ŭ��� ���� ���� ��Ŷ�� ���� ������ �ٸ� ���
		if (CurLevelType != _Packet->LevelType)
		{
			NetworkGUI::GetInst()->PrintLog("UpdatePacket Receive To Other Level", float4::RED);
			return;
		}

		unsigned int ObjID = _Packet->GetObjectID();

		//�ش� NetObejctID�� ��ü�� �������� �ʴ� ��� ���⼭ ����������
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


			//� �����κ��� � Ÿ���� ���Ͱ� �����Ǿ����� GUI�� ���
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
			//GameEngineNetObject::PushNetObjectPacket(_Packet);
		}

		if (NetworkObjectBase::GetDebugType() == static_cast<Net_ActorType>(_Packet->ActorType))
		{
			NetworkGUI::GetInst()->PrintLog("Object with selected DebugType has successfully received UpdatePacket", float4::GREEN);
		}

		//������ ��쿣 ���Ź��� Ư�� ������Ʈ�� ��Ŷ�� �ٸ� Ŭ�� �� �ѷ��� �Ѵ�
		NetInst->SendPacket(_Packet, _Packet->NetID);
	});



	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//MessageChatPacket ó��
	//----------------------------------------------------------------------------------------------------------------------------------------------------

	NetInst->Dispatcher.AddHandler<MessageChatPacket>(
		[=](std::shared_ptr<MessageChatPacket> _Packet)
	{
		const std::string& Msg = _Packet->Message;
		NetGUI_MsgType MsgType = NetGUI_MsgType::Chat;
		if (SystemChatCheck.size() < Msg.size() && SystemChatCheck == Msg.substr(0, SystemChatCheck.size()))
		{
			MsgType = NetGUI_MsgType::System;
		}

		NetworkGUI::GetInst()->PrintLog(_Packet->Message, _Packet->Color, MsgType);

		//������ ��쿣 ���Ź��� Ư�� ������Ʈ�� ��Ŷ�� �ٸ� Ŭ�� �� �ѷ��� �Ѵ�
		NetInst->SendPacket(_Packet, _Packet->GetObjectID());
	});



	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//LinkObjectPacket ó��
	//----------------------------------------------------------------------------------------------------------------------------------------------------

	NetInst->Dispatcher.AddHandler<LinkObjectPacket>(
		[=](std::shared_ptr<LinkObjectPacket> _Packet)
	{
		unsigned int CliendID = _Packet->GetObjectID();
		unsigned int NewID = GameEngineNetObject::CreateServerID();

		//���Ź��� ������Ʈ ����
		//std::shared_ptr<GameEngineNetObject> NewNetObj = nullptr;
		//NewNetObj = NetworkManager::CreateNetActor(_Packet->ActorType, NewID);
		////������ ��Ʈ�� ���� ����
		//NewNetObj->SetControll(NetControllType::NetControll);


		//����� �� ��Ŷ
		std::shared_ptr<LinkObjectPacket> ReplyLinkPacket = std::make_shared<LinkObjectPacket>();
		//���������� ����� �� ������Ʈ ID ����
		ReplyLinkPacket->SetObjectID(NewID);
		ReplyLinkPacket->Ptr = _Packet->Ptr;

		//��Ŷ����ȭ
		GameEngineSerializer Ser;
		ReplyLinkPacket->SerializePacket(Ser);

		//������ ������ �������׸� ������Ŷ�� ������
		SOCKET ClientSocket = ServerInst.GetUser(CliendID);
		GameEngineNet::Send(ClientSocket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
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

		//Passive���� �Ѿ�� ��Ŷ�� ��쿣
		if (0 != _Packet->AttackerID)
		{
			//Active��Ʈ�� ��ü�� ��Ŷ�� ó���� �� �ְ���
			_Packet->SetActiveRecv();
		}

		//���� ������ ó���� �� �ְ� �ڷᱸ���� ����
		GameEngineNetObject::PushNetObjectPacket(_Packet);

		//������ ��쿣 ���Ź��� Ư�� ������Ʈ�� ��Ŷ�� �ٸ� Ŭ�� �� �ѷ��� �Ѵ�(�ѷ��� �ϳ�?)
		NetInst->SendPacket(_Packet, _Packet->NetID);
	});




	//----------------------------------------------------------------------------------------------------------------------------------------------------
	//NetEventPacketó��
	//----------------------------------------------------------------------------------------------------------------------------------------------------

	NetInst->Dispatcher.AddHandler<NetEventPacket>(
		[](std::shared_ptr<NetEventPacket> _Packet)
	{
		unsigned int ObjID = _Packet->GetObjectID();
		int EventType = _Packet->EventType;

		//���� ������Ʈ�� ��Ʈ��ũ �̺�Ʈ�� ���
		if (-1 != ObjID)
		{
			//Active���� ������ �ְ� ó��
			_Packet->SetActiveRecv();

			//���� ������ ó���� �� �ְ� �ڷᱸ���� ����
			GameEngineNetObject::PushNetObjectPacket(_Packet);
			return;
		}

		//������ ��Ʈ��ũ �̺�Ʈ�� ���
		if (true == AllNetEvent.empty() || true == AllNetEvent[EventType].empty())
		{
			NetworkGUI::GetInst()->PrintLog(GameEngineString::ToString(EventType) + " Event Not Exist!", float4::RED);
			return;
		}

		NetworkGUI* NetGUI = NetworkGUI::GetInst();
		std::string LogValue = GameEngineString::ToString(EventType) + " NetworkEvent be executed";

		const std::vector<std::function<void()>>& Events = AllNetEvent[EventType];
		for (const std::function<void()>& Event : Events)
		{
			Event();
			NetGUI->PrintLog(LogValue, float4::GREEN);
		}
	});
}


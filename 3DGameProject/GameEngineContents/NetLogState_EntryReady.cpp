#include "PrecompileHeader.h"
#include "NetLogState_EntryReady.h"

#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>
#include <GameEngineBase/GameEngineNetObject.h>

#include "NetLogViewer.h"
#include "ConnectIDPacket.h"

#include "Player.h"

NetLogState_EntryReady::NetLogState_EntryReady()
{

}

NetLogState_EntryReady::~NetLogState_EntryReady()
{

}


void NetLogState_EntryReady::Start() 
{
	NetLogStateBase::Start();

	BtnText_ForHost = GameEngineString::AnsiToUTF8("ȣ��Ʈ�� �÷���");
	BtnText_ForClient = GameEngineString::AnsiToUTF8("Ŭ���̾�Ʈ�� �÷���");
}


void NetLogState_EntryReady::Update(float _DeltaTime) 
{
	NetLogStateBase::Update(_DeltaTime);


	NetLogViewer* LogViewer = GetOwner();
	
	//ȣ��Ʈ ��ư�� ���� ���
	ImGui::InputInt("Port ", &PortNum);
	if (ImGui::Button(BtnText_ForHost.c_str()))
	{
		GameEngineNetServer& ServerNet = LogViewer->ServerNet;

		//������ ��Ŷ ó�� �ݹ� ���
		ServerPacketInit(ServerNet);

		//���� ����
		ServerNet.ServerOpen(static_cast<unsigned short>(PortNum));

		//Accept�ݹ� ���
		ServerNet.SetAcceptCallBack(NetLogState_EntryReady::NetAcceptCallBack);


		//���� �÷��̾ �ִ� ��� ������ ����(�����ڵ�)
		/*if (nullptr != Player::MainPlayer)
		{
			Player::MainPlayer->InitServerObject();
		}*/

		LogViewer->IsHostValue = true;
		LogViewer->NetPtr = &ServerNet;
		GetFSM()->ChangeState("NetLogState_LogPrint");
	}

	//Ŭ���̾�Ʈ ��ư�� ���� ���
	ImGui::InputText("IP", &IpNum[0], IpNum.size());
	if (ImGui::Button(BtnText_ForClient.c_str()))
	{
		GameEngineNetClient& ClientNet = LogViewer->ClientNet;

		//Ŭ��� ��Ŷ ó�� �ݹ� ���
		ClientPacketInit(ClientNet);

		//������ ���� �õ�
		bool IsConnect = ClientNet.Connect(IpNum, static_cast<unsigned short>(PortNum));
		if (false == IsConnect)
			return;

		LogViewer->IsHostValue = false;
		LogViewer->NetPtr = &ClientNet;
		GetFSM()->ChangeState("NetLogState_LogPrint");
	}
}



void NetLogState_EntryReady::NetAcceptCallBack(SOCKET _Socket, GameEngineNetServer* _Server)
{
	// ������ ������Ը� ����, ConnectIDPacket��Ŷ
	std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();

	int ID = GameEngineNetObject::CreateServerID();
	Packet->SetObjectID(ID);

	GameEngineSerializer Ser;
	Packet->SerializePacket(Ser);

	// �����ϰ� �ѹ� �� ���� ������ �Ἥ �������Ҷ�.
	GameEngineNet::Send(_Socket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
}

void NetLogState_EntryReady::ServerPacketInit(GameEngineNetServer& _Net)
{
	
}

void NetLogState_EntryReady::ClientPacketInit(GameEngineNetClient& _Net)
{
	_Net.Dispatcher.AddHandler<ConnectIDPacket>(PacketEnum::ConnectIDPacket,
		[](std::shared_ptr<ConnectIDPacket> _Packet)
		{
			//TODO
		}
	);


}


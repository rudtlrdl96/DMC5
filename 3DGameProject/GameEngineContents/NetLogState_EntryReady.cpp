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

	BtnText_ForHost = GameEngineString::AnsiToUTF8("호스트로 플레이");
	BtnText_ForClient = GameEngineString::AnsiToUTF8("클라이언트로 플레이");
}


void NetLogState_EntryReady::Update(float _DeltaTime) 
{
	NetLogStateBase::Update(_DeltaTime);


	NetLogViewer* LogViewer = GetOwner();
	
	//호스트 버튼을 누른 경우
	ImGui::InputInt("Port ", &PortNum);
	if (ImGui::Button(BtnText_ForHost.c_str()))
	{
		GameEngineNetServer& ServerNet = LogViewer->ServerNet;

		//서버용 패킷 처리 콜백 등록
		ServerPacketInit(ServerNet);

		//리슨 시작
		ServerNet.ServerOpen(static_cast<unsigned short>(PortNum));

		//Accept콜백 등록
		ServerNet.SetAcceptCallBack(NetLogState_EntryReady::NetAcceptCallBack);


		//메인 플레이어가 있는 경우 서버와 연결(예시코드)
		/*if (nullptr != Player::MainPlayer)
		{
			Player::MainPlayer->InitServerObject();
		}*/

		LogViewer->IsHostValue = true;
		LogViewer->NetPtr = &ServerNet;
		GetFSM()->ChangeState("NetLogState_LogPrint");
	}

	//클라이언트 버튼을 누른 경우
	ImGui::InputText("IP", &IpNum[0], IpNum.size());
	if (ImGui::Button(BtnText_ForClient.c_str()))
	{
		GameEngineNetClient& ClientNet = LogViewer->ClientNet;

		//클라용 패킷 처리 콜백 등록
		ClientPacketInit(ClientNet);

		//서버와 연결 시도
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
	// 접속한 사람에게만 보냄, ConnectIDPacket패킷
	std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();

	int ID = GameEngineNetObject::CreateServerID();
	Packet->SetObjectID(ID);

	GameEngineSerializer Ser;
	Packet->SerializePacket(Ser);

	// 유일하게 한번 딱 직접 소켓을 써서 보내야할때.
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


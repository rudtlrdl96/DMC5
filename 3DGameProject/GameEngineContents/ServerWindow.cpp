#include "PrecompileHeader.h"
#include "ServerWindow.h"
#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"

#include "ConnectIDPacket.h"
#include "Player.h"

GameEngineNet* ServerWindow::NetInst = nullptr;
bool ServerWindow::IsServer = false;
bool ServerWindow::IsClient = false;

ServerWindow::ServerWindow()
{
	IP.resize(1024);
	TestSendBuffer.resize(1024);
}

ServerWindow::~ServerWindow()
{
}



void ServerWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	//테스트 코드
	NowLevel = Level.get();

	// IP.resize(128);
	// ImGui::PushID(1312321);
	std::string Text = "서버로 동작중입니다.";

	if (true == IsServer)
	{
		ImGui::Text(GameEngineString::AnsiToUTF8(Text).c_str());

		ImGui::PushID(321312321);
		ImGui::InputText(Text.c_str(), &TestSendBuffer[0], TestSendBuffer.size());
		ImGui::PopID();

		ImGui::PushID(33333121);
		Text = "SendTest";
		if (ImGui::Button(GameEngineString::AnsiToUTF8(Text).c_str()))
		{
			size_t StrLen = strlen(TestSendBuffer.c_str());
			NetInst->Send(&TestSendBuffer[0], static_cast<unsigned int>(StrLen));
		}
		ImGui::PopID();

		return;
	}

	Text = "클라로 동작중입니다.";
	if (true == IsClient)
	{
		ImGui::Text(GameEngineString::AnsiToUTF8(Text).c_str());
		Text = "SendTest";

		// 처리하면 쉬워진다.
		ImGui::PushID(321312321);
		ImGui::InputText(Text.c_str(), &TestSendBuffer[0], TestSendBuffer.size());
		ImGui::PopID();

		ImGui::PushID(33333121);
		Text = "SendTest";
		if (ImGui::Button(GameEngineString::AnsiToUTF8(Text).c_str()))
		{
			size_t StrLen = strlen(TestSendBuffer.c_str());

			// TestSendBuffer = Arr;
			NetInst->Send(&TestSendBuffer[0], static_cast<unsigned int>(StrLen));
		}
		ImGui::PopID();
		return;
	}

	Text = "Port : ";
	ImGui::InputInt(Text.c_str(), &Port);
	Text = "호스트 하기";
	if (ImGui::Button(GameEngineString::AnsiToUTF8(Text).c_str()))
	{
		//Thread 이름 설정
		SetThreadDescription(GetCurrentThread(), L"Server Main Thread");

		//서버용 패킷 처리 콜백 등록
		ServerPacketInit(&Server);

		Server.ServerOpen(static_cast<unsigned short>(Port));
		ServerInit(Level);

		

		//메인 플레이어가 있는 경우 서버와 연결
		/*TestNeroMeshLoad();
		std::shared_ptr<BasePlayerActor> Player = nullptr;
		Player = NowLevel->CreateActor<BasePlayerActor>();
		Player->InitNetObject(GameEngineNetObject::CreateServerID(), &Server);*/

		if (nullptr != Player::MainPlayer)
		{
			Player::MainPlayer->InitNetObject(GameEngineNetObject::CreateServerID(), &Server);
		}

		NetInst = &Server;
		IsServer = true;
	}

	Text = "IP : ";
	// IP.resize(128);
	// ImGui::PushID(1312321);
	ImGui::InputText(Text.c_str(), &IP[0], IP.size());
	// ImGui::PopID();

	Text = "클라이언트로 접속하기";
	if (ImGui::Button(GameEngineString::AnsiToUTF8(Text).c_str()))
	{
		//Thead이름 설정
		SetThreadDescription(GetCurrentThread(), L"Client Main Thread");

		//클라용 패킷 처리 콜백 등록
		ClientPacketInit(&Client);

		IsClient = Client.Connect(IP, static_cast<unsigned short>(Port));
		NetInst = &Client;
	}
}

void ServerWindow::ServerInit(std::shared_ptr<GameEngineLevel> Level)
{
	//AcceptCallBack 설정
	Server.SetAcceptCallBack(
		[=](SOCKET _Socket, GameEngineNetServer* _Server)
		{
			// 접속한 사람에게만 보냄, ConnectIDPacket패킷
			std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();

			int ID = GameEngineNetObject::CreateServerID();
			Packet->SetObjectID(ID);

			GameEngineSerializer Ser;
			Packet->SerializePacket(Ser);

			//상대방의 ID와 소켓을 연관지어 저장
			_Server->AddUser(ID, _Socket);

			// 유일하게 한번 딱 직접 소켓을 써서 보내야할때.
			GameEngineNet::Send(_Socket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
		}
	);



}



void ServerWindow::ServerPacketInit(GameEngineNetServer* _Net)
{
	//ObjectUpdatePacket 처리
	_Net->Dispatcher.AddHandler<ObjectUpdatePacket>
		([=](std::shared_ptr<ObjectUpdatePacket> _Packet)
		{
			//해당 NetObejctID의 객체가 존재하지 않다면 만든다
			if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
			{
				std::shared_ptr<Player> NewPlayer = NowLevel->CreateActor<Player>();
				NewPlayer->InitNetObject(_Packet->GetObjectID(), _Net);
			}

			//Player가 스스로 처리할 수 있게 자료구조에 저장
			GameEngineNetObject::PushNetObjectPacket(_Packet);

			//서버의 경우엔 수신받은 특정 오브젝트의 패킷을 다른 클라에 다 뿌려야 한다
			_Net->SendPacket(_Packet, _Packet->GetObjectID());
		}
	);
}

void ServerWindow::ClientPacketInit(GameEngineNetClient* _Net)
{
	//ConnectIDPacket 처리
	_Net->Dispatcher.AddHandler<ConnectIDPacket>
		([=](std::shared_ptr<ConnectIDPacket> _Packet)
		{
			//처음 접속 했다면 Player를 서버와 연동 시작
			Player::MainPlayer->InitNetObject(_Packet->GetObjectID(), ServerWindow::NetInst);
		}
	);

	//ObjectUpdatePacket 처리
	_Net->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
		{
			//해당 NetObejctID의 객체가 존재하지 않다면 만든다
			if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
			{
				std::shared_ptr<Player> NewPlayer = NowLevel->CreateActor<Player>();
				NewPlayer->InitNetObject(_Packet->GetObjectID(), _Net);
			}

			//Player가 스스로 처리할 수 있게 자료구조에 저장
			GameEngineNetObject::PushNetObjectPacket(_Packet);
		}
	);
}


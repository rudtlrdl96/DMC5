#include "PrecompileHeader.h"
#include "ServerWindow.h"
#include "ConnectIDPacket.h"
#include "ObjectUpdatePacket.h"
#include "Player.h"

GameEngineNet* ServerWindow::NetInst = nullptr;

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
		Server.ServerOpen(static_cast<unsigned short>(Port));
		ServerInit(Level);
		IsServer = true;
		Player::MainPlayer->InitServerObject();
		NetInst = &Server;
	}

	Text = "IP : ";
	// IP.resize(128);
	// ImGui::PushID(1312321);
	ImGui::InputText(Text.c_str(), &IP[0], IP.size());
	// ImGui::PopID();

	Text = "클라이언트로 접속하기";
	if (ImGui::Button(GameEngineString::AnsiToUTF8(Text).c_str()))
	{
		IsClient = Client.Connect(IP, static_cast<unsigned short>(Port));
		NetInst = &Client;
	}
}

void ServerWindow::ServerInit(std::shared_ptr<GameEngineLevel> Level)
{
	//AcceptCallBack 설정
	Server.SetAcceptCallBack(
		[=](SOCKET, GameEngineNetServer* _Server)
		{
			//연결된 클라에게 ConnectIDPacket 보내기
			std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();
			std::shared_ptr<Player> NewPlayer = Level->CreateActor<Player>();
		}
	);



}

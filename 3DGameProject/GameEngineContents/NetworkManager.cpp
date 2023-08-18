#include "PrecompileHeader.h"
#include "NetworkManager.h"

GameEngineNet* NetworkManager::NetInst = nullptr;
GameEngineNetServer NetworkManager::ServerInst;
GameEngineNetClient NetworkManager::ClientInst;

bool NetworkManager::IsServerValue = false;
bool NetworkManager::IsClientValue = false;



void NetworkManager::ServerOpen(int _Port)
{
	//Thread 이름 설정
	SetThreadDescription(GetCurrentThread(), L"Server Main Thread");

	NetInst = &ServerInst;
	
	//클라이언트가 Accept될 때 처리해야 할 작업 등록
	ServerInst.SetAcceptCallBack(AcceptCallback);
	
	//서버용 패킷 처리 콜백 등록
	ServerPacketInit();

	//서버 오픈
	ServerInst.ServerOpen(static_cast<unsigned short>(_Port));
	IsServerValue = true;
}


bool NetworkManager::ConnectServer(const std::string_view& _IP, int _Port)
{
	//Thread 이름 설정
	SetThreadDescription(GetCurrentThread(), L"Client Main Thread");

	NetInst = &ClientInst;

	//클라용 패킷 처리 콜백 등록
	ClientPacketInit();

	//서버 접속
	IsClientValue = ClientInst.Connect(_IP.data(), static_cast<unsigned short>(_Port));
	return IsClientValue;
}

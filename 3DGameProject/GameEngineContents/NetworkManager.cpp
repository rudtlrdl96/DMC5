#include "PrecompileHeader.h"
#include "NetworkManager.h"

GameEngineNet* NetworkManager::NetInst = nullptr;
GameEngineNetServer NetworkManager::ServerInst;
GameEngineNetClient NetworkManager::ClientInst;

bool NetworkManager::IsServerValue = false;
bool NetworkManager::IsClientValue = false;



void NetworkManager::ServerOpen(int _Port)
{
	//Thread �̸� ����
	SetThreadDescription(GetCurrentThread(), L"Server Main Thread");

	NetInst = &ServerInst;
	
	//Ŭ���̾�Ʈ�� Accept�� �� ó���ؾ� �� �۾� ���
	ServerInst.SetAcceptCallBack(AcceptCallback);
	
	//������ ��Ŷ ó�� �ݹ� ���
	ServerPacketInit();

	//���� ����
	ServerInst.ServerOpen(static_cast<unsigned short>(_Port));
	IsServerValue = true;
}


bool NetworkManager::ConnectServer(const std::string_view& _IP, int _Port)
{
	//Thread �̸� ����
	SetThreadDescription(GetCurrentThread(), L"Client Main Thread");

	NetInst = &ClientInst;

	//Ŭ��� ��Ŷ ó�� �ݹ� ���
	ClientPacketInit();

	//���� ����
	IsClientValue = ClientInst.Connect(_IP.data(), static_cast<unsigned short>(_Port));
	return IsClientValue;
}

#include "PrecompileHeader.h"
#include "GameEngineNetClient.h"
#include "GameEngineDebug.h"

GameEngineNetClient::GameEngineNetClient()
{
}

GameEngineNetClient::~GameEngineNetClient()
{
    if (0 != ClientSocket)
    {
        closesocket(ClientSocket);
    }
}

void GameEngineNetClient::Send(void* Data, unsigned int _Size)
{

}

bool GameEngineNetClient::Connect(const std::string& _IP, unsigned short _Port)
{
    // �� ���α׷��� ��Ʈ��ũ�� ����ҷ���.
    WSAData WsaData;

    // �� ���α׷��� �����쿡�� ������ ����ϰڴٰ� �˷��ִ� �̴ϴ�.
    int errorCode = WSAStartup(MAKEWORD(2, 2), &WsaData);
    if (SOCKET_ERROR == errorCode)
    {
        MsgAssert("socket Error");
        return false;
    }

    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (INVALID_SOCKET == ClientSocket)
    {
        return false;
    }

    SOCKADDR_IN ClientAdd;

    ClientAdd.sin_family = AF_INET;
    ClientAdd.sin_port = htons(30000);

    // ����ȣ��Ʈ ip ����ǻ�Ϳ� ���� �����ϰڴ�.
    std::string IP = _IP;

    if (SOCKET_ERROR == inet_pton(AF_INET, IP.c_str(), &ClientAdd.sin_addr))
    {
        return false;
    }

    int Len = sizeof(SOCKADDR_IN);

    // ���� �ð��� �ʰ��߽��ϴ�.
    if (SOCKET_ERROR == connect(ClientSocket, (const sockaddr*)&ClientAdd, Len))
    {
        MsgAssert("Ŀ��Ʈ�� �����߽��ϴ�.");
        return false;
    }

    RecvThread.Start("Client Recv Thread", std::bind(&GameEngineNet::RecvThreadFunction, ClientSocket, this));

    return true;
}
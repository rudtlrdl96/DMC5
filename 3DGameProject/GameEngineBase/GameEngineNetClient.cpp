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
    //네트워크를 사용하겠다 OS에 알림
    WSAData WsaData;

    // 이 프로그램이 윈도우에게 서버를 사용하겠다고 알려준다
    int errorCode = WSAStartup(MAKEWORD(2, 2), &WsaData);
    if (SOCKET_ERROR == errorCode)
    {
        MsgAssert("socket Error");
        return false;
    }

    //TCP로 소켓 생성
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == ClientSocket)
    {
        return false;
    }


    SOCKADDR_IN ClientAdd;
    ClientAdd.sin_family = AF_INET;         // ip4주소 체계를 쓰겠다.
    ClientAdd.sin_port = htons(30000);     // htons : 네트워크 통신에 유효한 에디안 방식으로 만들어준다. (기본적으로 네트워크는 빅 에디안)

    std::string IP = _IP;
    if (SOCKET_ERROR == inet_pton(AF_INET, IP.c_str(), &ClientAdd.sin_addr))
    {
        return false;
    }

    int Len = sizeof(SOCKADDR_IN);

    // 이 소켓으로 서버에 연결 시도
    if (SOCKET_ERROR == connect(ClientSocket, (const sockaddr*)&ClientAdd, Len))
    {
        MsgAssert("커넥트에 실패했습니다.");
        return false;
    }

    //Recv스레드 시작
    RecvThread.Start("Client Recv Thread", std::bind(&GameEngineNet::RecvThreadFunction, ClientSocket, this));

    return true;
}
#include "PrecompileHeader.h"
#include "GameEngineNetServer.h"
#include "GameEngineDebug.h"

void GameEngineNetServer::AcceptThread(SOCKET _AcceptSocket, GameEngineNetServer* _Net)
{
    int AddressLen = sizeof(SOCKADDR_IN);
    while (true == _Net->IsNet())
    {
        SOCKADDR_IN ClientAdd;

        memset(&ClientAdd, 0, sizeof(ClientAdd));

        //서버에 입장한 클라 소켓이 저장되는 자료구조가
        //비여있지 않다면 하나를 꺼내온다
        SOCKET CientSocket = accept(_AcceptSocket, (sockaddr*)&ClientAdd, &AddressLen);
        if (SOCKET_ERROR == CientSocket || INVALID_SOCKET == CientSocket)
        {
            return;
        }

        //꺼내온 클라 소켓으로부터 데이터를 수신할 Recv스레드를 만들고 벡터에 저장
        std::shared_ptr<GameEngineThread> NewThread = std::make_shared<GameEngineThread>();
        _Net->RecvThreads.push_back(NewThread);

        //Recv스레드 시작1
        std::string ThreadName = std::to_string(CientSocket);
        ThreadName += "Server Recv Thread";

        //클라가 서버에 연결되었을 때 처리할 콜백이 존재한다면
        if (nullptr != _Net->AcceptCallBack)
        {
            //해당 소켓과 서버의 포인터를 인자로 호출
            _Net->AcceptCallBack(CientSocket, _Net);
        }

        ////Recv스레드 시작2
        NewThread->Start(ThreadName, std::bind(&GameEngineNet::RecvThreadFunction, CientSocket, _Net));
    }

    int a = 0;
}

GameEngineNetServer::GameEngineNetServer()
{
}

GameEngineNetServer::~GameEngineNetServer()
{
    if (0 != AcceptSocket)
    {
        closesocket(AcceptSocket);
    }
}

void GameEngineNetServer::Send(const char* Data, unsigned int _Size, int _IgnoreID /*= -1*/)
{
    //이 서버와 연결된 모든 클라에게 바이트 전송
    for(std::pair<int, SOCKET> UserPair : Users)
    {
        //IgnoreID는 Continue
        if (_IgnoreID == UserPair.first)
            continue;

        send(UserPair.second, Data, _Size, 0);
    }
}

void GameEngineNetServer::ServerOpen(short _Port, int _BackLog)
{
    WSAData WsaData;

    //네트워크를 사용하겠다 OS에 알림
    int errorCode = WSAStartup(MAKEWORD(2, 2), &WsaData);
    if (SOCKET_ERROR == errorCode)
    {
        MsgAssert("socket Error");
        return;
    }

    SOCKADDR_IN Add;
    Add.sin_family = AF_INET; // ip4주소 체계를 쓰겠다.
    Add.sin_port = htons(_Port); // htons : 네트워크 통신에 유효한 에디안 방식으로 만들어준다. (기본적으로 네트워크는 빅 에디안)
    //"0.0.0.0"는 자기자신의 IP로 서버를 열겠다는 의미
    if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &Add.sin_addr))
    {
        return;
    }

    //TCP로 소켓 생성
    AcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == AcceptSocket)
    {
        return;
    }

    //만든 소켓을 Add의 설정(포트번호)으로 서버를 열 것이라고 설정
    if (SOCKET_ERROR == bind(AcceptSocket, (const sockaddr*)&Add, sizeof(SOCKADDR_IN)))
    {
        return;
    }

    BackLog = _BackLog;

    //서버 오픈, 이때부터 클라가 서버에 연결될 수 있음
    if (SOCKET_ERROR == listen(AcceptSocket, _BackLog))
    {
        return;
    }

    //클라 입장 스레드 생성
    AccpetThread.Start("AcceptFunction", std::bind(GameEngineNetServer::AcceptThread, AcceptSocket, this));
}

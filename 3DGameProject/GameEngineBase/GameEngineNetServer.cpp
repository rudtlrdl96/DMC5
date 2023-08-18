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

        //������ ������ Ŭ�� ������ ����Ǵ� �ڷᱸ����
        //������ �ʴٸ� �ϳ��� �����´�
        SOCKET CientSocket = accept(_AcceptSocket, (sockaddr*)&ClientAdd, &AddressLen);
        if (SOCKET_ERROR == CientSocket || INVALID_SOCKET == CientSocket)
        {
            return;
        }

        //������ Ŭ�� �������κ��� �����͸� ������ Recv�����带 ����� ���Ϳ� ����
        std::shared_ptr<GameEngineThread> NewThread = std::make_shared<GameEngineThread>();
        _Net->RecvThreads.push_back(NewThread);

        //Recv������ ����1
        std::string ThreadName = std::to_string(CientSocket);
        ThreadName += "Server Recv Thread";

        //Ŭ�� ������ ����Ǿ��� �� ó���� �ݹ��� �����Ѵٸ�
        if (nullptr != _Net->AcceptCallBack)
        {
            //�ش� ���ϰ� ������ �����͸� ���ڷ� ȣ��
            _Net->AcceptCallBack(CientSocket, _Net);
        }

        ////Recv������ ����2
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
    //�� ������ ����� ��� Ŭ�󿡰� ����Ʈ ����
    for (size_t i = 0; i < Users.size(); i++)
    {
        //TODO : IgnoreID�� Continue

        send(Users[static_cast<int>(i)], Data, _Size, 0);
    }
}

void GameEngineNetServer::ServerOpen(short _Port, int _BackLog)
{
    WSAData WsaData;

    //��Ʈ��ũ�� ����ϰڴ� OS�� �˸�
    int errorCode = WSAStartup(MAKEWORD(2, 2), &WsaData);
    if (SOCKET_ERROR == errorCode)
    {
        MsgAssert("socket Error");
        return;
    }

    SOCKADDR_IN Add;
    Add.sin_family = AF_INET; // ip4�ּ� ü�踦 ���ڴ�.
    Add.sin_port = htons(_Port); // htons : ��Ʈ��ũ ��ſ� ��ȿ�� ����� ������� ������ش�. (�⺻������ ��Ʈ��ũ�� �� �����)
    //"0.0.0.0"�� �ڱ��ڽ��� IP�� ������ ���ڴٴ� �ǹ�
    if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &Add.sin_addr))
    {
        return;
    }

    //TCP�� ���� ����
    AcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == AcceptSocket)
    {
        return;
    }

    //���� ������ Add�� ����(��Ʈ��ȣ)���� ������ �� ���̶�� ����
    if (SOCKET_ERROR == bind(AcceptSocket, (const sockaddr*)&Add, sizeof(SOCKADDR_IN)))
    {
        return;
    }

    BackLog = _BackLog;

    //���� ����, �̶����� Ŭ�� ������ ����� �� ����
    if (SOCKET_ERROR == listen(AcceptSocket, _BackLog))
    {
        return;
    }

    //Ŭ�� ���� ������ ����
    AccpetThread.Start("AcceptFunction", std::bind(GameEngineNetServer::AcceptThread, AcceptSocket, this));
}

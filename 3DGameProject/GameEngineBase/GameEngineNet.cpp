#include "PrecompileHeader.h"
#include "GameEngineNet.h"
#include "GameEngineSerializer.h"

GameEngineNet::GameEngineNet()
{
}

GameEngineNet::~GameEngineNet()
{
	IsNetValue = false;
}

void GameEngineNet::RecvThreadFunction(SOCKET _Socket, GameEngineNet* _Net)
{
	char Data[1024] = { 0 };
	GameEngineSerializer Serializer;

	int PacketType = -1;
	int PacketSize = -1;
	while (true == _Net->IsNet())
	{
		// 상대가 보내온 패킷의 양. (-1인 경우 끊김을 의미)
		int Result = recv(_Socket, Data, sizeof(Data), 0);

		if (-1 == Result)
		{
			return;
		}

	}
}


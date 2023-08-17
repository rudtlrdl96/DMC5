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

		//접속 끊김
		if (-1 == Result)
		{
			return;
		}

		// 접속 끊김
		if (SOCKET_ERROR == _Socket || INVALID_SOCKET == _Socket)
		{
			return;
		}


		//여태까지 받은 바이트를 읽어보기
		Serializer.Write(Data, Result);
		//아직 패킷을 분석할 정도의 바이트 크기가 아니라면 다시 수신대기
		if (8 > Serializer.GetWriteOffSet())
		{
			/*
				TCP는 언제나 바이트가 짤려서 오거나 다음 패킷과 같이
				붙어서 올 수가 있다는 점을 유의하자
			*/
			continue;
		}


		// 8바이트 이상 받았고
		// 아직 패킷타입이 뭔지 알아내지 못했다면(PacketType 초기값이 -1)
		if (-1 == PacketType)
		{
			// 패킷타입 알아낸다.(앞쪽 4바이트 분석)
			{
				unsigned char* TypePivotPtr = &Serializer.GetDataPtr()[0];
				int* ConvertPtr = reinterpret_cast<int*>(TypePivotPtr);
				PacketType = *ConvertPtr;
			}

			// 사이즈 알아내고(4~8의 바이트 분석)
			{
				unsigned char* SizePivotPtr = &Serializer.GetDataPtr()[4];
				int* ConvertPtr = reinterpret_cast<int*>(SizePivotPtr);
				PacketSize = *ConvertPtr;
			}
		}

		// 패킷사이즈가 말도 안되는 크기인 경우(오류)
		if (-1 == PacketSize)
		{
			return;
		}

		// 8바이트 이상 받았지만
		// 그걸 통해서 알아낸 패킷의 크기보다는 덜 온 경우
		//if (static_cast<unsigned int>(PacketSize) > Serializer.GetWriteOffSet())
		if (PacketSize > Serializer.GetWriteOffSet())
		{
			//다음 수신을 대기
			continue;
		}

		//2개 이상의 패킷이 한번에 올 수도 있기 때문에
		//들어온 패킷을 모두 처리하기 위해 while문 안에서 패킷을 분석
		while (true)
		{
			std::shared_ptr<GameEnginePacket> Packet = _Net->Dispatcher.ConvertPacket(PacketType, Serializer);

			//이거 일단 임시
			_Net->Dispatcher.ProcessPacket(Packet);

			//TODO
		}
	}
}

void GameEngineNet::SendPacket(std::shared_ptr<class GameEnginePacket> _Packet)
{
	GameEngineSerializer Ser;
	_Packet->SerializePacket(Ser);
	Send(reinterpret_cast<const char*>(Ser.GetDataPtr()), Ser.GetWriteOffSet());
}


void GameEngineNet::Send(SOCKET _Socket, const char* Data, unsigned int _Size)
{
	send(_Socket, Data, _Size, 0);
}


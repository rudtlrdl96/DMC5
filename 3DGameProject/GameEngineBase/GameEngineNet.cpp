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

	unsigned int PacketType = -1;
	unsigned int PacketSize = -1;
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
				unsigned int* ConvertPtr = reinterpret_cast<unsigned int*>(TypePivotPtr);
				PacketType = *ConvertPtr;
			}

			// 사이즈 알아내고(4~8의 바이트 분석)
			{
				unsigned char* SizePivotPtr = &Serializer.GetDataPtr()[4];
				unsigned int* ConvertPtr = reinterpret_cast<unsigned int*>(SizePivotPtr);
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
		//if (PacketSize > static_cast<int>(Serializer.GetWriteOffSet()))
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

			//바이트를 정상적으로 패킷으로 바꾸었다면
			//메인스레드에서 패킷처리를 하기위해 자료구조에 저장
			if (nullptr != Packet)
			{
				_Net->RecvPacketLock.lock();
				_Net->RecvPacket.push_back(Packet);
				_Net->RecvPacketLock.unlock();
			}
			else
			{
				MsgAssert("알 수 없는 패킷 타입 : " + std::to_string(PacketType));
				return;
			}

			//수신받은 모든 바이트를 패킷크기에 딱 맞게 처리한 경우
			if (Serializer.GetWriteOffSet() == Serializer.GetReadOffSet())
			{
				Serializer.Reset();
				PacketType = -1;
				PacketSize = -1;
				break;
			}

			//아직 처리해야 하는 바이트가 남은 경우
			else
			{
				unsigned int RemainSize = Serializer.GetWriteOffSet() - Serializer.GetReadOffSet();

				//남은 바이트가 8바이트 미만인 경우 다시 데이터 수신을 대기한다(23번째 줄로 이동)
				if (8 > RemainSize)
				{
					//직렬화 버퍼를 정리하고(앞쪽으로 땡기고) 다시 데이터 수신
					Serializer.ClearReadData();
					break;
				}


				//패킷 타입 분석
				{
					unsigned char* TypePivotPtr = &Serializer.GetDataPtr()[0];
					unsigned int* ConvertPtr = reinterpret_cast<unsigned int*>(TypePivotPtr);
					PacketType = *ConvertPtr;
				}

				//패킷 사이즈 분석
				{
					unsigned char* SizePivotPtr = &Serializer.GetDataPtr()[4];
					unsigned int* ConvertPtr = reinterpret_cast<unsigned int*>(SizePivotPtr);
					PacketSize = *ConvertPtr;
				}

				//남은 데이터가 8바이트 보다는 적은 경우엔
				if (RemainSize < PacketSize)
				{
					//직렬화 버퍼를 정리하고(앞쪽으로 땡기고) 다시 데이터 수신
					Serializer.ClearReadData();
					break;
				}

				//지금 당장 붙어있던 패킷을 처리할 수 있던 경우엔
				//지금 While문으로 이동해서 바이트를 패킷으로 변환
			}
		}

	}
}

void GameEngineNet::SendPacket(std::shared_ptr<GameEnginePacket> _Packet, int _IgnoreID)
{
	GameEngineSerializer Ser;
	_Packet->SerializePacket(Ser);
	Send(reinterpret_cast<const char*>(Ser.GetDataPtr()), Ser.GetWriteOffSet(), _IgnoreID);
}


//얘는 소켓을 직접 지정해서 한명에게만 보내는 static Send
void GameEngineNet::Send(SOCKET _Socket, const char* Data, unsigned int _Size)
{
	send(_Socket, Data, _Size, 0);
}



void GameEngineNet::UpdatePacket()
{
	RecvPacketLock.lock();

	//수신 받은 데이터가 없을때는 그냥 return
	if (0 >= RecvPacket.size())
	{
		RecvPacketLock.unlock();
		return;
	}

	//수신받은 패킷 옮기기
	ProcessPackets = RecvPacket;
	RecvPacket.clear();
	RecvPacketLock.unlock();

	//패킷 처리하기
	for (std::shared_ptr<GameEnginePacket> Packet : ProcessPackets)
	{
		Dispatcher.ProcessPacket(Packet);
	}
	ProcessPackets.clear();
}

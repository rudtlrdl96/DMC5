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
		// ��밡 ������ ��Ŷ�� ��. (-1�� ��� ������ �ǹ�)
		int Result = recv(_Socket, Data, sizeof(Data), 0);

		//���� ����
		if (-1 == Result)
		{
			return;
		}

		// ���� ����
		if (SOCKET_ERROR == _Socket || INVALID_SOCKET == _Socket)
		{
			return;
		}


		//���±��� ���� ����Ʈ�� �о��
		Serializer.Write(Data, Result);
		//���� ��Ŷ�� �м��� ������ ����Ʈ ũ�Ⱑ �ƴ϶�� �ٽ� ���Ŵ��
		if (8 > Serializer.GetWriteOffSet())
		{
			/*
				TCP�� ������ ����Ʈ�� ©���� ���ų� ���� ��Ŷ�� ����
				�پ �� ���� �ִٴ� ���� ��������
			*/
			continue;
		}


		// 8����Ʈ �̻� �޾Ұ�
		// ���� ��ŶŸ���� ���� �˾Ƴ��� ���ߴٸ�(PacketType �ʱⰪ�� -1)
		if (-1 == PacketType)
		{
			// ��ŶŸ�� �˾Ƴ���.(���� 4����Ʈ �м�)
			{
				unsigned char* TypePivotPtr = &Serializer.GetDataPtr()[0];
				int* ConvertPtr = reinterpret_cast<int*>(TypePivotPtr);
				PacketType = *ConvertPtr;
			}

			// ������ �˾Ƴ���(4~8�� ����Ʈ �м�)
			{
				unsigned char* SizePivotPtr = &Serializer.GetDataPtr()[4];
				int* ConvertPtr = reinterpret_cast<int*>(SizePivotPtr);
				PacketSize = *ConvertPtr;
			}
		}

		// ��Ŷ����� ���� �ȵǴ� ũ���� ���(����)
		if (-1 == PacketSize)
		{
			return;
		}

		// 8����Ʈ �̻� �޾�����
		// �װ� ���ؼ� �˾Ƴ� ��Ŷ�� ũ�⺸�ٴ� �� �� ���
		//if (PacketSize > static_cast<int>(Serializer.GetWriteOffSet()))
		if (static_cast<unsigned int>(PacketSize) > Serializer.GetWriteOffSet())
		{
			//���� ������ ���
			continue;
		}

		//2�� �̻��� ��Ŷ�� �ѹ��� �� ���� �ֱ� ������
		//���� ��Ŷ�� ��� ó���ϱ� ���� while�� �ȿ��� ��Ŷ�� �м�
		while (true)
		{
			std::shared_ptr<GameEnginePacket> Packet = _Net->Dispatcher.ConvertPacket(PacketType, Serializer);

			//����Ʈ�� ���������� ��Ŷ���� �ٲپ��ٸ�
			//���ν����忡�� ��Ŷó���� �ϱ����� �ڷᱸ���� ����
			if (nullptr != Packet)
			{
				_Net->RecvPacketLock.lock();
				_Net->RecvPacket.push_back(Packet);
				_Net->RecvPacketLock.unlock();
			}

			//���Ź��� ��� ����Ʈ�� ��Ŷũ�⿡ �� �°� ó���� ���
			if (Serializer.GetWriteOffSet() == Serializer.GetReadOffSet())
			{
				Serializer.Reset();
				PacketType = -1;
				PacketSize = -1;
				break;
			}

			//���� ó���ؾ� �ϴ� ����Ʈ�� ���� ���
			else
			{
				unsigned int RemainSize = Serializer.GetWriteOffSet() - Serializer.GetReadOffSet();

				//���� ����Ʈ�� 8����Ʈ �̸��� ��� �ٽ� ������ ������ ����Ѵ�(23��° �ٷ� �̵�)
				if (8 > RemainSize)
				{
					break;
				}


				//��Ŷ Ÿ�� �м�
				{
					unsigned char* TypePivotPtr = &Serializer.GetDataPtr()[0];
					int* ConvertPtr = reinterpret_cast<int*>(TypePivotPtr);
					PacketType = *ConvertPtr;
				}

				//��Ŷ ������ �м�
				{
					unsigned char* SizePivotPtr = &Serializer.GetDataPtr()[4];
					int* ConvertPtr = reinterpret_cast<int*>(SizePivotPtr);
					PacketSize = *ConvertPtr;
				}

				//���� �����Ͱ� 8����Ʈ ���ٴ� ���� ��쿣
				if (static_cast<int>(RemainSize) < PacketSize)
				{
					//����ȭ ���۸� �����ϰ�(�������� �����) �ٽ� ������ ����
					Serializer.ClearReadData();
					break;
				}

				//���� ���� �پ��ִ� ��Ŷ�� ó���� �� �ִ� ��쿣
				//���� While������ �̵��ؼ� ����Ʈ�� ��Ŷ���� ��ȯ
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


//��� ������ ���� �����ؼ� �Ѹ��Ը� ������ static Send
void GameEngineNet::Send(SOCKET _Socket, const char* Data, unsigned int _Size)
{
	send(_Socket, Data, _Size, 0);
}



void GameEngineNet::UpdatePacket()
{
	RecvPacketLock.lock();

	//���� ���� �����Ͱ� �������� �׳� return
	if (0 >= RecvPacket.size())
	{
		RecvPacketLock.unlock();
		return;
	}

	//���Ź��� ��Ŷ �ű��
	ProcessPackets = RecvPacket;
	RecvPacket.clear();
	RecvPacketLock.unlock();

	//��Ŷ ó���ϱ�
	for (std::shared_ptr<GameEnginePacket> Packet : ProcessPackets)
	{
		Dispatcher.ProcessPacket(Packet);
	}
	ProcessPackets.clear();
}

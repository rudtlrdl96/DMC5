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
		//if (static_cast<unsigned int>(PacketSize) > Serializer.GetWriteOffSet())
		if (PacketSize > Serializer.GetWriteOffSet())
		{
			//���� ������ ���
			continue;
		}

		//2�� �̻��� ��Ŷ�� �ѹ��� �� ���� �ֱ� ������
		//���� ��Ŷ�� ��� ó���ϱ� ���� while�� �ȿ��� ��Ŷ�� �м�
		while (true)
		{
			std::shared_ptr<GameEnginePacket> Packet = _Net->Dispatcher.ConvertPacket(PacketType, Serializer);

			//�̰� �ϴ� �ӽ�
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


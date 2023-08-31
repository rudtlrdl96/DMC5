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
	char Data[4096] = { 0 };
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

		
		//���±��� ���� ����Ʈ�� �����ؼ� 8����Ʈ �̸��̸� ������ ���� ���,
		//8����Ʈ �̻��̸� ��Ŷ Ÿ�԰� ����� �����Ѵ�
		if (false == SearchPacketData(Serializer, PacketType, PacketSize))
			continue;


		// ��Ŷ����� ���� �ȵǴ� ũ���� ���(����)
		if (-1 == PacketSize)
		{
			MsgAssert("��Ŷ ����� -1�� ���Խ��ϴ�");
			return;
		}

		// 8����Ʈ �̻� �޾�����
		// �װ� ���ؼ� �˾Ƴ� ��Ŷ�� ũ�⺸�ٴ� �� �� ���
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

			//����Ʈ�� ���������� ��Ŷ���� �ٲپ��ٸ�
			//���ν����忡�� ��Ŷó���� �ϱ����� �ڷᱸ���� ����
			if (nullptr != Packet)
			{
				_Net->RecvPacketLock.lock();
				_Net->RecvPacket.push_back(Packet);
				_Net->RecvPacketLock.unlock();
			}
			else
			{
				MsgAssert("�� �� ���� ��Ŷ Ÿ�� : " + std::to_string(PacketType));
				return;
			}

			//���� ��Ŷ�� ó���߱� ������ Ÿ�԰� ������ ����
			PacketType = -1;
			PacketSize = -1;

			//���Ź��� ��� ����Ʈ�� ��Ŷũ�⿡ �� �°� ó���� ���
			if (Serializer.GetWriteOffSet() == Serializer.GetReadOffSet())
			{
				Serializer.Reset();
				break;
			}

			//���� ó���ؾ� �ϴ� ����Ʈ�� ���� ���
			else
			{
				//���� ��Ŷ�� ������ �� ���� Byteũ���� ��쿣 ������ ���� ���,
				//������ �� ���� Byteũ���϶� ��Ŷ Ÿ�԰� ����� �˾Ƴ���
				if (false == SearchPacketData(Serializer, PacketType, PacketSize))
					break;

				//����ȭ ���ۿ� �����ִ� ����Ʈ ũ��
				unsigned int RemainSize = Serializer.GetWriteOffSet() - Serializer.GetReadOffSet();

				//���� �����Ͱ� 8����Ʈ ���ٴ� ���� ��쿣 ������ ���� ���
				if (RemainSize < PacketSize)
					break;

				//���� ���� �پ��ִ� ��Ŷ�� ó���� �� �ִ� ��쿣
				//���� While������ �̵��ؼ� ����Ʈ�� ��Ŷ���� ��ȯ
			}
		}

	}
}



bool GameEngineNet::SearchPacketData(GameEngineSerializer& _Ser, unsigned int& _PacketType, unsigned int& _PacketSize)
{
	//����Ʈ ���۰� ���������� �Ǿ����� ���� ��쿣 ���� �����Ѵ�
	if (0 != _Ser.GetReadOffSet())
	{
		_Ser.ClearReadData();
	}
	
	//�о���� ����Ʈ�� 8����Ʈ �̸��϶� �������� ����
	unsigned int BuffSize = _Ser.GetWriteOffSet() - _Ser.GetReadOffSet();
	if (BuffSize < 8)
		return false;


	// ��ŶŸ�� �˾Ƴ���.(���� 4����Ʈ �м�)
	if (-1 == _PacketType)
	{
		//���� ����ȭ�� Read�ϴ� �κ��� ��Ŷ�ʿ��� �̷����, ������ ���縸
		unsigned char* TypePivotPtr = &_Ser.GetDataPtr()[0];
		unsigned int* ConvertPtr = reinterpret_cast<unsigned int*>(TypePivotPtr);
		_PacketType = *ConvertPtr;
	}

	// ��Ŷ����� �˾Ƴ���.(4~8�� ����Ʈ �м�)
	if (-1 == _PacketSize)
	{
		//���� ����ȭ�� Read�ϴ� �κ��� ��Ŷ�ʿ��� �̷����, ������ ���縸
		unsigned char* SizePivotPtr = &_Ser.GetDataPtr()[4];
		unsigned int* ConvertPtr = reinterpret_cast<unsigned int*>(SizePivotPtr);
		_PacketSize = *ConvertPtr;
	}

	return true;
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

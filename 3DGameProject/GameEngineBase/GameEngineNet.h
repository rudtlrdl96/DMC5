#pragma once

#include <atomic>
#include <mutex>
#include "GameEngineDispatcher.h"

//������ Ŭ�� �������� ����ϴ� Ŭ����
class GameEngineNet
{
public:
	// constrcuter destructer
	GameEngineNet();
	~GameEngineNet();

	// delete Function
	GameEngineNet(const GameEngineNet& _Other) = delete;
	GameEngineNet(GameEngineNet&& _Other) noexcept = delete;
	GameEngineNet& operator=(const GameEngineNet& _Other) = delete;
	GameEngineNet& operator=(GameEngineNet&& _Other) noexcept = delete;

	//Ư�� ��뿡�� �����͸� �����ϴ� �Լ�(������ Accept�����忡�� Ŭ��� ó�� ����� �� ���)
	static void Send(SOCKET _Socket, const char* Data, unsigned int _Size);

	virtual void Send(const char* Data, unsigned int _Size, int _IgnoreID = -1) = 0;

	//���ڷ� ���� ��Ŷ�� ����ȭ�Ͽ��� ���� Send ����Լ��� ���� ����
	void SendPacket(std::shared_ptr<class GameEnginePacket> _Packet, int _IgnoreID = -1);



	//������ ����Ǿ����� ����
	inline bool IsNet()
	{
		return IsNetValue;
	}

	//��Ŷ�� ��� ó���� ���� ���� ��ü
	GameEngineDispatcher Dispatcher;

	//Recv�����忡�� ���Ź��� ��Ŷ��  ���ν����忡�� ó���ϴ� �κ�(��� ���� �� ó�� ȣ������־�� ��)
	void UpdatePacket();

protected:
	//����� ����Ǿ� �ִ� ����, �ڱ� �ڽ��� Net������(������ ������ ������, Ŭ��� Ŭ���� ������)
	static void RecvThreadFunction(SOCKET _Socket, GameEngineNet* _Net);

private:
	std::atomic<bool> IsNetValue = true;


	//�Ʒ� �ڷᱸ�� RecvPacket�� ���� ���ؽ�
	std::mutex RecvPacketLock;

	//Recv�����忡�� ���Ź��� ��Ŷ�� �ٷ� ó���� �� ����(���ν������ ������ �������� �ϰ� �ִ� ���ϼ��� �ֱ� ������)
	//�׷��� Recv�����忡�� ���Ź��� ��Ŷ�� �Ʒ� �ڷᱸ���� ��Ҵٰ�,
	//���ν����忡�� ó���Ѵ�
	std::list<std::shared_ptr<GameEnginePacket>> RecvPacket;

	//���ν����忡�� ��Ŷ ó���� ���� ���� �ڷᱸ��(Lock�ɰ� ProcessPackets = RecvPacket �� ����)
	std::list<std::shared_ptr<GameEnginePacket>> ProcessPackets;


	static bool SearchPacketData(GameEngineSerializer& _Ser, unsigned int& _PacketType, unsigned int& _PacketSize);
};


#pragma once
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

	virtual void Send(const char* Data, unsigned int _Size) = 0;

	//���ڷ� ���� ��Ŷ�� ����ȭ�Ͽ��� ���� Send ����Լ��� ���� ����
	void SendPacket(std::shared_ptr<class GameEnginePacket> _Packet);



	//������ ����Ǿ����� ����
	inline bool IsNet()
	{
		return IsNetValue;
	}

	//��Ŷ�� ��� ó���� ���� ���� ��ü
	GameEngineDispatcher Dispatcher;

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
};


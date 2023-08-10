#pragma once

// ���� :
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

	virtual void Send(void* Data, unsigned int _Size) = 0;

	//������ ����Ǿ����� ����
	inline bool IsNet()
	{
		return IsNetValue;
	}

protected:
	static void RecvThreadFunction(SOCKET _Socket, GameEngineNet* _Net);

private:
	std::atomic<bool> IsNetValue = true;
};


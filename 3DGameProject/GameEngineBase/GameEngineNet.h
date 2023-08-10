#pragma once

// 설명 :
//서버와 클라가 공통으로 사용하는 클래스
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

	//서버에 연결되었는지 여부
	inline bool IsNet()
	{
		return IsNetValue;
	}

protected:
	static void RecvThreadFunction(SOCKET _Socket, GameEngineNet* _Net);

private:
	std::atomic<bool> IsNetValue = true;
};


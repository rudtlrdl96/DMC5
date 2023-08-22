#pragma once
#include <GameEngineBase/GameEngineNetObject.h>
#include <GameEngineCore/GameEngineActor.h>

class NetTestPlayer : public GameEngineActor, public GameEngineNetObject
{
public:
	NetTestPlayer();
	~NetTestPlayer();

	NetTestPlayer(const NetTestPlayer& _Other) = delete;
	NetTestPlayer(NetTestPlayer&& _Other) noexcept = delete;
	NetTestPlayer& operator=(const NetTestPlayer& _Other) = delete;
	NetTestPlayer& operator=(const NetTestPlayer&& _Other) noexcept = delete;

protected:
	void Start() override;

	//패킷을 처리하는 부분입니다. 모든 레벨의 맨 처음에 실행됩니다.
	void Update_ProcessPacket() override;

	void Update(float _DeltaTime) override;

	//업데이트 패킷을 전송하는 부분입니다. 모든 Update가 끝난 뒤에 실행됩니다
	void Update_SendPacket(float _DeltaTime) override;

private:

};


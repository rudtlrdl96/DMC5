#pragma once
#include <GameEngineBase/GameEngineNetObject.h>
#include <GameEngineCore/GameEngineActor.h>


class NetTestEnemy : public GameEngineActor, public GameEngineNetObject
{
public:
	NetTestEnemy();
	~NetTestEnemy();

	NetTestEnemy(const NetTestEnemy& _Other) = delete;
	NetTestEnemy(NetTestEnemy&& _Other) noexcept = delete;
	NetTestEnemy& operator=(const NetTestEnemy& _Other) = delete;
	NetTestEnemy& operator=(const NetTestEnemy&& _Other) noexcept = delete;

protected:
	void Start() override;

	//패킷을 처리하는 부분입니다. 모든 레벨의 맨 처음에 실행됩니다.
	void Update_ProcessPacket() override;

	void Update(float _DeltaTime) override;

	//업데이트 패킷을 전송하는 부분입니다. 모든 Update가 끝난 뒤에 실행됩니다
	void Update_SendPacket(float _DeltaTime) override;

private:

};


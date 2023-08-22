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

	//��Ŷ�� ó���ϴ� �κ��Դϴ�. ��� ������ �� ó���� ����˴ϴ�.
	void Update_ProcessPacket() override;

	void Update(float _DeltaTime) override;

	//������Ʈ ��Ŷ�� �����ϴ� �κ��Դϴ�. ��� Update�� ���� �ڿ� ����˴ϴ�
	void Update_SendPacket(float _DeltaTime) override;

private:

};


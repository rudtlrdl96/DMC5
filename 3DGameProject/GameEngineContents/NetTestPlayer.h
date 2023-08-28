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

	//��Ŷ�� ó���ϴ� �κ��Դϴ�. ��� ������ �� ó���� ����˴ϴ�.
	void Update_ProcessPacket() override;

	void Update(float _DeltaTime) override;

	//������Ʈ ��Ŷ�� �����ϴ� �κ��Դϴ�. ��� Update�� ���� �ڿ� ����˴ϴ�
	void Update_SendPacket(float _DeltaTime) override;

private:
	bool IsConnect = false;
};


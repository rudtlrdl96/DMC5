#pragma once
#include "NetLogStateBase.h"

class NetLogViewer;
class GameEngineNet;
class GameEngineNetServer;
class GameEngineNetClient;


class NetLogState_EntryReady : public NetLogStateBase
{
public:
	NetLogState_EntryReady();
	~NetLogState_EntryReady() override;

	NetLogState_EntryReady(const NetLogState_EntryReady& _Other) = delete;
	NetLogState_EntryReady(NetLogState_EntryReady&& _Other) noexcept = delete;
	NetLogState_EntryReady& operator=(const NetLogState_EntryReady& _Other) = delete;
	NetLogState_EntryReady& operator=(const NetLogState_EntryReady&& _Other) noexcept = delete;

protected:
	void Update(float _DeltaTime) override;

private:
	const std::string_view BtnText_ForHost = "Play For Host";
	const std::string_view BtnText_ForClient = "Play For Client";

	int PortNum = 30000;
	std::string IpNum = "127.0.0.1";


	//Accept�ݹ�
	static void NetAcceptCallBack(SOCKET _Socket, GameEngineNetServer* _Server);

	//������ ��Ŷ�� ��� ó���� �� ���
	void ServerPacketInit(GameEngineNetServer& _Net);

	//Ŭ��� ��Ŷ�� ��� ó���� �� ���
	void ClientPacketInit(GameEngineNetClient& _Net);
};


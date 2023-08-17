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
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::string BtnText_ForHost;
	std::string BtnText_ForClient;

	int PortNum = 30000;
	std::string IpNum = "127.0.0.1";


	//Accept콜백
	static void NetAcceptCallBack(SOCKET _Socket, GameEngineNetServer* _Server);

	//서버용 패킷을 어떻게 처리할 지 등록
	void ServerPacketInit(GameEngineNetServer& _Net);

	//클라용 패킷을 어떻게 처리할 지 등록
	void ClientPacketInit(GameEngineNetClient& _Net);
};


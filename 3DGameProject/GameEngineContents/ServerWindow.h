#pragma once
#include <GameEngineCore/GameEngineGUI.h>
#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>

// 설명 :
class ServerWindow : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	ServerWindow();
	~ServerWindow();

	// delete Function
	ServerWindow(const ServerWindow& _Other) = delete;
	ServerWindow(ServerWindow&& _Other) noexcept = delete;
	ServerWindow& operator=(const ServerWindow& _Other) = delete;
	ServerWindow& operator=(ServerWindow&& _Other) noexcept = delete;

	static GameEngineNet* NetInst;

	static bool IsClientMainThread()
	{
		return IsClient;
	}

	static bool IsServerMainThread()
	{
		return IsServer;
	}

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

	//서버를 오픈했을때 처리할 일
	void ServerInit(std::shared_ptr<GameEngineLevel> Level);

	//서버용 패킷을 어떻게 처리할 지 등록
	void ServerPacketInit(GameEngineNetServer* _Net);

	//클라용 패킷을 어떻게 처리할 지 등록
	void ClientPacketInit(GameEngineNetClient* _Net);

private:
	GameEngineNetServer Server;
	GameEngineNetClient Client;

	static bool IsServer;
	static bool IsClient;
	std::string IP = "127.0.0.1";
	int Port = 30000;

	std::string TestSendBuffer = "fashdjklfhajklfhasdjfhaklfhafhasd";

	//테스트 코드 나중에 지워야 함
	class GameEngineLevel* NowLevel = nullptr;

};


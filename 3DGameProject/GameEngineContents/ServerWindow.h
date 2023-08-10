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

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

	//서버를 오픈했을때 처리할 일
	void ServerInit(std::shared_ptr<GameEngineLevel> Level);

private:
	GameEngineNetServer Server;
	GameEngineNetClient Client;
	static GameEngineNet* NetInst;

	bool IsServer;
	std::string IP = "127.0.0.1";
	int Port = 30000;
	bool IsClient;

	std::string TestSendBuffer = "fashdjklfhajklfhasdjfhaklfhafhasd";
};


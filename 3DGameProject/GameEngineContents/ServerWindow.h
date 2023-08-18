#pragma once
#include <GameEngineCore/GameEngineGUI.h>
#include <GameEngineBase/GameEngineNetServer.h>
#include <GameEngineBase/GameEngineNetClient.h>

// ���� :
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

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

	//������ ���������� ó���� ��
	void ServerInit(std::shared_ptr<GameEngineLevel> Level);

	//������ ��Ŷ�� ��� ó���� �� ���
	void ServerPacketInit(GameEngineNetServer* _Net);

	//Ŭ��� ��Ŷ�� ��� ó���� �� ���
	void ClientPacketInit(GameEngineNetClient* _Net);

private:
	GameEngineNetServer Server;
	GameEngineNetClient Client;

	bool IsServer;
	std::string IP = "127.0.0.1";
	int Port = 30000;
	bool IsClient;

	std::string TestSendBuffer = "fashdjklfhajklfhasdjfhaklfhafhasd";

	//�׽�Ʈ �ڵ� ���߿� ������ ��
	class GameEngineLevel* NowLevel = nullptr;
};


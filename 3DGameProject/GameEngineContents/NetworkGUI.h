#pragma once
#include <GameEngineCore/GameEngineGUI.h>

class GameEngineLevel;

class NetworkGUI : public GameEngineGUIWindow
{
public:
	static NetworkGUI* GetInst()
	{
		return Inst;
	}

	NetworkGUI();
	~NetworkGUI();

	NetworkGUI(const NetworkGUI& _Other) = delete;
	NetworkGUI(NetworkGUI&& _Other) noexcept = delete;
	NetworkGUI& operator=(const NetworkGUI& _Other) = delete;
	NetworkGUI& operator=(const NetworkGUI&& _Other) noexcept = delete;

	void PrintLog(const std::string_view& _LogText);

	//GUI �� ��� �ؽ�Ʈ ����
	void SetClientTitle(int _ClientID);

	//Ÿ��Ʋ ȭ�鿡�� GUI��ư�� �������� ó���� �Լ������͸� �Է¹޽��ϴ�.
	inline void SetEntryCallBack(std::function<void()> _EntryCallBack)
	{
		EntryCallBack = _EntryCallBack;
	}

protected:
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

private:
	static NetworkGUI* Inst;

	enum class State
	{
		Wait,
		Single,
		Multi,
	};
	
	State CurState = State::Wait;

	const std::string_view BtnText_ForHost = "Play For Host";
	const std::string_view BtnText_ForClient = "Play For Client";

	int PortNum = 30000;
	//std::string IpNum = "127.0.0.1";
	char IpNum[64] = "127.0.0.1";
	std::string Title;
	
	std::function<void()> EntryCallBack = nullptr;


	void Update_Wait();
	void Update_Multi();
};


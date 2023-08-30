#pragma once
#include <GameEngineCore/GameEngineGUI.h>

class GameEngineLevel;
enum class PlayerType;

class NetworkGUI : public GameEngineGUIWindow
{
	friend class NetworkManager;

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

	//void PrintLog(const std::string_view& _LogText);

	//Ÿ��Ʋ ȭ�鿡�� GUI��ư�� �������� ó���� �Լ������͸� �Է¹޽��ϴ�.
	inline void SetEntryCallBack(std::function<void()> _EntryCallBack)
	{
		EntryCallBack = _EntryCallBack;
	}

	inline std::pair<std::string_view, int> GetNetworkData() const
	{
		return std::make_pair(IpNum, PortNum);
	}

	bool ChangeFieldState();

protected:
	void Start() override;
	void OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime) override;

private:
	static NetworkGUI* Inst;

	enum class State
	{
		SelectWait,
		ServerWait,
		ClientWait,
		OnFieldStage,

		COUNT
	};
	
	State CurState = State::SelectWait;
	std::vector<std::function<void()>> AllStateFunc;





	const std::string_view BtnText_ForHost = "Play For Host";
	const std::string_view BtnText_ForClient = "Play For Client";

	int PortNum = 30000;
	char IpNum[64] = "127.0.0.1";
	std::string NickName;
	std::string Title = "Select Host or Clinet";
	
	std::function<void()> EntryCallBack = nullptr;
	


	void Update_SelectWait();
	void Update_OnFieldStage();

	inline void InitName(unsigned int _NetID)
	{
		const std::string AddStr = "(" + std::to_string(_NetID) + ")";
		NickName = "MyName : " + NickName + AddStr;
	}
};


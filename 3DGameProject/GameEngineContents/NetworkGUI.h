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

	void PrintLog(const std::string_view& _LogText);

	//타이틀 화면에서 GUI버튼을 눌렀을때 처리할 함수포인터를 입력받습니다.
	inline void SetEntryCallBack(std::function<void()> _EntryCallBack)
	{
		EntryCallBack = _EntryCallBack;
	}

	inline std::pair<std::string_view, int> GetNetworkData() const
	{
		return std::make_pair(IpNum, PortNum);
	}

	bool ChangeFieldState();

	inline const std::string& GetNickName() const
	{
		return NickName;
	}

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
	char IpNum[16] = "127.0.0.1";
	std::function<void()> EntryCallBack = nullptr;

	std::string Title = "Select Host or Clinet";
	std::string NickName;
	std::string LocalPrintNickName;
	
	std::string Message;


	void Update_SelectWait();
	void Update_OnFieldStage();

	void InitName(unsigned int _NetID);
};


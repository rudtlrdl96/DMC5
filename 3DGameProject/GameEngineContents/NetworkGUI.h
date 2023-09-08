#pragma once
#include <GameEngineCore/GameEngineGUI.h>

class GameEngineLevel;
enum class PlayerType;

class NetworkGUI : public GameEngineGUIWindow
{
	//friend class NetworkManager;

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

	void PrintLog(const std::string_view& _LogText, const float4&  _Color = float4::WHITE);

	//Ÿ��Ʋ ȭ�鿡�� GUI��ư�� �������� ó���� �Լ������͸� �Է¹޽��ϴ�.
	inline void SetEntryCallBack(std::function<void()> _EntryCallBack)
	{
		EntryCallBack = _EntryCallBack;
	}

	inline std::pair<std::string_view, int> GetNetworkData() const
	{
		return std::make_pair(IpNum, PortNum);
	}

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
		Chat,

		COUNT
	};
	
	State CurState = State::SelectWait;
	std::vector<std::function<void()>> AllStateFunc;

	int PortNum = 30000;
	char IpNum[16] = "127.0.0.1";
	std::function<void()> EntryCallBack = nullptr;

	std::string Title = "Select Host or Clinet";
	std::string NickName;
	std::string LocalPrintNickName;
	
	std::string Message;
	std::list<std::pair<std::string, float4>> AllLog;

	void Update_SelectWait();
	void Update_Chat();

	void InitName(unsigned int _NetID);
};


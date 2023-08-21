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

	//GUI 최 상단 텍스트 설정
	void SetClientTitle(int _ClientID);

	//타이틀 화면에서 GUI버튼을 눌렀을때 처리할 함수포인터를 입력받습니다.
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
	std::string IpNum = "127.0.0.1";
	std::list<std::string> AllLog;
	std::string Title;
	
	std::function<void()> EntryCallBack = nullptr;


	void Update_Wait();
	void Update_Multi();
};


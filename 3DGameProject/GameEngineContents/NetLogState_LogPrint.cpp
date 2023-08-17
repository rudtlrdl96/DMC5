#include "PrecompileHeader.h"
#include "NetLogState_LogPrint.h"

#include "NetLogViewer.h"


std::list<std::string> NetLogState_LogPrint::AllLog(15, "");
std::mutex NetLogState_LogPrint::LogLock;

NetLogState_LogPrint::NetLogState_LogPrint()
{

}

NetLogState_LogPrint::~NetLogState_LogPrint()
{

}

#include <GameEnginePlatform/GameEngineInput.h>

void NetLogState_LogPrint::Start()
{
	NetLogStateBase::Start();

	if (false == GameEngineInput::IsKey("TEST_LOG"))
	{
		GameEngineInput::CreateKey("TEST_LOG", 'G');
	}
}


void NetLogState_LogPrint::EnterState() 
{
	NetLogStateBase::EnterState();

	TitleName = GetOwner()->IsHost() ? "Play For Host..." : "Play For Client...";
}



void NetLogState_LogPrint::Update(float _DeltaTime) 
{
	NetLogStateBase::Update(_DeltaTime);

	ImGui::Text(TitleName.c_str());
	LogLock.lock();
	for (const std::string& Log : AllLog)
	{
		ImGui::Text(Log.c_str());
	}
	LogLock.unlock();

	if (true == GameEngineInput::IsDown("TEST_LOG"))
	{
		static int Index = 0;
		PrintLog("TEST" + GameEngineString::ToString(Index++));
	}
}


void NetLogState_LogPrint::PrintLog(const std::string_view& _Text)
{
	LogLock.lock();
	AllLog.pop_front();
	AllLog.push_back(_Text.data());
	LogLock.unlock();
}

#include "PrecompileHeader.h"
#include "NetLogState_LogPrint.h"

#include "NetLogViewer.h"

NetLogState_LogPrint::NetLogState_LogPrint()
{

}

NetLogState_LogPrint::~NetLogState_LogPrint()
{

}


void NetLogState_LogPrint::EnterState() 
{
	NetLogStateBase::EnterState();

	TitleName = GetOwner()->IsHost() ? "호스트로 플레이 중입니다..." : "클라이언트로 플레이 중입니다...";
	TitleName = GameEngineString::AnsiToUTF8(TitleName.c_str());
}

void NetLogState_LogPrint::Update(float _DeltaTime) 
{
	NetLogStateBase::Update(_DeltaTime);

	ImGui::Text(TitleName.c_str());
}

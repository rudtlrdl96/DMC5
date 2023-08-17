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

	TitleName = GetOwner()->IsHost() ? "ȣ��Ʈ�� �÷��� ���Դϴ�..." : "Ŭ���̾�Ʈ�� �÷��� ���Դϴ�...";
	TitleName = GameEngineString::AnsiToUTF8(TitleName.c_str());
}

void NetLogState_LogPrint::Update(float _DeltaTime) 
{
	NetLogStateBase::Update(_DeltaTime);

	ImGui::Text(TitleName.c_str());
}

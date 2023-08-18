#include "PrecompileHeader.h"
#include "NetworkGUI.h"

#include "NetworkManager.h"

NetworkGUI* NetworkGUI::Inst = nullptr;

NetworkGUI::NetworkGUI()
{
	Inst = this;
}

NetworkGUI::~NetworkGUI()
{
	
}

void NetworkGUI::PrintLog(const std::string_view& _LogText)
{
	if (State::Multi != CurState)
		return;

	if (true == AllLog.empty())
	{
		AllLog.resize(15, "");
	}

	AllLog.pop_front();
	AllLog.push_back(_LogText.data());
}

void NetworkGUI::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	switch (CurState)
	{
	case NetworkGUI::State::Wait:
		Update_Wait();
		break;
	case NetworkGUI::State::Single:
		
		break;
	case NetworkGUI::State::Multi:
		Update_Multi();
		break;
	}
}

void NetworkGUI::Update_Wait()
{
	//호스트 버튼을 누른 경우
	ImGui::InputInt("Port ", &PortNum);
	if (ImGui::Button(BtnText_ForHost.data()))
	{
		NetworkManager::ServerOpen(PortNum);
		CurState = State::Multi;
		PrintLog("Server Open Success");
		Title = "This is Host";
		return;
	}

	//클라이언트 버튼을 누른 경우
	ImGui::InputText("IP", &IpNum[0], IpNum.size());
	if (ImGui::Button(BtnText_ForClient.data()))
	{
		bool IsResult = false;
		IsResult = NetworkManager::ConnectServer(IpNum, PortNum);
		if (true == IsResult)
		{
			CurState = State::Multi;
			PrintLog("Server Connect Success");
			return;
		}
	}

}

void NetworkGUI::SetClientTitle(int _ClientID)
{
	Title = "Client Number : " + GameEngineString::ToString(_ClientID);
}

void NetworkGUI::Update_Multi() 
{
	ImGui::Text(Title.c_str());

	for (const std::string& Log : AllLog)
	{
		ImGui::Text(Log.c_str());
	}
}



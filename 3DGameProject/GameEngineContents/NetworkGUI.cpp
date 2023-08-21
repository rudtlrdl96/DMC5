#include "PrecompileHeader.h"
#include "NetworkGUI.h"

#include "NetworkManager.h"
#include "BaseLog.h"

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

	BaseLog::PushLog(LogOrder::Network, _LogText.data());
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
	//ȣ��Ʈ ��ư�� ���� ���
	ImGui::InputInt("Port ", &PortNum);
	if (ImGui::Button(BtnText_ForHost.data()))
	{
		NetworkManager::ServerOpen(PortNum);
		CurState = State::Multi;
		PrintLog("Server Open Success");
		Title = "This is Host";
		return;
	}

	//Ŭ���̾�Ʈ ��ư�� ���� ���
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

	const std::vector<std::string>& LogDatas = BaseLog::GetLog(0);

	for (const std::string& Log : LogDatas)
	{
		ImGui::Text(Log.c_str());
	}
}



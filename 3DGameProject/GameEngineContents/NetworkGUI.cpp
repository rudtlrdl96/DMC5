#include "PrecompileHeader.h"
#include "NetworkGUI.h"

#include "NetworkManager.h"
#include "BaseLog.h"
#include "ContentsEnum.h"

NetworkGUI* NetworkGUI::Inst = nullptr;

NetworkGUI::NetworkGUI()
{
	Inst = this;
}

NetworkGUI::~NetworkGUI()
{
	
}

void NetworkGUI::Start()
{
	AllStateFunc.resize(static_cast<size_t>(State::COUNT), nullptr);
	AllStateFunc[static_cast<size_t>(State::SelectWait)] = std::bind(&NetworkGUI::Update_SelectWait, this);
	AllStateFunc[static_cast<size_t>(State::ServerWait)] = [=]() 
	{
		ImGui::Text(Title.c_str()); 
		ImGui::Text("Server Open Success");
	};
	AllStateFunc[static_cast<size_t>(State::ClientWait)] = [=]() 
	{
		ImGui::Text(Title.c_str()); 
		ImGui::Text("Waiting Server Connect..");
	};
	AllStateFunc[static_cast<size_t>(State::OnFieldStage)] = std::bind(&NetworkGUI::Update_OnFieldStage, this);
}



void NetworkGUI::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	size_t StateIndex = static_cast<size_t>(CurState);
	if (AllStateFunc.size() <= StateIndex)
	{
		MsgAssert("NetworkGUI의 State가 잘못된 값으로 설정되었습니다");
		return;
	}

	if (nullptr == AllStateFunc[StateIndex])
		return;

	AllStateFunc[StateIndex]();
}


void NetworkGUI::Update_SelectWait()
{
	ImGui::Text(Title.c_str());

	ImGui::InputText("IP", IpNum, 64);
	ImGui::InputInt("Port ", &PortNum);

	//호스트 버튼을 누른 경우
	if (ImGui::Button(BtnText_ForHost.data()))
	{
		NetworkManager::ServerOpen(PortNum);
		Title = "This is Host";
		if (nullptr != EntryCallBack)
		{
			EntryCallBack();
			EntryCallBack = nullptr;
		}

		CurState = State::ServerWait;
		return;
	}

	//클라이언트 버튼을 누른 경우
	if (ImGui::Button(BtnText_ForClient.data()))
	{
		Title = "This is Client";
		if (nullptr != EntryCallBack)
		{
			EntryCallBack();
			EntryCallBack = nullptr;
		}

		//서버 연결은 캐릭터를 선택하고 난 뒤에
		
		CurState = State::ClientWait;
		return;
	}
}



bool NetworkGUI::ChangeFieldState()
{
	if (State::ServerWait != CurState && State::ClientWait != CurState)
		return false;

	CurState = State::OnFieldStage;
	return true;
}


void NetworkGUI::Update_OnFieldStage() 
{
	ImGui::Text(Title.c_str());

	//여기서 어쩌면 채팅기능?
}

//
//void NetworkGUI::PrintLog(const std::string_view& _LogText)
//{
//	BaseLog::PushLog(LogOrder::Network, _LogText.data());
//}
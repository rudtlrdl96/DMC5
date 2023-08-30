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
		ImGui::TextColored(ImVec4(1, 1, 0, 1), Title.c_str());
		ImGui::Text("Server Open Success");
	};
	AllStateFunc[static_cast<size_t>(State::ClientWait)] = [=]() 
	{
		ImGui::TextColored(ImVec4(1, 1, 0, 1), Title.c_str());
		ImGui::Text("Waiting Server Connect..");
	};
	AllStateFunc[static_cast<size_t>(State::OnFieldStage)] = std::bind(&NetworkGUI::Update_OnFieldStage, this);

	NickName.resize(64, 0);
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
	ImGui::InputText("NickName", &NickName[0], NickName.size());

	//호스트 버튼을 누른 경우
	if (ImGui::Button(BtnText_ForHost.data()))
	{
		CurState = State::ServerWait;

		//GUI 제목 초기화
		Title = "This is Host";

		//닉네임 초기화
		if (0 == NickName.front())
		{
			NickName = "Unknown";
		}
		unsigned int NetID = NetworkManager::ServerOpen(PortNum);
		InitName(NetID);

		//UI쪽에서 넣은 콜백 호출
		if (nullptr != EntryCallBack)
		{
			EntryCallBack();
			EntryCallBack = nullptr;
		}
		return;
	}

	//클라이언트 버튼을 누른 경우
	if (ImGui::Button(BtnText_ForClient.data()))
	{
		CurState = State::ClientWait;

		//GUI 제목 초기화
		Title = "This is Client";

		//닉네임 초기화
		if (0 == NickName.front())
		{
			NickName = "Unknown";
		}

		//서버 연결은 캐릭터를 선택하고 난 뒤에

		//UI쪽에서 넣은 콜백 호출
		if (nullptr != EntryCallBack)
		{
			EntryCallBack();
			EntryCallBack = nullptr;
		}
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
	ImGui::TextColored(ImVec4(1, 1, 0, 1), Title.c_str());
	ImGui::Text(NickName.c_str());

	//여기서 어쩌면 채팅기능?
}

//
//void NetworkGUI::PrintLog(const std::string_view& _LogText)
//{
//	BaseLog::PushLog(LogOrder::Network, _LogText.data());
//}
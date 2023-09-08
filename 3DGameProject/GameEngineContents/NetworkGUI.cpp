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
	AllStateFunc[static_cast<size_t>(State::Chat)] = std::bind(&NetworkGUI::Update_Chat, this);

	NickName.resize(16, 0);
	Message.resize(32, 0);

	/*for (size_t i = 0; i < 20; ++i)
	{
		BaseLog::PushLog(LogOrder::Network, " ");
	}*/

	AllLog.resize(20, { " ", float4::WHITE });
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

	ImGui::InputText("IP", IpNum, 16);
	ImGui::InputInt("Port ", &PortNum);
	ImGui::InputText("NickName", &NickName[0], NickName.size());

	//호스트 버튼을 누른 경우
	if (ImGui::Button("Play For Host"))
	{
		//서버 오픈 및 닉네임 초기화
		unsigned int NetID = NetworkManager::ServerOpen(PortNum);
		InitName(NetID);


		//GUI 제목 및 상태 변경
		Title = "Server Open Success!";
		CurState = State::Chat;


		//UI쪽에서 넣은 콜백 호출
		if (nullptr != EntryCallBack)
		{
			EntryCallBack();
			EntryCallBack = nullptr;
		}
		return;
	}

	//클라이언트 버튼을 누른 경우
	if (ImGui::Button("Play For Client"))
	{
		//서버와 연결 및 닉네임 변경 콜백 등록
		NetworkManager::ConnectServer(IpNum, PortNum, std::bind(&NetworkGUI::InitName, this, std::placeholders::_1));

		//GUI 제목 및 상태 변경
		Title = "Server Connect Success";
		CurState = State::Chat;

		//UI쪽에서 넣은 콜백 호출
		if (nullptr != EntryCallBack)
		{
			EntryCallBack();
			EntryCallBack = nullptr;
		}
		return;
	}
}


void NetworkGUI::InitName(unsigned int _NetID)
{
	if (0 == NickName.front())
	{
		NickName = "Unknown";
	}
	else
	{
		size_t EndIndex = NickName.find('\0');
		NickName = NickName.substr(0, EndIndex);
	}

	const std::string AddStr = "(" + std::to_string(_NetID) + ")";
	NickName = NickName + AddStr;
	LocalPrintNickName = "MyName : " + NickName;
}


void NetworkGUI::Update_Chat()
{
	ImGui::TextColored(ImVec4(1, 1, 0, 1), Title.c_str());
	ImGui::Text(LocalPrintNickName.c_str());

	/*const std::vector<std::string>& AllLog = BaseLog::GetLog(LogOrder::Network);
	for (const std::string& Log : AllLog)
	{
		ImGui::Text(Log.c_str());
	}*/

	for (const std::pair<std::string, float4>& Log : AllLog)
	{
		const std::string& LogValue = Log.first;
		const float4& LogColor = Log.second;

		if (float4::WHITE == LogColor)
		{
			ImGui::Text(LogValue.c_str());
		}
		else
		{
			ImGui::TextColored(ImVec4(LogColor.r, LogColor.g, LogColor.b, 1), LogValue.c_str());
		}
	}


	ImGui::InputText("Message", &Message[0], Message.size());
	if (ImGui::Button("SendMessage"))
	{
		if (0 == Message.front())
			return;

		std::string Msg = NickName + " : " + Message;
		PrintLog(Msg);
		NetworkManager::PushChatPacket(Msg);
		for (char& C : Message)
			C = 0;
	}
}


void NetworkGUI::PrintLog(const std::string_view& _LogText, const float4& _Color /*= float4::WHITE*/)
{
	BaseLog::PushLog(LogOrder::Network, _LogText.data());

	AllLog.pop_front();
	AllLog.push_back(std::make_pair(_LogText.data(), _Color));
}
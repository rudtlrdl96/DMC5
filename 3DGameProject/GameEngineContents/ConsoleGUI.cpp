#include "PrecompileHeader.h"
#include "ConsoleGUI.h"

#include <GameEngineCore/imgui.h>
#include <GameEngineCore/GameEngineCore.h>

#include "BaseLog.h"
#include "ThreadLoadingLevel.h"

float ConsoleGUI::FrameTime = 0.0f;
int ConsoleGUI::FrameCount = 0;
int ConsoleGUI::DrawFrame = 0;

ConsoleGUI::ConsoleGUI()
{
}

ConsoleGUI::~ConsoleGUI() 
{
}

void ConsoleGUI::Start()
{
	LogTypeNames[(int)LogOrder::Network] = "Network";
	LogTypeNames[(int)LogOrder::Enemy] = "Enemy";
}

void ConsoleGUI::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	FrameTime += _DeltaTime;
	++FrameCount;

	if (1.0f <= FrameTime)
	{
		DrawFrame = FrameCount;

		FrameTime -= 1.0f;
		FrameCount = 0;
	}


	ImGui::Text((std::string("Frame : ") + std::to_string(DrawFrame)).data());

	ImGui::Spacing();
	ImGui::InputText("Move Level Name : ", MoveLevelName, 255);

	if(true == ImGui::Button("Move Button"))
	{
		if (true == GameEngineCore::IsLevel(MoveLevelName))
		{		
			GameEngineCore::ChangeLevel(MoveLevelName);
		}
	}

	ImGui::Combo("Log Type", &PrintLogOrder, LogTypeNames, (int)LogOrder::Count);

	if (ImGui::Button("Log Reset"))
	{
		BaseLog::LogReset(PrintLogOrder);
	}

	const std::vector<std::string>& LogDatas = BaseLog::GetLog(PrintLogOrder); 

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Log");
	ImGui::BeginChild("Log Scrolling");
		
	for (int n = 0; n < LogDatas.size(); n++)
	{
		ImGui::Text("%04d: %s", n, LogDatas[n].data());
	}

	ImGui::SeparatorText("Move To");
	
	for (auto& i : GameEngineCore::GetLevelMap())
	{
		if (ImGui::Button(i.first.c_str()))
		{
			ThreadLoadingLevel::ChangeLevel(i.first);
		}
			
	}

	ImGui::EndChild();

}
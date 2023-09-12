#include "PrecompileHeader.h"
#include "StageGUI.h"

StageGUI::StageGUI()
{

}

StageGUI::~StageGUI()
{

}

void StageGUI::OnGUI(std::shared_ptr<GameEngineLevel> _Level, float _DeltaTime)
{
	FPSView(_DeltaTime);

	if (ImGui::Button("DebugSwitch"))
	{
		_Level->IsDebugSwitch();
	}
}

void StageGUI::FPSView(float _DeltaTime)
{
	static float Rate = 0;
	FPSTime += _DeltaTime;
	if (FPSTime >= 1.0)
	{
		FPSTime = 0.f;
		Rate = GameEngineCore::GetFrameRate();
	}
	ImGui::Text(std::to_string(Rate).c_str());
}

#include "PrecompileHeader.h"
#include "StageEditGUI.h"
#include <GameEngineCore/GameEngineLevel.h>

StageEditGUI::StageEditGUI()
{
}

StageEditGUI::~StageEditGUI()
{
}

void StageEditGUI::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	if (nullptr == Level)
		return;
    if (ImGui::Button("StageEdit"))
    {
        TestBool = !TestBool;
    }
    if (true == TestBool)
    {
        const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
        static int item_current = 1;
        ImGui::ListBox("listbox", &item_current, items, IM_ARRAYSIZE(items), 4);
        ImGui::SameLine();
    }
}



#include "PrecompileHeader.h"
#include "EditorUI.h"
#include <GameEngineCore/GameEngineSpriteRenderer.h>


EditorUI::EditorUI()
{

}

EditorUI::~EditorUI()
{

}

void EditorUI::Start()
{
	Floor = CreateComponent<GameEngineSpriteRenderer>();
	Floor->SetTexture("Floor.png");
	Floor->GetTransform()->SetWorldPosition(float4::ZERO);
	Floor->GetTransform()->SetWorldScale(float4(10000, 10000, 10000));
	Floor->GetTransform()->SetWorldRotation(float4(90, 0, 0));
}

void EditorUI::Update(float _DeltaTime)
{
}

#include "PrecompileHeader.h"
#include "NeroHPUI.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>

NeroHPUI::NeroHPUI() 
{
}

NeroHPUI::~NeroHPUI() 
{
}

void NeroHPUI::Start()
{
	std::shared_ptr<GameEngineFBXRenderer> NeroUI_HP = CreateComponent<GameEngineFBXRenderer>();
	NeroUI_HP->SetFBXMesh("NeroHPUI.FBX", "NoneAlphaMesh");
}

void NeroHPUI::Update(float _DeltaTime)
{
}


#include "PrecompileHeader.h"
#include "Nero_InvenToryUI.h"

#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
//std::vector<std::shared_ptr<Nero_InvenToryUI>> Nero_InvenToryUI::Invens;
Nero_InvenToryUI::Nero_InvenToryUI() 
{
}

Nero_InvenToryUI::~Nero_InvenToryUI() 
{
}

void Nero_InvenToryUI::Start()
{

	InvenBaseRender = CreateComponent<GameEngineUIRenderer>(1);
	InvenBaseRender->GetTransform()->SetLocalScale(Scale);
	InvenBaseRender->SetTexture("Nero_Inven4.png");
}

void Nero_InvenToryUI::Update(float _Delta)
{
}

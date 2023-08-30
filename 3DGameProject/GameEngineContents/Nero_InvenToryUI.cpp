#include "PrecompileHeader.h"
#include "Nero_InvenToryUI.h"

#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
std::vector<std::shared_ptr<Nero_InvenToryUI>> Nero_InvenToryUI::Invens;
Nero_InvenToryUI::Nero_InvenToryUI() 
{
}

Nero_InvenToryUI::~Nero_InvenToryUI() 
{
}

void Nero_InvenToryUI::CreateNeroInven(GameEngineLevel* _Level)
{
	std::shared_ptr<Nero_InvenToryUI> Inven = _Level->CreateActor<Nero_InvenToryUI>();
	Invens.push_back(Inven);
};

void Nero_InvenToryUI::Start()
{

	InvenBaseRender = CreateComponent<GameEngineUIRenderer>(1);
	InvenBaseRender->GetTransform()->SetLocalScale({330.0f,110.f,0.0f});
	InvenBaseRender->SetTexture("Nero_Inven.png");
}

void Nero_InvenToryUI::Update(float _Delta)
{
}

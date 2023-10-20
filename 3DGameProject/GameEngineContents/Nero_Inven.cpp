#include "PrecompileHeader.h"
#include "Nero_Inven.h"
#include "Nero_InvenToryUI.h"

#include <GameEngineCore/GameEngineUIRenderer.h>

Nero_Inven::Nero_Inven() 
{
}

Nero_Inven::~Nero_Inven() 
{

}

void Nero_Inven::Start()
{
	Inventory.push_back(Inven0 = GetLevel()->CreateActor<Nero_InvenToryUI>());
	Inventory.push_back(Inven1 = GetLevel()->CreateActor<Nero_InvenToryUI>());
	Inventory.push_back(Inven2 = GetLevel()->CreateActor<Nero_InvenToryUI>());
	Inven0->GetTransform()->SetParent(GetTransform());
	Inven1->GetTransform()->SetParent(GetTransform());
	Inven2->GetTransform()->SetParent(GetTransform());

}

void Nero_Inven::Update(float _Delta)
{

}


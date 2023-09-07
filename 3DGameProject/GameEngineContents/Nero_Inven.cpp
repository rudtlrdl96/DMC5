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
	Inventory.push_back(GetLevel()->CreateActor<Nero_InvenToryUI>());
	Inventory.push_back(GetLevel()->CreateActor<Nero_InvenToryUI>());
	Inventory.push_back(GetLevel()->CreateActor<Nero_InvenToryUI>());
}

void Nero_Inven::Update(float _Delta)
{

}


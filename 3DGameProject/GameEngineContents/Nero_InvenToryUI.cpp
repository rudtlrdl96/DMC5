#include "PrecompileHeader.h"
#include "Nero_InvenToryUI.h"
#include "NeroItemGlass.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
bool Nero_InvenToryUI::IsAddInvenArm = false;
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
	ItemRender_0 = CreateComponent<GameEngineUIRenderer>(1);
	ItemRender_0->GetTransform()->SetLocalPosition({ -159.0f,0.0f,0.0f });
	ItemRender_0->SetTexture("NullTexture.png");
	DevilBreakerRender.push_back(ItemRender_0);
	ItemRender_1 = CreateComponent<GameEngineUIRenderer>(1);
	ItemRender_1->GetTransform()->SetLocalPosition({ -54.0f,0.0f,0.0f });
	ItemRender_1->SetTexture("NullTexture.png");
	DevilBreakerRender.push_back(ItemRender_1);
	ItemRender_2 = CreateComponent<GameEngineUIRenderer>(1);
	ItemRender_2->GetTransform()->SetLocalPosition({ 52.0f,0.0f,0.0f });
	ItemRender_2->SetTexture("NullTexture.png");
	DevilBreakerRender.push_back(ItemRender_2);
	ItemRender_3 = CreateComponent<GameEngineUIRenderer>(1);	
	ItemRender_3->GetTransform()->SetLocalPosition({ 159.0f,0.0f,0.0f });
	ItemRender_3->SetTexture("NullTexture.png");
	DevilBreakerRender.push_back(ItemRender_3);
}

void Nero_InvenToryUI::Update(float _Delta)
{
	AddInvenArm();
}


void Nero_InvenToryUI::AddInvenArm()
{
	InvenDevil = NeroItemGlass::GetVectorArmUIPtr();
	if (IsAddInvenArm == true)
	{

		for (size_t t = 0; t < InvenDevil.size(); t++)
		{
			CurDevilBreaker = InvenDevil[t];
			switch (CurDevilBreaker)
			{
			case DevilBreaker::None:
				break;
			case DevilBreaker::Overture:
				DevilBreakerRender[t]->SetScaleToTexture("Nero_Overture.png");
				break;
			case DevilBreaker::Gerbera:
				DevilBreakerRender[t]->SetScaleToTexture("Nero_Gerbera.png");
				break;
			case DevilBreaker::BusterArm:
				DevilBreakerRender[t]->SetScaleToTexture("Nero_BusterArm.png");
				break;
			default:
				break;
			}
		}
		IsAddInvenArm = false;
	}
	
	
}

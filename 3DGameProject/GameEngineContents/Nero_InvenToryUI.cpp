#include "PrecompileHeader.h"
#include "Nero_InvenToryUI.h"
#include "Shop_NeroArmUI.h"
#include "PlayerActor_Nero.h"
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
	InvenBaseRender->GetTransform()->SetLocalScale(Scale*GameEngineActor::ScreenRatio);
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
	if (IsAddInvenArm == true)
	{
		InvenDevil.clear();
		std::copy(PlayerActor_Nero::GetBreakerListPointer()->begin(), PlayerActor_Nero::GetBreakerListPointer()->end(), std::back_inserter(InvenDevil));
		for (size_t t = InvenDevil.size(); t >1 ; t--)
		{
			CurDevilBreaker = InvenDevil[t-1];
			DevilBreakerRender[InvenDevil.size()-t]->On();
			switch (CurDevilBreaker)
			{
			case DevilBreaker::None:
				break;
			case DevilBreaker::Overture:
				DevilBreakerRender[InvenDevil.size() - t]->SetScaleToTexture("Nero_Overture.png");
				break;
			case DevilBreaker::Gerbera:
				DevilBreakerRender[InvenDevil.size() - t]->SetScaleToTexture("Nero_Gerbera.png");
				break;
			case DevilBreaker::BusterArm:
				DevilBreakerRender[InvenDevil.size() - t]->SetScaleToTexture("Nero_BusterArm.png");
				break;
			default:
				break;
			}
		}
		for (size_t i = InvenDevil.size()-1; i < 4; i++)
		{
			DevilBreakerRender[i]->Off();
		}
		IsAddInvenArm = false;
	}
	
	
}

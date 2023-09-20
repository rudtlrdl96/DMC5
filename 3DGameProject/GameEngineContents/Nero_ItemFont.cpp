#include "PrecompileHeader.h"
#include "Nero_ItemFont.h"
#include "BasePlayerActor.h"
#include "PlayerActor_Nero.h"


#include <GameEngineCore/GameEngineFontRenderer.h>
Nero_ItemFont::Nero_ItemFont()
{
}

Nero_ItemFont::~Nero_ItemFont()
{
}

void Nero_ItemFont::Start()
{
	ItemText = CreateComponent<GameEngineFontRenderer>(5);
	ItemText->SetFont("DMC5Font");
	ItemText->SetFontFlag(FW1_CENTER);
	ItemText->SetScale(36);
	ItemText->SetColor(float4(0.701f, 0.772f, 0.788f, 1.0f));
	ItemText->GetTransform()->SetLocalPosition({ 540.0f,-313.f,0.0f });


	CurCount = CreateComponent<GameEngineFontRenderer>(5);
	CurCount->SetFont("DMC5Font");
	CurCount->SetFontFlag(FW1_CENTER);
	CurCount->SetScale(50);
	CurCount->SetColor(float4(0.701f, 0.772f, 0.788f, 1.0f));
	CurCount->GetTransform()->SetLocalPosition({ 634.0f,-302.f,0.0f, });


	SlashText = CreateComponent<GameEngineFontRenderer>(8);
	SlashText->SetFont("DMC5Font");
	SlashText->SetFontFlag(FW1_CENTER);
	SlashText->SetScale(36);
	SlashText->SetColor(float4(0.656f, 0.668f, 0.665f, 1.0f));
	SlashText->GetTransform()->SetLocalPosition({ 660.0f,-315.f,0.0f });
	SlashText->SetText("/");

	MaxCount = CreateComponent<GameEngineFontRenderer>(8);
	MaxCount->SetFont("DMC5Font");
	MaxCount->SetFontFlag(FW1_CENTER);
	MaxCount->SetScale(36);
	MaxCount->SetColor(float4(0.656f, 0.668f, 0.665f, 1.0f));
	MaxCount->GetTransform()->SetLocalPosition({ 680.0f,-315.f,0.0f });
	MaxCount->SetText(std::to_string(MaxItem));
}

void Nero_ItemFont::Update(float _DeltaTime)
{
	CurCount->SetText(std::to_string(PlayerActor_Nero::GetBreakerStackPointer()->size()));
	switch (CurDevilBreaker)
	{
	case DevilBreaker::None:
		ItemText->SetText("Donthave");
		break;
	case DevilBreaker::Overture:
		ItemText->SetText("Overture");
		break;
	case DevilBreaker::Gerbera:
		ItemText->SetText("Gerbera");
		break;
	case DevilBreaker::BusterArm:
		ItemText->SetText("BusterArm");
		break;
	default:
		break;
	}
}

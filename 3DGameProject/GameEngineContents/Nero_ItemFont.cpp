#include "PrecompileHeader.h"
#include "Nero_ItemFont.h"

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
	ItemText->SetScale(40);
	ItemText->SetColor(float4(0.701f, 0.772f, 0.788f, 1.0f));
	ItemText->GetTransform()->SetLocalPosition({ 515.0f,90.f,0.0f });
	ItemText->SetText(std::to_string(MaxItem));

	CurCount = CreateComponent<GameEngineFontRenderer>(5);
	CurCount->SetFont("DMC5Font");
	CurCount->SetFontFlag(FW1_CENTER);
	CurCount->SetScale(16);
	CurCount->SetColor(float4(0.701f, 0.772f, 0.788f, 1.0f));
	CurCount->GetTransform()->SetLocalPosition({ 515.0f,36.f,0.0f, });
	CurCount->SetText(std::to_string(MaxItem));


	MaxCount = CreateComponent<GameEngineFontRenderer>(8);
	MaxCount->SetFont("DMC5Font");
	MaxCount->SetFontFlag(FW1_CENTER);
	MaxCount->SetScale(16);
	MaxCount->SetColor(float4(0.656f, 0.668f, 0.665f, 1.0f));
	MaxCount->GetTransform()->SetLocalPosition({ 705.0f,36.f,0.0f });
	MaxCount->SetText(std::to_string(MaxItem));
}

void Nero_ItemFont::Update(float _DeltaTime)
{
}


#include "PrecompileHeader.h"
#include "NeroLoding.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
NeroLoding::NeroLoding() 
{
}

NeroLoding::~NeroLoding() 
{
}

void NeroLoding::Start()
{
	LodingBackRender = CreateComponent<GameEngineUIRenderer>(0);
	LodingBackRender->SetTexture("LodingTexture.png");
	LodingBackRender->GetTransform()->SetLocalScale({ 1920.0f,1080.0f,0.0f });
	LodingBackRender->GetTransform()->SetLocalPosition({ 155.0f,0.0f,0.0f });
}

void NeroLoding::Update(float _Delta)
{
}


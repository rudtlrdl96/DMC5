#include "PrecompileHeader.h"
#include "MyContentSpriteRenderer.h"

MyContentSpriteRenderer::MyContentSpriteRenderer()
{
	SetName("MyContentSpriteRenderer");
}

MyContentSpriteRenderer::~MyContentSpriteRenderer()
{
}

void MyContentSpriteRenderer::Start()
{
	GameEngineRenderer::Start();

	std::shared_ptr<GameEngineRenderUnit> Unit = CreateRenderUnit();

	Unit->SetMesh("Rect");
	Unit->SetMaterial("My2DTexture");

	AtlasData.x = 0.0f;
	AtlasData.y = 0.0f;
	AtlasData.z = 1.0f;
	AtlasData.w = 1.0f;

	ColorOptionValue.MulColor = float4::ONE;
	ColorOptionValue.PlusColor = float4::ZERONULL;

	AColor.r = 0.0f;
	AColor.a = 0.0f;

	GetShaderResHelper().SetConstantBufferLink("AtlasData", AtlasData);
	GetShaderResHelper().SetConstantBufferLink("ColorOption", ColorOptionValue);
	GetShaderResHelper().SetConstantBufferLink("TestColor", AColor);
}
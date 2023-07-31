#include "PrecompileHeader.h"
#include "MyContentSpriteRenderer.h"

MyContentSpriteRenderer::MyContentSpriteRenderer() 
{
}

MyContentSpriteRenderer::~MyContentSpriteRenderer() 
{
}

void MyContentSpriteRenderer::Start() 
{
	GameEngineRenderer::Start();

	SetMesh("Rect");
	SetPipeLine("My2DTexture");

	AtlasData.x = 0.0f;
	AtlasData.y = 0.0f;
	AtlasData.z = 1.0f;
	AtlasData.w = 1.0f;

	ColorOptionValue.MulColor = float4::One;
	ColorOptionValue.PlusColor = float4::Null;

	AColor.r = 0.0f;
	AColor.a = 0.0f;

	GetShaderResHelper().SetConstantBufferLink("AtlasData", AtlasData);
	GetShaderResHelper().SetConstantBufferLink("ColorOption", ColorOptionValue);
	GetShaderResHelper().SetConstantBufferLink("TestColor", AColor);
}
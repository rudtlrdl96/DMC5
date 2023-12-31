#include "PrecompileHeader.h"
#include "GameEngineFontRenderer.h"
#include "GameEngineFont.h"
#include "GameEngineSpriteRenderer.h"
GameEngineFontRenderer::GameEngineFontRenderer()
{
	SetName("GameEngineFontRenderer");
}

GameEngineFontRenderer::~GameEngineFontRenderer()
{
}

void GameEngineFontRenderer::SetFont(const std::string_view& _Font)
{
	Font = GameEngineFont::Find(_Font);
}

void GameEngineFontRenderer::SetText(const std::string_view& _Text)
{
	Text = _Text;
}

void GameEngineFontRenderer::Start()
{
	PushCameraRender(100);
	Unit = CreateRenderUnit("Rect", "2DTexture");
	AtlasData.x = 0.0f;
	AtlasData.y = 0.0f;
	AtlasData.z = 1.0f;
	AtlasData.w = 1.0f;
	float4 HSVMin = { 0.0f, 0.5f, 0.5f, 0.5f };
	ColorOptionValue.MulColor = float4::ONE;
	ColorOptionValue.PlusColor = float4::ZERONULL;
	ColorOptionValue.BSCColor = HSVMin;

	GetShaderResHelper().SetConstantBufferLink("AtlasData", AtlasData);
	GetShaderResHelper().SetConstantBufferLink("ColorOption", ColorOptionValue);
	GetShaderResHelper().SetConstantBufferLink("ClipData", Clip);
	GetShaderResHelper().SetConstantBufferLink("FlipData", Flip);

	Unit->RenderFunction = std::bind(&GameEngineFontRenderer::Render, this, std::placeholders::_1);
}

void GameEngineFontRenderer::Render(float _Delta)
{
	if (nullptr == Font)
	{
		return;
	}

	if ("" == Text)
	{
		return;
	}

	float4 Pos = GetTransform()->GetWorldPosition();

	GameEngineCamera* Camera = GetCamera();
	Pos *= Camera->GetView();
	Pos *= Camera->GetProjection();
	Pos *= Camera->GetViewPort();

	Font->FontDraw(Text, Pos, FontScale, FontColor, FwTextFlag);

	GameEngineDevice::GetContext()->GSSetShader(nullptr, nullptr, 0);
}

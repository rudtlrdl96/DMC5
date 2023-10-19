#include "PrecompileHeader.h"
#include "UIMeshRenderer.h"
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>

UIMeshRenderer::UIMeshRenderer()
{
	SetName("UIMeshRenderer");
}

UIMeshRenderer::~UIMeshRenderer()
{
}

void UIMeshRenderer::LightDataUpdate()
{
	std::shared_ptr<GameEngineCamera> UICamera = GetLevel()->GetCamera(100);

	if (nullptr == UICamera)
	{
		MsgAssert("UI 카메라가 존재하지 않습니다.");
		return;
	}

	LightDataValue.ViewLightPos = LightDataValue.LightPos * UICamera->GetView();
	LightDataValue.ViewLightDir = LightDataValue.LightDir * UICamera->GetView();
	LightDataValue.ViewLightRevDir = -LightDataValue.ViewLightDir;
	LightDataValue.CameraPosition = UICamera->GetTransform()->GetWorldPosition() * UICamera->GetView();
	LightDataValue.CameraWorldPosition = UICamera->GetTransform()->GetWorldPosition();

	LightDataValue.LightType = static_cast<int>(LightType::Directional);

	LightDataValue.CameraView = UICamera->GetView();
	LightDataValue.CameraViewInverseMatrix = UICamera->GetView().InverseReturn();
}

void UIMeshRenderer::Start()
{
	PushCameraRender(100);
}

void UIMeshRenderer::LightDataLink()
{
	for (size_t i = 0; i < Unit.size(); i++)
	{
		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			Unit[i][j]->ShaderResHelper.SetConstantBufferLink("UILightDatas", LightDataValue);
			Unit[i][j]->ShaderResHelper.SetConstantBufferLink("ColorData", HSVColor);
			Unit[i][j]->ShaderResHelper.SetConstantBufferLink("ClipData", Clip);
		}
	}
}

void UIMeshRenderer::DrawEditor()
{
	ImGui::Spacing();

	{
		float4 BC = BaseValue.AddColor;
		float InputColor[4] = { BC.r, BC.g, BC.b, BC.a };
		ImGui::DragFloat4("AddColor", InputColor, 0.01f);
		BaseValue.AddColor = { InputColor[0], InputColor[1], InputColor[2], InputColor[3] };
	}

	{
		float4 BC = BaseValue.MulColor;
		float InputColor[4] = { BC.r, BC.g, BC.b, BC.a };
		ImGui::DragFloat4("MulColor", InputColor, 0.01f);
		BaseValue.MulColor = { InputColor[0], InputColor[1], InputColor[2], InputColor[3] };
	}

	{
		float4 BC = HSVColor;
		float InputColor[4] = { BC.r, BC.g, BC.b, BC.a };
		ImGui::DragFloat4("HSVColor", InputColor, 0.01f);
		HSVColor = { InputColor[0], InputColor[1], InputColor[2], InputColor[3] };
	}

	ImGui::Spacing();

	{
		float InputColor[4] = { Clip.ClipStartX, Clip.ClipEndX, Clip.ClipStartY, Clip.ClipEndY };
		ImGui::DragFloat4("Clip", InputColor, 0.01f);

		Clip.ClipStartX = InputColor[0];
		Clip.ClipEndX = InputColor[1];
		Clip.ClipStartY = InputColor[2];
		Clip.ClipEndY = InputColor[3];
	}
}
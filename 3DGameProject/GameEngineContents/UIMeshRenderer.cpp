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
		}
	}
}

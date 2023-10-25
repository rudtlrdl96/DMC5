#include "PrecompileHeader.h"
#include "StageEditLevel.h"

#include <GameEngineCore/GameEngineGUI.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/EngineGrid.h>
#include <GameEngineCore/BloomEffect.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "NetworkManager.h"
#include "StageEditGUI.h"
#include "FreeCameraActor.h"

#include "DistortionEffect.h"


StageEditLevel::StageEditLevel()
{
}

StageEditLevel::~StageEditLevel()
{
}

void StageEditLevel::Start()
{
	StageBaseLevel::Start();
	if (GetMainCamera() != nullptr)
	{
		GetMainCamera()->SetProjectionType(CameraType::Perspective);
	}
	GetMainCamera()->GetTransform()->SetLocalPosition({0,0,-500});

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Texture");
	NewDir.Move("StageEditorUI");

	std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });

	for (size_t i = 0; i < File.size(); i++)
	{
		GameEngineTexture::Load(File[i].GetFullPath());
	}

	Grid = CreateActor<EngineGrid>();
	FreeCam = CreateActor<FreeCameraActor>();
	IsEditLevel = true;

	GetCamera(0)->GetDeferredLightTarget()->CreateEffect<BloomEffect>();

	{
		std::shared_ptr<DistortionEffect> Distortion = GetCamera(0)->GetCamTarget()->CreateEffect<DistortionEffect>();
		Distortion->SetMaskTexture(GetCamera(0)->GetCamAlphaTarget(), 1);
		Distortion->SetDistortionValue(10, 10);
		Distortion->SetMaxPixelValue(100, 100);
	}

	GetDirectionalLight()->GetTransform()->SetWorldPosition(float4(0.f, 100000.f, 0.f));
	GetDirectionalLight()->GetTransform()->SetWorldRotation({ 45.f,45.f,45.f });
	GetDirectionalLight()->SetLightPower(1.f);
	GetDirectionalLight()->SetLightColor({ 1.f,1.f,1.f });
}

void StageEditLevel::Update(float _DeltaTime)
{
}

void StageEditLevel::LevelChangeStart()
{
	StageBaseLevel::LevelChangeStart();

	if (Editor == nullptr)
	{
		std::shared_ptr<GameEngineGUIWindow> EditGUI = GameEngineGUI::GUIWindowCreate<StageEditGUI>("StageEditGUI");
		Editor = EditGUI;
	}

	Editor->On();

	IsDebugSwitch();

	GameEngineCoreWindow::Clear();
	GameEngineCoreWindow::AddDebugRenderTarget(0, "Forward Target", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "Deferred Target", GetMainCamera()->GetCamDeferrdTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "Light Target", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "Alpha Target", GetMainCamera()->GetCamAlphaTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(4, "Last Target", GetMainCamera()->GetCamTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(5, "Bake Shadow", GetDirectionalLight()->GetBakeTarget(0));
	GameEngineCoreWindow::AddDebugRenderTarget(6, "Last Shadow", GetDirectionalLight()->GetShadowTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(7, "Distortion Target", GetCamera(100)->GetCamAlphaTarget());
}

void StageEditLevel::LevelChangeEnd()
{
	Editor->Off();
}


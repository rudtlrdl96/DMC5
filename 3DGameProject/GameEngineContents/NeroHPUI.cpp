#include "PrecompileHeader.h"
#include "NeroHPUI.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include "UIFBXRenderer.h"
#include "UI_HPGaege.h"
#include "UI_DTGauge.h"
#include "UI_BulletGauge.h"
NeroHPUI::NeroHPUI() 
{
}

NeroHPUI::~NeroHPUI() 
{
}
/*
�ĵ�,�ĵ��
HP�� 1��
���Ű�����
�Ѿ�
*/
void NeroHPUI::Start()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Mesh");
	NewDir.Move("UIMesh");
	NewDir.Move("NeroUIMesh");
	NewDir.Move("NeroHPMesh");

	if (nullptr == GameEngineFBXMesh::Find("NeroIden3.FBX"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}
	NeroUI_HPGlass= UIFBXActorBase::CreateUIFBX(NeroUI_HPGlass, { -535.0f,273.0f,172.0f }, { 0.6f,0.6f,0.6f }, { -90.0f,0.0f,0.0f }, "Nero_HPGlass.FBX");
	NeroUI_Hud1 = UIFBXActorBase::CreateUIFBX(NeroUI_Hud1, { -711.0f,303.0f,20.0f }, { 0.7f,0.7f,0.7f }, { 30.0f,-90.0f,-30.0f }, "NeroEx01.FBX");
	NeroUI_Hud2 = UIFBXActorBase::CreateUIFBX(NeroUI_Hud2, { -708.0f,334.0f,0.0f }, { 0.7f,0.7f,0.7f }, { 30.0f,-90.0f,-30.0f }, "NeroEx02.FBX");
	NeroUI_Hud3 = UIFBXActorBase::CreateUIFBX(NeroUI_Hud3, { -703.0f,365.0f,0.0f }, { 0.7f,0.7f,0.7f }, { 30.0f,-90.0f,-30.0f }, "NeroEx03.FBX");

	std::shared_ptr<GameEngineUIRenderer> NeroUI_Back = CreateComponent<GameEngineUIRenderer>(0);
	NeroUI_Back->SetTexture("HudBack.png");
	NeroUI_Back->GetTransform()->SetLocalScale({ 97.0f,131.0f,0.0f });
	NeroUI_Back->GetTransform()->SetLocalPosition({ -696.0f,334.0f,40.0f });

	std::shared_ptr<UI_HPGaege> UI_HPGaegeBar = GetLevel()->CreateActor<UI_HPGaege>();
	std::shared_ptr<UI_DTGauge> UI_DTGaugeBar = GetLevel()->CreateActor<UI_DTGauge>();
	std::shared_ptr<UI_BulletGauge> UI_BulletGaugeBar = GetLevel()->CreateActor<UI_BulletGauge>();

	
}

void NeroHPUI::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsPress("UI_Tab"))
	{

		NeroUI_HPGlass->GetTransform()->AddLocalPosition({ 10.f,0.f,0.0f });
	}
}


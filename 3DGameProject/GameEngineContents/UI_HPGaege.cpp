#include "PrecompileHeader.h"
#include "UI_HPGaege.h"
#include "UIFBXRenderer.h"
UI_HPGaege::UI_HPGaege() 
{
}

UI_HPGaege::~UI_HPGaege() 
{
}

void UI_HPGaege::Start()
{
	for (int i = 0; i < 10; i++)
	{
		CreateHPGaugeBar(GetLevel(), { -570.0f + i * 50.0f , 405.0f , 0.0f });
	}
}

void UI_HPGaege::Update(float _DeltaTime)
{

}

std::shared_ptr<UIFBXRenderer> UI_HPGaege::CreateHPGaugeBar(GameEngineLevel* _Level, float4 _Pos)
{
	std::shared_ptr<UIFBXRenderer> Gaege = nullptr;
	Gaege = CreateComponent<UIFBXRenderer>(3);
	Gaege->SetFBXMesh("HpGauge.FBX", "FBX_Low");
	Gaege->GetTransform()->SetLocalRotation({ 90.0f,0.0f,0.0f });
	Gaege->GetTransform()->SetLocalScale({ 0.7f,0.7f,0.7f });
	Gaege->GetTransform()->SetLocalPosition(_Pos);
	HpGauges.push_back(Gaege);
	return Gaege;
}

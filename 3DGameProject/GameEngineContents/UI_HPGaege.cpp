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
		GaugeBaseActor::CreateGaugeBar({ -570.0f + i * 49.0f , 405.0f , 0.0f },{ 0.7f,0.6f,0.6f }, { 90.0f,0.0f,0.0f }, "HpGauge.FBX");
	}
}

void UI_HPGaege::Update(float _DeltaTime)
{

}


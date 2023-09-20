#include "PrecompileHeader.h"
#include "UI_DTGauge.h"

UI_DTGauge::UI_DTGauge() 
{
}

UI_DTGauge::~UI_DTGauge() 
{
}

void UI_DTGauge::Start()
{

	for (int i = 0; i < 3; i++)
	{
		UIFBXActorBase::CreateGaugeBar({ -590.0f + i * 16.0f , 372.0f , 0.0f }, { 0.9f, 0.9f, 0.9f }, {-123.0f,0.0f,0.0f}, "DtGauge.FBX");
	}
	for (int i = 0; i < 7; i++)
	{
		UIFBXActorBase::CreateGaugeBar({ -541.0f + i * 13.0f , 375.0f , 0.0f }, { 0.7f, 0.7f, 0.7f }, { -123.0f,0.0f,0.0f }, "DtGauge.FBX");
	}
}

void UI_DTGauge::Update(float _DeltaTime)
{
}


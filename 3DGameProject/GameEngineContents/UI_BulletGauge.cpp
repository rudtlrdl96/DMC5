#include "PrecompileHeader.h"
#include "UI_BulletGauge.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
UI_BulletGauge::UI_BulletGauge()
{
}

UI_BulletGauge::~UI_BulletGauge()
{
}

void UI_BulletGauge::Start()
{

	BulletGaugeBase = CreateComponent<GameEngineUIRenderer>(5);
	BulletGaugeBase->SetScaleToTexture("BulletGaugeBase.png");
	BulletGaugeBase->GetTransform()->SetLocalPosition({ -561.0f,336.0f,0.0f });
	BulletGauge = CreateComponent<GameEngineUIRenderer>(5);
	BulletGauge->SetScaleToTexture("BulletGauge.png");
	BulletGauge->GetTransform()->SetLocalPosition({ -561.0f,336.0f,0.0f });
}

void UI_BulletGauge::Update(float _DeltaTime)
{
	ClipGauge(_DeltaTime);
}

void UI_BulletGauge::ClipGauge(float _DeltaTime)
{
	
	AddTime += _DeltaTime; 
	BulletGauge->ImageClippingX(ExBulletCount, ClipXDir::Left);
	//ÃÑ¾ËÀ» ½úÀ»¶§
	if (true == GameEngineInput::IsUp("UIDEBUGMODE"))
	{
		ExBulletCount-=0.33f;
	}
	//ÃÑ¾ËÀ» Ã¤¿ï¶§
	if (true == GameEngineInput::IsPress("UI_Tab"))
	{

		if (ExBulletCount < 1.0f)
		{
			ExBulletCount += _DeltaTime*0.3f;
		}

	}
}

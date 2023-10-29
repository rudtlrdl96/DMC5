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
	float Ratio = GameEngineWindow::GetScreenSize().y / 900;
	GetTransform()->SetWorldScale(float4::ONE * Ratio);

	BulletGaugeBase = CreateComponent<GameEngineUIRenderer>(5);
	BulletGaugeBase->SetScaleToTexture("BulletGaugeBase.png");
	BulletGaugeBase->GetTransform()->SetLocalPosition({ -561.0f,336.0f,-1.0f });
	BulletGauge = CreateComponent<GameEngineUIRenderer>(5);
	BulletGauge->SetScaleToTexture("BulletGauge.png");
	BulletGauge->GetTransform()->SetLocalPosition({ -561.0f,336.0f,-1.0f });
}

void UI_BulletGauge::Update(float _DeltaTime)
{
	ClipGauge(_DeltaTime);
}

void UI_BulletGauge::ClipGauge(float _DeltaTime)
{
	if (ExBulletCount == 0)
	{
		BulletGauge->ImageClippingX(0.0f, ClipXDir::Left);
	}
	else if (ExBulletCount == 1)
	{
		BulletGauge->ImageClippingX(0.33f, ClipXDir::Left);
	}
	else if (ExBulletCount == 2)
	{
		BulletGauge->ImageClippingX(0.66f, ClipXDir::Left);
	}
	else if (ExBulletCount == 3)
	{
		BulletGauge->ImageClippingX(1.0f, ClipXDir::Left);
	}
	//�Ѿ��� ä�ﶧ
	if (true == GameEngineInput::IsPress("UI_bullet") || true == GameEngineInput::IsPress("Player_Gun"))
	{
		AddTime += _DeltaTime;
		if (AddTime > 1.0f)
		{
			if (ExBulletCount < 3)
			{
				ExBulletCount++;
				AddTime = 0.0f;
				if (ExBulletCount == 3)
				{
					GameEngineSound::Play("BR_2.wav");
				}
				else
				{
					GameEngineSound::Play("BR_3.wav");
				}
			}
			AddTime = 0.0f;
		}
	}
	else
	{
		AddTime = 0.0f;
	}
}

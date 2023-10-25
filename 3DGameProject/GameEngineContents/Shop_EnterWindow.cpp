#include "PrecompileHeader.h"
#include "Shop_EnterWindow.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
Shop_EnterWindow::Shop_EnterWindow() 
{
}

Shop_EnterWindow::~Shop_EnterWindow() 
{
}

void Shop_EnterWindow::Start()
{
	ShopWindowRender = CreateComponent<GameEngineUIRenderer>();
	ShopWindowRender->SetTexture("Shop_EnterWindow.png");
	ShopWindowRender->GetTransform()->SetLocalScale({ 281.0f,75.f,0.0f });
	ShopWindowRender->GetTransform()->SetLocalPosition({ 270.0f,-32.0f,0.0f });
	ShopWindowRender->ColorOptionValue.MulColor.a = 0.0f;
	ShopWindowRender->ImageClippingX(0.0f, ClipXDir::Left);
	ShopWindowRender->Off();
}

void Shop_EnterWindow::Update(float _Delta)
{
	if (true == GameEngineInput::IsUp("UI_ESC"))
	{
		IsApper = true;
	}
	if (true == GameEngineInput::IsUp("UI_Tab"))
	{
		IsApper = false;
	}
	ApperWindow(_Delta);
}

void Shop_EnterWindow::ApperWindow(float _Delta)
{
	if (IsApper == true)
	{
		ShopWindowRender->On();
		Time += _Delta;
		if (Time+0.3f <= 1.0f)
		{
			ShopWindowRender->ColorOptionValue.MulColor.a = Time+0.3f;
		}
		if (Time <= 1.0f)
		{
			ShopWindowRender->ImageClippingX(Time*1.5f, ClipXDir::Left);
		}

	}
	else
	{
		Time = 0.0f;
	}
}

void Shop_EnterWindow::DisApperWindow(float _Delta)
{
}


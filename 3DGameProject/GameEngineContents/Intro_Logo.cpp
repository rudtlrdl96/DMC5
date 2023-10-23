#include "PrecompileHeader.h"
#include "Intro_Logo.h"

#include <GameEngineCore/GameEngineSpriteRenderer.h>
Intro_Logo::Intro_Logo()
{
}

Intro_Logo::~Intro_Logo()
{
}

void Intro_Logo::Start()
{
	CapComIntro_Logo = CreateComponent<GameEngineSpriteRenderer>();
	CapComIntro_Logo->GetTransform()->SetLocalPosition(Pos);
	CapComIntro_Logo->SetTexture("CapComLogo.png");
	CapComIntro_Logo->GetTransform()->SetLocalScale(Scale * GameEngineActor::ScreenRatio);
	CapComIntro_Logo->ColorOptionValue.MulColor.a = 0;
	PhysxIntro_Logo = CreateComponent<GameEngineSpriteRenderer>();
	PhysxIntro_Logo->GetTransform()->SetLocalPosition(Pos);
	PhysxIntro_Logo->SetTexture("PhysxLogo.png");
	PhysxIntro_Logo->GetTransform()->SetLocalScale(Scale * GameEngineActor::ScreenRatio);
	PhysxIntro_Logo->ColorOptionValue.MulColor.a = 0;
}

void Intro_Logo::Update(float _DeltaTime)
{
	Time += _DeltaTime;
	if (2.0f < Time && Time < 4.0f)
	{
		if (CapComIntro_Logo->ColorOptionValue.MulColor.a <= 1.0f)
		{
			CapComIntro_Logo->ColorOptionValue.MulColor.a += Ratio * _DeltaTime;
		}

	}
	if (4.0f < Time && Time < 6.0f)
	{
		if (CapComIntro_Logo->ColorOptionValue.MulColor.a >= 0)
		{
			CapComIntro_Logo->ColorOptionValue.MulColor.a -= Ratio * _DeltaTime;
		}
	}
	if (6.0f < Time && Time < 8.0f)
	{
		if (PhysxIntro_Logo->ColorOptionValue.MulColor.a <= 1.0f)
		{
			PhysxIntro_Logo->ColorOptionValue.MulColor.a += Ratio * _DeltaTime;
		}

	}
	if (8.0f < Time && Time < 10.0f)
	{
		if (PhysxIntro_Logo->ColorOptionValue.MulColor.a >= 0)
		{
			PhysxIntro_Logo->ColorOptionValue.MulColor.a -= Ratio * _DeltaTime;
		}
	}

}


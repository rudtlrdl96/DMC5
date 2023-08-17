#include "PrecompileHeader.h"
#include "Logo.h"

#include <GameEngineCore/GameEngineSpriteRenderer.h>
Logo::Logo() 
{
}

Logo::~Logo() 
{
}

void Logo::Start()
{
	CapComLogo = CreateComponent<GameEngineSpriteRenderer>();
	CapComLogo->GetTransform()->SetLocalPosition(Pos);
	CapComLogo->SetScaleToTexture("CapComLogo.png");
	CapComLogo->ColorOptionValue.MulColor.a = 0;
	PhysxLogo = CreateComponent<GameEngineSpriteRenderer>();
	PhysxLogo->GetTransform()->SetLocalPosition(Pos);
	PhysxLogo->SetScaleToTexture("PhysxLogo.png");
	PhysxLogo->ColorOptionValue.MulColor.a = 0;
	PhysxLogo->Off();
}

void Logo::Update(float _DeltaTime)
{
	Time += _DeltaTime;
	if (2.0f < Time && Time < 4.0f)
	{
		if (CapComLogo->ColorOptionValue.MulColor.a <= 1.0f)
		{
			CapComLogo->ColorOptionValue.MulColor.a += Ratio * _DeltaTime;
		}
		
	}
	if (4.0f < Time && Time < 6.0f)
	{
		if (CapComLogo->ColorOptionValue.MulColor.a >= 0)
		{
			CapComLogo->ColorOptionValue.MulColor.a -= Ratio * _DeltaTime;
		}
	}
	if (6.0f < Time && Time < 8.0f)
	{
		if (PhysxLogo->ColorOptionValue.MulColor.a <= 1.0f)
		{
			PhysxLogo->ColorOptionValue.MulColor.a += Ratio * _DeltaTime;
		}

	}
	if (8.0f < Time && Time < 10.0f)
	{
		if (PhysxLogo->ColorOptionValue.MulColor.a >= 0)
		{
			PhysxLogo->ColorOptionValue.MulColor.a -= Ratio * _DeltaTime;
		}
	}

}


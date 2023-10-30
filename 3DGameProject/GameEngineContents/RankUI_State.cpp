#include "PrecompileHeader.h"
#include "RankUI.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include "FXSystem.h"
#include "UIEffectRenderer.h"
#include "UIFBXRenderer.h"
#include "UIMeshRenderer.h"
void RankUI::StateInit_Wait()
{
	RankFSM.CreateState(
		{
			.StateValue = RankState::Rank_WaitState,
			.Start = [=]
			{
				RankBackEffect_Up->GetTransform()->SetLocalPosition(InsideOut);
				RankBackEffect->GetTransform()->SetLocalPosition(InsideOut);
				DisApperValue = false;
				ResetRank();
			},
			.Update = [=](float _DeltaTime)
			{

				RankBackEffect->Loop = false;
				RankBackEffect_Up->Loop = false;
				if (RankScore > 0.0f)
				{
					RankFSM.ChangeState(RankState::Appear_RankD);
				}
			},
			.End = [=]()
			{

			},
		});
}
void RankUI::StateInit_RankD()
{
	RankFSM.CreateState(
		{
			.StateValue = RankState::Appear_RankD,
			.Start = [=]
			{
			RankBackEffect_Up->GetTransform()->SetLocalPosition(InsidePos);
			RankBackEffect->GetTransform()->SetLocalPosition(InsidePos);
			RankBackEffect->PlayFX("RankBackEffect.effect");
			RankBackEffect->Loop = true;
			ExplaneSpeed = 0.0F;
			Ratio = 0.0f;
			OutRank = false;
			RankD_Inside->Clip = { 0.0f, 0.0f, 0.0f, 0.0f };

			},
			.Update = [=](float _DeltaTime)
			{
			RankApper(_DeltaTime,RankD_Frame,RankState::Spin_RankD,false,RankD_Frame);

			},
			.End = [=]()
			{

			},
		});

	RankFSM.CreateState(
		{
			.StateValue = RankState::Spin_RankD,
			.Start = [=]()
			{
				Ratio = 0.0f;
				ExplaneSpeed = 0.0f;
				ScaleSpeed = 0.0f;
				ScaleUpValue = false;
				ScaleValue = false;
				TurnValue = false;
				UpTime = 0.0f;
				DisApperValue = false;

			},
			.Update = [=](float _DeltaTime)
			{
				RankSpin(_DeltaTime,RankD_Frame,RankD_Inside);
				RankClip(_DeltaTime, RankD_Frame, RankD_Inside,0);

				if (UpTime > 1.0f)
				{
					SetRankExPlane("RankD_Explane.png", float4{228.0f,82.0f,0.0f}, InsideStart, _DeltaTime);
				}
				RankDisApper(_DeltaTime, RankD_Frame, RankD_Inside);
				if (RankScore >= 100.0f)
				{
					RankFSM.ChangeState(RankState::Appear_RankC);
				}
			},
			.End = [=]
			{

			},
		});
}

void RankUI::StateInit_RankC()
{
	RankFSM.CreateState({
		.StateValue = RankState::Appear_RankC,
		.Start = [this]()
		{
			OutRank = false;
			Ratio = 0.0f;
			RankC_Inside->Clip = {0.0f,0.0f,0.0f,0.0f};
		},
		.Update = [this](float _DeltaTime)
		{
			RankApper(_DeltaTime,RankC_Frame,RankState::Spin_RankC,true,RankD_Frame);
		},
		.End = [this]()
		{

		},
		});
	RankFSM.CreateState(
		{
			.StateValue = RankState::Spin_RankC,
			.Start = [=]()
			{
			MemberInitialize();
			},
			.Update = [=](float _DeltaTime)
			{
				RankSpin(_DeltaTime,RankC_Frame,RankC_Inside);
				RankClip(_DeltaTime, RankC_Frame, RankC_Inside, 100);
				if (UpTime > 1.0f)
				{
					SetRankExPlane("RankC_Explane.png", float4{ 203.0f,99.0f,0.0f }, InsideStart, _DeltaTime);
				}
				RankDisApper(_DeltaTime, RankC_Frame, RankC_Inside);
			if (RankScore >= 200)
			{
				RankFSM.ChangeState(RankState::Appear_RankB);
			}
		},
		.End = [=]
		{

		},
		});
}

void RankUI::StateInit_RankB()
{
	RankFSM.CreateState({
		.StateValue = RankState::Appear_RankB,
		.Start = [this]()
		{
			OutRank = false;
			Ratio = 0.0f;
			RankC_Inside->Clip = { 0.0f,0.0f,0.0f,0.0f };

		},
		.Update = [this](float _DeltaTime)
		{
			RankApper(_DeltaTime,RankB_Frame,RankState::Spin_RankB,true,RankC_Frame);
		},
		.End = [this]()
		{

		},
		});
	RankFSM.CreateState(
		{
			.StateValue = RankState::Spin_RankB,
			.Start = [=]()
			{
				MemberInitialize();
			},
			.Update = [=](float _DeltaTime)
			{
				RankSpin(_DeltaTime,RankB_Frame,RankB_Inside);
				RankClip(_DeltaTime, RankB_Frame, RankB_Inside, 200);

				if (UpTime > 1.0f)
				{
					SetRankExPlane("RankB_Explane.png", float4{ 264.0f,90.0f,0.0f }, InsideStart, _DeltaTime);
				}
				RankDisApper(_DeltaTime, RankB_Frame, RankB_Inside);

				if (RankScore >= 300)
				{
					RankFSM.ChangeState(RankState::Appear_RankA);
				}
			},
			.End = [=]
			{

			},
		});
}

void RankUI::StateInit_RankA()
{
	RankFSM.CreateState({
	.StateValue = RankState::Appear_RankA,
	.Start = [this]()
	{
		OutRank = false;
		Ratio = 0.0f;
		RankC_Inside->Clip = { 0.0f,0.0f,0.0f,0.0f };
		RankAFire->EffectOption.MulColor = float4{ 0.3f,0.3f,1.0f,1.f };
		RankAFire->EffectOption.PlusColor = float4{ 0.274f,0.583f,1.f,0.0f };
		EffectValue = false;
	},
	.Update = [this](float _DeltaTime)
	{
		
			
		RankApper(_DeltaTime,RankA_Frame,RankState::Spin_RankA,true,RankB_Frame);
		if (EffectValue == true)
		{
			RankAFire->On();
			RankAFire->ChangeAnimation("FireAni");
			RankALightEffect->PlayFX("RankALightEffect.effect");
			EffectValue = false;
		}

	},
	.End = [this]()
	{

	},
		});
	RankFSM.CreateState(
		{
			.StateValue = RankState::Spin_RankA,
			.Start = [=]()
			{		
			MemberInitialize();
			},
			.Update = [=](float _DeltaTime)
			{
				RankSpin(_DeltaTime,RankA_Frame,RankA_Inside);
				RankClip(_DeltaTime, RankA_Frame, RankA_Inside, 300);

				if (UpTime > 1.0f)
				{
					SetRankExPlane("RankA_Explane.png", float4{434.0f,107.0f,0.0f }, InsideStart, _DeltaTime);
				}
				RankDisApper(_DeltaTime, RankA_Frame, RankA_Inside);

				if (RankScore >= 400)
				{
					RankFSM.ChangeState(RankState::Appear_RankS);
				}
			},
			.End = [=]
			{

			},
		});
}

void RankUI::StateInit_RankS()
{
	RankFSM.CreateState({
	.StateValue = RankState::Appear_RankS,
	.Start = [this]()
	{
		OutRank = false;
		Ratio = 0.0f;
		RankC_Inside->Clip = { 0.0f,0.0f,0.0f,0.0f };
		RankAFire->EffectOption.MulColor = float4{ 0.913f,0.894f,0.250f,1.f };
		RankAFire->EffectOption.PlusColor = float4{ 0.588f,0.78f,1.f,0.006f };

	},
	.Update = [this](float _DeltaTime)
	{
		RankApper(_DeltaTime,RankS_Frame,RankState::Spin_RankS,true,RankA_Frame);
		if (EffectValue == true)
		{
			RankAFire->On();
			RankAFire->ChangeAnimation("FireAni");
			RankSLightEffect->PlayFX("RankSLightEffect.effect");
			EffectValue = false;
		}
	},
	.End = [this]()
	{

	},
		});
	RankFSM.CreateState(
		{
			.StateValue = RankState::Spin_RankS,
			.Start = [=]()
			{
				MemberInitialize();
			},
			.Update = [=](float _DeltaTime)
			{
				RankSpin(_DeltaTime,RankS_Frame,RankS_Inside);
				RankClip(_DeltaTime, RankS_Frame, RankS_Inside, 400);
				if (UpTime > 1.0f)
				{
					SetRankExPlane("RankS_Explane.png", float4{ 285.0f,147.0f,0.0f }, InsideStart, _DeltaTime);
				}
				RankDisApper(_DeltaTime, RankS_Frame, RankS_Inside);

				if (RankScore >= 500)
				{
					RankFSM.ChangeState(RankState::Appear_RankSS);
				}
			},
			.End = [=]
			{

			},
		});
}

void RankUI::StateInit_RankSS()
{
	RankFSM.CreateState({
	.StateValue = RankState::Appear_RankSS,
	.Start = [this]()
	{
		OutRank = false;
		Ratio = 0.0f;
		RankC_Inside->Clip = { 0.0f,0.0f,0.0f,0.0f };
		RankAFire->EffectOption.MulColor = float4{ 0.913f,0.894f,0.250f,1.f };
		RankAFire->EffectOption.PlusColor = float4{ 0.588f,0.78f,1.f,0.006f };

	},
	.Update = [this](float _DeltaTime)
	{
		RankApper(_DeltaTime,RankSS_Frame,RankState::Spin_RankSS,true,RankS_Frame);
		if (EffectValue == true)
		{
			RankAFire->On();
			RankAFire->ChangeAnimation("FireAni");
			RankSLightEffect->PlayFX("RankSLightEffect.effect");
			EffectValue = false;
		}
	},
	.End = [this]()
	{

	},
		});
	RankFSM.CreateState(
		{
			.StateValue = RankState::Spin_RankSS,
			.Start = [=]()
			{
				MemberInitialize();
			},
			.Update = [=](float _DeltaTime)
			{
				RankSpin(_DeltaTime,RankSS_Frame,RankSS_Inside);
				RankClip(_DeltaTime, RankSS_Frame, RankSS_Inside, 500);
				if (UpTime > 1.0f)
				{
					SetRankExPlane("RankSS_Explane.png", float4{ 269.0f,183.0f,0.0f }, InsideStart, _DeltaTime);
				}
				RankDisApper(_DeltaTime, RankSS_Frame, RankSS_Inside);

				if (RankScore >= 600)
				{
					RankFSM.ChangeState(RankState::Appear_RankSSS);
				}
			},
			.End = [=]
			{

			},
		});
}

void RankUI::StateInit_RankSSS()
{
	RankFSM.CreateState({
	.StateValue = RankState::Appear_RankSSS,
	.Start = [this]()
	{
		OutRank = false;
		RankC_Inside->Clip = { 0.0f,0.0f,0.0f,0.0f };
		RankAFire->EffectOption.MulColor = float4{ 0.913f,0.894f,0.250f,1.f };
		RankAFire->EffectOption.PlusColor = float4{ 0.588f,0.78f,1.f,0.006f };
		Ratio = 0.0f;
	},
	.Update = [this](float _DeltaTime)
	{
		RankApper(_DeltaTime,RankSSS_Frame,RankState::Spin_RankSSS,true,RankSS_Frame);
		if (EffectValue == true)
		{
			RankAFire->On();
			RankAFire->ChangeAnimation("FireAni");
			RankSLightEffect->PlayFX("RankSLightEffect.effect");
			EffectValue = false;
		}
	},
	.End = [this]()
	{

	},
		});
	RankFSM.CreateState(
		{
			.StateValue = RankState::Spin_RankSSS,
			.Start = [=]()
			{
				MemberInitialize();
			},
			.Update = [=](float _DeltaTime)
			{
				RankSpin(_DeltaTime,RankSSS_Frame,RankSSS_Inside);
				RankClip(_DeltaTime, RankSSS_Frame, RankSSS_Inside, 600);
				if (UpTime > 1.0f)
				{
					SetRankExPlane("RankSSS_Explane.png", float4{ 431.0f,250.0f,0.0f }, InsideStart, _DeltaTime);
				}
				RankDisApper(_DeltaTime, RankSSS_Frame, RankSSS_Inside);
			},
			.End = [=]
			{

			},
		});
}

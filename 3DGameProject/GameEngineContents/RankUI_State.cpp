#include "PrecompileHeader.h"
#include "RankUI.h"
#include "UIFBXRenderer.h"

void RankUI::StateInit_RankD()
{
	RankFSM.CreateState(
		{
			.StateValue = RankState::Appear_RankD,
			.Start = [=]
			{
				RankD_Frame->GetTransform()->SetLocalPosition({ { 660.0f,90.0f,0.0f,-150.0f } });
				RankD_Frame->GetTransform()->SetLocalRotation({ -90.0f,0.0f,0.0f });
				RankD_Frame->GetTransform()->SetLocalScale({ 8.0f,8.0f,8.0f });
				RankD_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
				Ratio = 0.0f;
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
				RankD_Inside->SetClipData(float4::ZERONULL);
				Ratio = 0.0f;
			},
			.Update = [=](float _DeltaTime)
			{
				UpTime += _DeltaTime;
				//랭크메쉬 돌리기
				RankSpin(_DeltaTime,RankD_Frame,RankD_Inside);
				//이부분에 게이지 클립과 이펙트 효과 추가.
				float EndUp = static_cast<float>(TestRankGauge) / 100.0f;
				RankD_Inside->SetClipData(float4::LerpClamp(float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, EndUp, 0.0f, 1.0f), UpTime));

				//설명란 띄우기
				if (UpTime > 2.0f)
				{
					SetRankExPlane("RankD_Explane.png", float4{228.0f,82.0f,0.0f}, float4{660.0f,-33.0f,0.0f }, _DeltaTime);
				}
				//만약 게이지가 100을 넘는다면
				if (TestRankGauge >= 100)
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
				Ratio = 0.0f;
				TurnValue = false;
				RankC_Inside->SetClipData(float4::ZERONULL);
				UpTime = 0.0f;
			},
			.Update = [=](float _DeltaTime)
			{
			//이전 메쉬 내보내기
			//랭크 돌리기.
				RankSpin(_DeltaTime,RankC_Frame,RankC_Inside);
				UpTime += _DeltaTime;
				float EndUp = static_cast<float>(TestRankGauge-100) / 100.0f;
				RankC_Inside->SetClipData(float4::LerpClamp(float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, EndUp, 0.0f, 1.0f), UpTime));

			if (TestRankGauge >= 200)
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
				Ratio = 0.0f;
				TurnValue = false;
				RankB_Inside->SetClipData(float4::ZERONULL);
				UpTime = 0.0f;
			},
			.Update = [=](float _DeltaTime)
			{
			//이전 메쉬 내보내기
			//랭크 돌리기.
				RankSpin(_DeltaTime,RankB_Frame,RankB_Inside);
				UpTime += _DeltaTime;
				float EndUp = static_cast<float>(TestRankGauge - 200) / 100.0f;
				RankB_Inside->SetClipData(float4::LerpClamp(float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, EndUp, 0.0f, 1.0f), UpTime));
				if (TestRankGauge >= 300)
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
	},
	.Update = [this](float _DeltaTime)
	{
		RankApper(_DeltaTime,RankA_Frame,RankState::Spin_RankA,true,RankB_Frame);
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
				Ratio = 0.0f;
				TurnValue = false;
				RankA_Inside->SetClipData(float4::ZERONULL);
				UpTime = 0.0f;
			},
			.Update = [=](float _DeltaTime)
			{
			//이전 메쉬 내보내기
			//랭크 돌리기.
				RankSpin(_DeltaTime,RankA_Frame,RankA_Inside);
				UpTime += _DeltaTime;
				float EndUp = static_cast<float>(TestRankGauge - 300) / 100.0f;
				RankA_Inside->SetClipData(float4::LerpClamp(float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, EndUp, 0.0f, 1.0f), UpTime));
				if (TestRankGauge >= 400)
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
	},
	.Update = [this](float _DeltaTime)
	{
		RankApper(_DeltaTime,RankS_Frame,RankState::Spin_RankS,true,RankA_Frame);
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
				Ratio = 0.0f;
				TurnValue = false;
				RankS_Inside->SetClipData(float4::ZERONULL);
				UpTime = 0.0f;
			},
			.Update = [=](float _DeltaTime)
			{
				RankSpin(_DeltaTime,RankS_Frame,RankS_Inside);
				UpTime += _DeltaTime;
				float EndUp = static_cast<float>(TestRankGauge - 400) / 100.0f;
				RankS_Inside->SetClipData(float4::LerpClamp(float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, EndUp, 0.0f, 1.0f), UpTime));
				if (TestRankGauge >= 500)
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
	},
	.Update = [this](float _DeltaTime)
	{
		RankApper(_DeltaTime,RankSS_Frame,RankState::Spin_RankSS,true,RankS_Frame);
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
				Ratio = 0.0f;
				TurnValue = false;
				RankSS_Inside->SetClipData(float4::ZERONULL);
				UpTime = 0.0f;
			},
			.Update = [=](float _DeltaTime)
			{
			//이전 메쉬 내보내기
			//랭크 돌리기.
				RankSpin(_DeltaTime,RankSS_Frame,RankSS_Inside);
				UpTime += _DeltaTime;
				float EndUp = static_cast<float>(TestRankGauge - 500) / 100.0f;
				RankSS_Inside->SetClipData(float4::LerpClamp(float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, EndUp, 0.0f, 1.0f), UpTime));
				if (TestRankGauge >= 600)
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
		Ratio = 0.0f;
	},
	.Update = [this](float _DeltaTime)
	{
		RankApper(_DeltaTime,RankSSS_Frame,RankState::Spin_RankSSS,true,RankSS_Frame);
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
				Ratio = 0.0f;
				TurnValue = false;
				RankSSS_Inside->SetClipData(float4::ZERONULL);
				UpTime = 0.0f;
			},
			.Update = [=](float _DeltaTime)
			{
			//이전 메쉬 내보내기
			//랭크 돌리기.
				RankSpin(_DeltaTime,RankSSS_Frame,RankSSS_Inside);
				UpTime += _DeltaTime;
				float EndUp = static_cast<float>(TestRankGauge - 600) / 100.0f;
				if (EndUp <= 1.0f)
				{
					RankSSS_Inside->SetClipData(float4::LerpClamp(float4(0.0f, 0.0f, 0.0f, 1.0f), float4(0.0f, EndUp, 0.0f, 1.0f), UpTime));
				}
				//만약 게이지가 100을 넘는다면
				//if (FullGauge == true)
				//{
				//	RankFSM.ChangeState(RankState::Appear_RankC);
				//}
			},
			.End = [=]
			{

			},
		});
}

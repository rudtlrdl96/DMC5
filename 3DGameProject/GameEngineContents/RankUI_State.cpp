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
				Ratio = 0.0f;
			},
			.Update = [=](float _DeltaTime)
			{

				//랭크메쉬 돌리기
				RankSpin(_DeltaTime,RankD_Frame);
				//이부분에 게이지 클립과 이펙트 효과 추가.
				
				//만약 게이지가 100을 넘는다면
				if (FullGauge == true)
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
			},
			.Update = [=](float _DeltaTime)
			{
			//이전 메쉬 내보내기
			//랭크 돌리기.
				RankSpin(_DeltaTime,RankC_Frame);
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

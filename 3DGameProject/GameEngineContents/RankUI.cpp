#include "PrecompileHeader.h"
#include "RankUI.h"
#include "UIFBXRenderer.h"
enum RankState
{
	
};
RankUI::RankUI() 
{
}

RankUI::~RankUI() 
{
}
/*
1. �����ϱ�
2. ��鸮��
2. zȸ����Ű��
3. ������
*/
void RankUI::Start()
{
	RankD_Frame = UIFBXActorBase::CreateGaugeBar({ 580.0f,80.0f,0.0f,-150.0f }, { 8.0f,8.0f,8.0f }, { -89.0f,0.0f,0.0f }, "RankDFrame.FBX");
//	RankD_Inside = UIFBXActorBase::CreateGaugeBar({ 0.0f,0.0f,0.0f,0.0f }, { 5.0f,5.0f,5.0f }, { -90.0f,0.0f,0.0f }, "RankDInside.FBX");
	RankD_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
}

void RankUI::Update(float _DeltaTime)
{
	//if (true == GameEngineInput::IsUp("UIDEBUGMODE"))
	//{
	//	CreateRank = true;
	//}
	//TestMove(_DeltaTime);
	//OutMove(_DeltaTime);
}

void RankUI::TestMove(float _Delta)
{
	//FSM �����ϱ�
	if (CreateRank == true)
	{
		Ratio += _Delta;
		RankD_Frame->GetTransform()->SetLocalPosition(
			float4::LerpClamp({ 580.0f,80.0f,0.0f,-150.0f }, { 660.0f,90.0f,0.0f,-150.0f }, Ratio*3.0f));
		RankD_Frame->GetTransform()->SetLocalScale(
			float4::LerpClamp({ 9.0f,9.0f,9.0f }, { 5.0f,5.0f,5.0f }, Ratio * 3.0f));
		if (Ratio+0.5f <= 1.0f)
		{
			RankD_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, Ratio+0.5f));
		}
		if (RankD_Frame->GetTransform()->GetLocalPosition() == float4{ 660.0f, 90.0f, 0.0f, -150.0f })
		{
			CreateRank = false;
			//CHANGE STATE
			ShakeRank = true;
		}
	}
	//FSM ����
	if (ShakeRank == true)
	{
		Ratio += _Delta;
		if (Ratio < 0.6f)
		{
			UIFBXActorBase::ShakeUIRender(RankD_Frame, float4(400.f, 400.f, 0.0f), _Delta);
		}
		else
		{
			RankD_Frame->GetTransform()->SetLocalPosition({ 660.0f,90.0f,0.0f,-150.0f });
			ShakeRank = false;
			TurnRank = true;
			Ratio = 0.0f;
		}
		//Inside�� ���� ������
		

	}
	if (TurnRank == true)
	{
		if (TurnValue == false)
		{
			Ratio += _Delta;
			if (TurnIndex % 2 == 0)
			{
				RankD_Frame->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,0.0f }, { -90.0f,0.0f,30.0f }, Ratio * 0.2f));
			}
			else
			{
				RankD_Frame->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,0.0f }, { -90.0f,0.0f,-30.0f }, Ratio * 0.2f));
			}
			if (Ratio>5.0f)
			{
				TurnValue = true;
				Ratio = 0.0f;
				OutRank = true;
				TurnIndex++;
			}
		}
		else
		{
			Ratio += _Delta;
			if (TurnIndex % 2 == 0)
			{
				RankD_Frame->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,-30.0f }, { -90.0f,0.0f,0.0f }, Ratio * 0.2f));
			}
			else
			{
				RankD_Frame->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,30.0f }, { -90.0f,0.0f,0.0f }, Ratio * 0.2f));
			}
			if (Ratio > 5.0f)
			{
				TurnValue = false;
				Ratio = 0.0f;
			}
		}
	


	}

}

void RankUI::OutMove(float _Delta)
{
	if (OutRank == true)
	{
		Ratio += _Delta;
		RankD_Frame->GetTransform()->SetLocalPosition(float4::LerpClamp({ 660.0f,90.0f,0.0f,-150.0f }, { 1060.0f,90.0f,0.0f,-150.0f }, Ratio * 2.0f));
		if (RankD_Frame->GetTransform()->GetLocalPosition() == float4{ 1060.0f, 90.0f, 0.0f, -150.0f })
		{
			//�ٽ� �⺻������ ����
			RankD_Frame->GetTransform()->SetLocalPosition({ { 660.0f,90.0f,0.0f,-150.0f } });
			RankD_Frame->GetTransform()->SetLocalRotation({ -90.0f,0.0f,0.0f });
			RankD_Frame->GetTransform()->SetLocalScale({ 8.0f,8.0f,8.0f });
			int a = 0;
		}
	}


}


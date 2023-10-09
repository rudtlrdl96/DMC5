#include "PrecompileHeader.h"
#include "RankUI.h"
#include "UIFBXRenderer.h"

RankUI::RankUI() 
{
}

RankUI::~RankUI() 
{
}
/*
1. 등장하기
2. 흔들리기
2. z회전시키기
3. 나가기
*/
void RankUI::Start()
{
	RankD_Frame = UIFBXActorBase::CreateGaugeBar({ 580.0f,80.0f,0.0f,-150.0f }, { 8.0f,8.0f,8.0f }, { -89.0f,0.0f,0.0f }, "RankDFrame.FBX");
//	RankD_Inside = UIFBXActorBase::CreateGaugeBar({ 0.0f,0.0f,0.0f,0.0f }, { 5.0f,5.0f,5.0f }, { -90.0f,0.0f,0.0f }, "RankDInside.FBX");
	RankD_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
}

void RankUI::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsUp("UIDEBUGMODE"))
	{
		TestValue = true;
	}
	TestMove(_DeltaTime);
}

void RankUI::TestMove(float _Delta)
{
	//FSM UPDATE
	if (TestValue == true)
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
			TestValue = false;
			//CHANGE STATE
			TestValue1 = true;
		}
	}
	if (TestValue1 == true)
	{
		Ratio += _Delta;
		if (Ratio < 0.6f)
		{
			UIFBXActorBase::ShakeUIRender(RankD_Frame, float4(400.f, 400.f, 0.0f), _Delta);
		}
		else
		{
			RankD_Frame->GetTransform()->SetLocalPosition({ 660.0f,90.0f,0.0f,-150.0f });
		}
		//Inside도 같이 돌리기
		//if (RankD_Frame->GetTransform()->GetLocalRotation().z < 30.0f)
		//{
		//	RankD_Frame->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,0.0f }, { -90.0f,0.0f,30.0f }, Ratio * 0.2f));
		//}

	}
	

}


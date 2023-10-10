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
1. �����ϱ�
2. ��鸮��
2. zȸ����Ű��
3. ������
*/
void RankUI::Start()
{
	RankD_Frame = UIFBXActorBase::CreateGaugeBar({ 580.0f,80.0f,0.0f,-150.0f }, { 9.0f,9.0f,9.0f }, { -89.0f,0.0f,0.0f }, "RankDFrame.FBX");
	RankD_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankD_Inside = UIFBXActorBase::CreateGaugeBar({ 660.0f,90.0f,-150.0f}, { 5.0f,5.0f,5.0f }, { -90.0f,0.0f,0.0f }, "RankDInside.FBX","FBX_ClipAlpha");
	RankD_Inside->SetClipData(float4(0.0f, 1.0f, 0.0f, 1.0f));
	
	RankC_Frame = UIFBXActorBase::CreateGaugeBar({ 580.0f,80.0f,0.0f,-150.0f }, { 9.0f,9.0f,9.0f }, { -89.0f,0.0f,0.0f }, "RankCFrame.FBX");
	RankC_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankC_Inside = UIFBXActorBase::CreateGaugeBar({ 0.0f,0.0f,0.0f,0.0f }, { 5.0f,5.0f,5.0f }, { -90.0f,0.0f,0.0f }, "RankBInside.FBX");
	RankC_Inside->SetClipData(float4(0.0f, 1.0f, 0.0f, 1.0f));

	RankB_Frame = UIFBXActorBase::CreateGaugeBar({ 580.0f,80.0f,0.0f,-150.0f }, { 9.0f,9.0f,9.0f }, { -89.0f,0.0f,0.0f }, "RankBFrame.FBX");
	RankB_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankB_Inside = UIFBXActorBase::CreateGaugeBar({ 0.0f,0.0f,0.0f,0.0f }, { 5.0f,5.0f,5.0f }, { -90.0f,0.0f,0.0f }, "RankCInside.FBX");
	RankB_Inside->SetClipData(float4(0.0f, 1.0f, 0.0f, 1.0f));

	RankA_Frame = UIFBXActorBase::CreateGaugeBar({ 580.0f,80.0f,0.0f,-150.0f }, { 9.0f,9.0f,9.0f }, { -89.0f,0.0f,0.0f }, "RankAFrame.FBX");
	RankA_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankA_Inside = UIFBXActorBase::CreateGaugeBar({ 660.0f,90.0f,-150.0f }, { 5.0f,5.0f,5.0f }, { -90.0f,0.0f,0.0f }, "RankAInside.FBX", "FBX_ClipAlpha");
	RankA_Inside->SetClipData(float4(0.0f, 1.0f, 0.0f, 1.0f));

	RankS_Frame = UIFBXActorBase::CreateGaugeBar({ 580.0f,80.0f,0.0f,-150.0f }, { 9.0f,9.0f,9.0f }, { -89.0f,0.0f,0.0f }, "RankSFrame.FBX");
	RankS_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankS_Inside = UIFBXActorBase::CreateGaugeBar({ 0.0f,0.0f,0.0f,0.0f }, { 5.0f,5.0f,5.0f }, { -90.0f,0.0f,0.0f }, "RankSInside.FBX");
	RankS_Inside->SetClipData(float4(0.0f, 1.0f, 0.0f, 1.0f));

	RankSS_Frame = UIFBXActorBase::CreateGaugeBar({ 580.0f,80.0f,0.0f,-150.0f }, { 9.0f,9.0f,9.0f }, { -89.0f,0.0f,0.0f }, "RankSSFrame.FBX");
	RankSS_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankSS_Inside = UIFBXActorBase::CreateGaugeBar({ 0.0f,0.0f,0.0f,0.0f }, { 5.0f,5.0f,5.0f }, { -90.0f,0.0f,0.0f }, "RankSSInside.FBX");
	RankSS_Inside->SetClipData(float4(0.0f, 1.0f, 0.0f, 1.0f));

	RankSSS_Frame = UIFBXActorBase::CreateGaugeBar({ 580.0f,80.0f,0.0f,-150.0f }, { 9.0f,9.0f,9.0f }, { -89.0f,0.0f,0.0f }, "RankSSSFrame.FBX");
	RankSSS_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankSSS_Inside = UIFBXActorBase::CreateGaugeBar({ 0.0f,0.0f,0.0f,0.0f }, { 5.0f,5.0f,5.0f }, { -90.0f,0.0f,0.0f }, "RankSSSInside.FBX", "FBX_ClipAlpha");
	RankSSS_Inside->SetClipData(float4(0.0f, 1.0f, 0.0f, 1.0f));

	StateInit_RankD();
	StateInit_RankC();
	StateInit_RankB();
	StateInit_RankA();
	StateInit_RankS();
	StateInit_RankSS();
	StateInit_RankSSS();

	RankFSM.ChangeState(RankState::Appear_RankD);

}

void RankUI::Update(float _DeltaTime)
{
	RankD_Inside->GetTransform()->SetLocalPosition(RankD_Frame->GetTransform()->GetLocalPosition());
	SetInsideMesh();
	RankFSM.Update(_DeltaTime);
	if (true == GameEngineInput::IsUp("UIDEBUGMODE"))
	{
		TestRankGauge += 100;
	}

}
void RankUI::RankSpin(float _Delta, std::shared_ptr<class UIFBXRenderer> _Render, std::shared_ptr<class UIFBXRenderer> _InsideRender)
{
	if (TurnValue == false)
	{
		Ratio += _Delta;
		if (TurnIndex % 2 == 0)
		{
			_Render->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,0.0f }, { -90.0f,0.0f,30.0f }, Ratio * 0.2f));
			_InsideRender->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,0.0f }, { -90.0f,0.0f,30.0f }, Ratio * 0.2f));

		}
		else
		{
			_Render->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,0.0f }, { -90.0f,0.0f,-30.0f }, Ratio * 0.2f));
			_InsideRender->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,0.0f }, { -90.0f,0.0f,-30.0f }, Ratio * 0.2f));
		}
		if (Ratio > 8.0f)
		{
			TurnValue = true;
			Ratio = 0.0f;
			TurnIndex++;
		}
	}
	else
	{
		Ratio += _Delta;
		if (TurnIndex % 2 == 0)
		{
			_Render->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,-30.0f }, { -90.0f,0.0f,0.0f }, Ratio * 0.2f));
			_InsideRender->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,-30.0f }, { -90.0f,0.0f,0.0f }, Ratio * 0.2f));
		}
		else
		{
			_Render->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,30.0f }, { -90.0f,0.0f,0.0f }, Ratio * 0.2f));
			_InsideRender->GetTransform()->SetLocalRotation(float4::LerpClamp({ -90.0f,0.0f,30.0f }, { -90.0f,0.0f,0.0f }, Ratio * 0.2f));

		}
		if (Ratio > 8.0f)
		{
			TurnValue = false;
			Ratio = 0.0f;
		}
	}
}

void RankUI::RankApper(float _Delta, std::shared_ptr<class UIFBXRenderer> _Render, RankState _State, bool _Value, std::shared_ptr<class UIFBXRenderer> _PrevRender)
{
	Ratio += _Delta;
	//�����ϱ�
	if (ShakeRank == false)
	{
		_Render->GetTransform()->SetLocalPosition(
			float4::LerpClamp({ 580.0f,80.0f,0.0f,-150.0f }, { 660.0f,90.0f,0.0f,-150.0f }, Ratio * 3.0f));
		_Render->GetTransform()->SetLocalScale(
			float4::LerpClamp({ 9.0f,9.0f,9.0f }, { 5.0f,5.0f,5.0f }, Ratio * 3.0f));
	}
	//alpha�� ���̱�
	if (Ratio + 0.5f <= 1.0f)
	{
		_Render->SetMulColor(float4(1.0f, 1.0f, 1.0f, Ratio + 0.5f));
	}
	if (_Render->GetTransform()->GetLocalPosition() == float4{ 660.0f, 90.0f, 0.0f, -150.0f })
	{
		ShakeRank = true;
		Ratio = 0.0f;
	}
	if (ShakeRank == true)
	{
		Ratio += _Delta;
		if (Ratio < 0.6f)
		{
			//���࿡ ���� ��ũ�� �ִٸ�
			if (_Value == true)
			{
				RankOut(_Delta, _PrevRender);
			}
			UIFBXActorBase::ShakeUIRender(_Render, float4(400.f, 400.f, 0.0f), _Delta);
		}
		else
		{
			_Render->GetTransform()->SetLocalPosition({ 660.0f,90.0f,0.0f,-150.0f });
			ShakeRank = false;
			RankFSM.ChangeState(_State);
			Ratio = 0.0f;
		}
	}
}

void RankUI::RankOut(float _Delta, std::shared_ptr<class UIFBXRenderer> _Render)
{
	Ratio += _Delta;
	if (OutRank == false)
	{
		_Render->GetTransform()->SetLocalPosition(float4::LerpClamp({ 660.0f,90.0f,0.0f,-150.0f }, { 1060.0f,90.0f,0.0f,-150.0f }, Ratio * 1.0f));
	}
	if (_Render->GetTransform()->GetLocalPosition().x == 1060.0f)
	{
		//�ٽ� �⺻������ ����
		_Render->GetTransform()->SetLocalPosition({ {  1060.0f, 90.0f, 0.0f, -150.0f } });
		_Render->GetTransform()->SetLocalRotation({ -90.0f,0.0f,0.0f });
		_Render->GetTransform()->SetLocalScale({ 9.0f,9.0f,9.0f });
		OutRank = true;
		Ratio = 0.0f;
	}
	if (Ratio + 0.5f <= 1.0f)
	{
		_Render->SetMulColor(float4(1.0f, 1.0f, 1.0f, 1- Ratio + 0.5f));

	}

}

void RankUI::SetInsideMesh()
{
	RankD_Inside->GetTransform()->SetLocalPosition({ RankD_Frame->GetTransform()->GetLocalPosition().x,RankD_Frame->GetTransform()->GetLocalPosition() .y,-150.0f});
	RankB_Inside->GetTransform()->SetLocalPosition({ RankC_Frame->GetTransform()->GetLocalPosition().x,RankC_Frame->GetTransform()->GetLocalPosition().y,-150.0f });
	RankC_Inside->GetTransform()->SetLocalPosition({ RankB_Frame->GetTransform()->GetLocalPosition().x,RankB_Frame->GetTransform()->GetLocalPosition().y,-150.0f });
	RankA_Inside->GetTransform()->SetLocalPosition({ RankA_Frame->GetTransform()->GetLocalPosition().x,RankA_Frame->GetTransform()->GetLocalPosition().y,-150.0f });
	RankS_Inside->GetTransform()->SetLocalPosition({ RankS_Frame->GetTransform()->GetLocalPosition().x,RankS_Frame->GetTransform()->GetLocalPosition().y,-150.0f });
	RankSS_Inside->GetTransform()->SetLocalPosition({ RankSS_Frame->GetTransform()->GetLocalPosition().x,RankSS_Frame->GetTransform()->GetLocalPosition().y,-150.0f });
	RankSSS_Inside->GetTransform()->SetLocalPosition({ RankSSS_Frame->GetTransform()->GetLocalPosition().x,RankSSS_Frame->GetTransform()->GetLocalPosition().y,-150.0f });

}



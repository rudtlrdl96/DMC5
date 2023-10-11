#include "PrecompileHeader.h"
#include "RankUI.h"
#include "UIFBXRenderer.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
RankUI* RankUI::MainRankUI = nullptr;
RankUI::RankUI() 
{
	MainRankUI = this;
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
void RankUI::AddRankScore(int _Score)
{
	RankScore += _Score*0.1f;
	ResetLiveTime();
}
void RankUI::Start()
{
	Rank_Explane = CreateComponent<GameEngineUIRenderer>();

	RankD_Frame = UIFBXActorBase::CreateGaugeBar(EndPos, StartScale, StartRotation, "RankDFrame.FBX");
	RankD_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankD_Inside = UIFBXActorBase::CreateGaugeBar(StartPos,EndScale, StartRotation, "RankDInside.FBX","FBX_ClipAlpha");
	RankD_Inside->SetClipData(float4(0.0f, 0.0f, 0.0f, 0.0f));
	
	RankC_Frame = UIFBXActorBase::CreateGaugeBar(EndPos, StartScale, StartRotation, "RankCFrame.FBX");
	RankC_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankC_Inside = UIFBXActorBase::CreateGaugeBar(InsidePos,EndScale, StartRotation, "RankCInside.FBX", "FBX_ClipAlpha");
	RankC_Inside->SetClipData(float4(0.0f, 0.0f, 0.0f, 0.0f));

	RankB_Frame = UIFBXActorBase::CreateGaugeBar(EndPos, StartScale, StartRotation, "RankBFrame.FBX");
	RankB_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankB_Inside = UIFBXActorBase::CreateGaugeBar(InsidePos,EndScale, StartRotation, "RankBInside.FBX", "FBX_ClipAlpha");
	RankB_Inside->SetClipData(float4(0.0f, 0.0f, 0.0f, 0.0f));

	RankA_Frame = UIFBXActorBase::CreateGaugeBar(EndPos, StartScale, StartRotation, "RankAFrame.FBX");
	RankA_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankA_Inside = UIFBXActorBase::CreateGaugeBar(InsidePos,EndScale, StartRotation, "RankAInside.FBX", "FBX_ClipAlpha");
	RankA_Inside->SetClipData(float4(0.0f, 0.0f, 0.0f, 0.0f));

	RankS_Frame = UIFBXActorBase::CreateGaugeBar(EndPos, StartScale, StartRotation, "RankSFrame.FBX");
	RankS_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankS_Inside = UIFBXActorBase::CreateGaugeBar(InsidePos,EndScale, StartRotation, "RankSInside.FBX", "FBX_ClipAlpha");
	RankS_Inside->SetClipData(float4(0.0f, 0.0f, 0.0f, 0.0f));

	RankSS_Frame = UIFBXActorBase::CreateGaugeBar(EndPos, StartScale, StartRotation, "RankSSFrame.FBX");
	RankSS_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankSS_Inside = UIFBXActorBase::CreateGaugeBar(InsidePos,EndScale, StartRotation, "RankSSInside.FBX", "FBX_ClipAlpha");
	RankSS_Inside->SetClipData(float4(0.0f, 0.0f, 0.0f, 0.0f));

	RankSSS_Frame = UIFBXActorBase::CreateGaugeBar(EndPos, StartScale, StartRotation, "RankSSSFrame.FBX");
	RankSSS_Frame->SetMulColor(float4(1.0f, 1.0f, 1.0f, 0.0f));
	RankSSS_Inside = UIFBXActorBase::CreateGaugeBar(InsidePos,EndScale, StartRotation, "RankSSSInside.FBX", "FBX_ClipAlpha");
	RankSSS_Inside->SetClipData(float4(0.0f, 0.0f, 0.0f, 0.0f));

	StateInit_Wait();
	StateInit_RankD();
	StateInit_RankC();
	StateInit_RankB();
	StateInit_RankA();
	StateInit_RankS();
	StateInit_RankSS();
	StateInit_RankSSS();

	RankFSM.ChangeState(RankState::Rank_WaitState);

}

void RankUI::Update(float _DeltaTime)
{
	RankFSM.Update(_DeltaTime);
	SetInsideMesh();
	if (DisApperValue == true)
	{
		Rank_Explane->GetTransform()->SetLocalPosition({ 1060.0f,-50.0f,0.0f });
		Rank_Explane->Off();
	}
	//if (true == GameEngineInput::IsDown("UI_Tab"))
	//{
	//	RankScore += 0.2f;
	//}
	//if (true == GameEngineInput::IsDown("UIDEBUGMODE"))
	//{
	//	DisApperValue = true;
	//	RankScore = 0;
	//}
	if (GetLiveTime() > 5.0f)
	{
		DisApperValue = true;
		//RankFSM.ChangeState(RankState::Rank_WaitState);
		RankScore = 0;
		ResetLiveTime();
		DisApperValue = true;
	
		//�ϳ��� �Ұ��� ����
		//�� �Ұ��� true�� �Ǹ� ������� ������Ʈ ����.
	}

}
void RankUI::RankSpin(float _Delta, std::shared_ptr<class UIFBXRenderer> _Render, std::shared_ptr<class UIFBXRenderer> _InsideRender)
{
	if (TurnValue == false)
	{
		Ratio += _Delta;
		if (TurnIndex % 2 == 0)
		{
			_Render->GetTransform()->SetLocalRotation(float4::LerpClamp(StartRotation, RightTurn, Ratio * 0.2f));
			_InsideRender->GetTransform()->SetLocalRotation(float4::LerpClamp(StartRotation, RightTurn, Ratio * 0.2f));

		}
		else
		{
			_Render->GetTransform()->SetLocalRotation(float4::LerpClamp(StartRotation, LeftTurn, Ratio * 0.2f));
			_InsideRender->GetTransform()->SetLocalRotation(float4::LerpClamp(StartRotation, LeftTurn, Ratio * 0.2f));
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
			_Render->GetTransform()->SetLocalRotation(float4::LerpClamp(LeftTurn, StartRotation, Ratio * 0.2f));
			_InsideRender->GetTransform()->SetLocalRotation(float4::LerpClamp(LeftTurn, StartRotation, Ratio * 0.2f));
		}
		else
		{
			_Render->GetTransform()->SetLocalRotation(float4::LerpClamp(RightTurn, StartRotation, Ratio * 0.2f));
			_InsideRender->GetTransform()->SetLocalRotation(float4::LerpClamp(RightTurn, StartRotation, Ratio * 0.2f));

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
			float4::LerpClamp(EndPos, StartPos, Ratio * 3.0f));
		_Render->GetTransform()->SetLocalScale(
			float4::LerpClamp(StartScale,EndScale, Ratio * 3.0f));
	}
	//alpha�� ���̱�
	if (Ratio + 0.5f <= 1.0f)
	{
		_Render->SetMulColor(float4(1.0f, 1.0f, 1.0f, Ratio + 0.5f));
	}
	if (_Render->GetTransform()->GetLocalPosition().x == StartPos.x)
	{
		ShakeRank = true;
		Ratio = 0.0f;
	}
	if (ShakeRank == true)
	{
		Ratio += _Delta;
		if (Ratio < 0.6f)
		{
			Rank_Explane->GetTransform()->SetLocalPosition({ 1060.0f,-50.0f,0.0f });
			//���࿡ ���� ��ũ�� �ִٸ�
			if (_Value == true)
			{
				RankOut(_Delta, _PrevRender);
			}
			UIFBXActorBase::ShakeUIRender(_Render, float4(400.f, 400.f, 0.0f), _Delta);
		}
		else
		{
			_Render->GetTransform()->SetLocalPosition(StartPos);
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
		_Render->GetTransform()->SetLocalPosition(float4::LerpClamp(StartPos, { StartPos.x+400.0f,StartPos.y,StartPos.z }, Ratio * 1.0f));
	}
	if (_Render->GetTransform()->GetLocalPosition().x == 1060.0f)
	{
		//�ٽ� �⺻������ ����
		_Render->GetTransform()->SetLocalPosition(InsideEnd);
		_Render->GetTransform()->SetLocalRotation(StartRotation);
		_Render->GetTransform()->SetLocalScale(StartScale);
		OutRank = true;
		Ratio = 0.0f;
	}
	if (Ratio + 0.5f <= 1.0f)
	{
		_Render->SetMulColor(float4(1.0f, 1.0f, 1.0f, 1- Ratio + 0.5f));

	}

}

void RankUI::RankScaleUpDown(std::shared_ptr<class UIFBXRenderer> _Render, std::shared_ptr<class UIFBXRenderer> _InsideRender, float _Ratio)
{
	ScaleSpeed += _Ratio;

	if (ScaleUpValue == false)
	{
		_Render->GetTransform()->SetLocalScale(float4::LerpClamp(EndScale, UpScale, ScaleSpeed *10.0f));
		_InsideRender->GetTransform()->SetLocalScale(float4::LerpClamp(EndScale, UpScale, ScaleSpeed * 10.0f));

		if (_Render->GetTransform()->GetLocalScale().x == UpScale.x)
		{
			ScaleUpValue = true;
			ScaleDownValue = true;
			ScaleSpeed = 0.0f;
		}
	}
	if(ScaleDownValue == true)
	{
		_Render->GetTransform()->SetLocalScale(float4::LerpClamp(UpScale, EndScale, ScaleSpeed * 10.0f));
		_InsideRender->GetTransform()->SetLocalScale(float4::LerpClamp(UpScale, EndScale, ScaleSpeed * 10.0f));
		if (_Render->GetTransform()->GetLocalScale().x == EndScale.x)
		{
			ScaleDownValue = false;
		}
	}



}

void RankUI::SetInsideMesh()
{
	RankD_Inside->GetTransform()->SetLocalPosition({ RankD_Frame->GetTransform()->GetLocalPosition().x,RankD_Frame->GetTransform()->GetLocalPosition() .y,-150.0f});
	RankC_Inside->GetTransform()->SetLocalPosition({ RankC_Frame->GetTransform()->GetLocalPosition().x,RankC_Frame->GetTransform()->GetLocalPosition().y,-150.0f });
	RankB_Inside->GetTransform()->SetLocalPosition({ RankB_Frame->GetTransform()->GetLocalPosition().x,RankB_Frame->GetTransform()->GetLocalPosition().y,-150.0f });
	RankA_Inside->GetTransform()->SetLocalPosition({ RankA_Frame->GetTransform()->GetLocalPosition().x,RankA_Frame->GetTransform()->GetLocalPosition().y,-150.0f });
	RankS_Inside->GetTransform()->SetLocalPosition({ RankS_Frame->GetTransform()->GetLocalPosition().x,RankS_Frame->GetTransform()->GetLocalPosition().y,-150.0f });
	RankSS_Inside->GetTransform()->SetLocalPosition({ RankSS_Frame->GetTransform()->GetLocalPosition().x,RankSS_Frame->GetTransform()->GetLocalPosition().y,-150.0f });
	RankSSS_Inside->GetTransform()->SetLocalPosition({ RankSSS_Frame->GetTransform()->GetLocalPosition().x,RankSSS_Frame->GetTransform()->GetLocalPosition().y,-150.0f });

}

void RankUI::RankDisApper(float _Delta, std::shared_ptr<class UIFBXRenderer> _Render, std::shared_ptr<class UIFBXRenderer> _InsideRender)
{
	if (DisApperValue == true)
	{
		DisTime += _Delta;
		_Render->SetMulColor(float4(1.0f, 1.0f, 1.0f, 1 - DisTime * 2.0f));
		_InsideRender->SetMulColor(float4(1.0f, 1.0f, 1.0f, 1 - DisTime * 2.0f));
		if (_Render->GetMulColor().w <= 0.0f)
		{
			DisTime = 0.0f;
			DisApperValue = false;
			//_Render->GetTransform()->SetLocalPosition(InsideEnd);
			//_InsideRender->GetTransform()->SetLocalPosition(InsideEnd);
			//_ExplaneRender->
			//_InsideRender->SetMulColor(float4::WHITE);
			//_InsideRender->SetMulColor(float4::WHITE);
			//_InsideRender->SetMulColor(float4::WHITE);

			RankFSM.ChangeState(RankState::Rank_WaitState);
		}
	}

}



void RankUI::SetRankExPlane(const std::string_view& _Png,float4 _Scale, float4 _Pos, float _Ratio)
{
	Rank_Explane->On();
	ExplaneSpeed += _Ratio;
	Rank_Explane->SetTexture(_Png);
	float4 Scale = _Scale * 0.5f;
	Rank_Explane->GetTransform()->SetLocalScale(Scale);
	Rank_Explane->GetTransform()->SetLocalPosition(float4::LerpClamp(float4(_Pos.x+400.0f, _Pos.y, _Pos.z)
		,_Pos, ExplaneSpeed*3.0f));
}

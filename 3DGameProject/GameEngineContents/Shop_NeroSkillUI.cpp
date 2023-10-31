#include "PrecompileHeader.h"
#include "Shop_NeroSkillUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include "Shop_ExplaneUI.h"
#include "RedOrbUI.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
bool Shop_NeroSkillUI::IsStreak =false;
bool Shop_NeroSkillUI::IsSplit = false;
bool Shop_NeroSkillUI::IsRedqueen = false;
bool Shop_NeroSkillUI::IsRouletteSpin = false;
bool Shop_NeroSkillUI::IsShufle = false;
bool Shop_NeroSkillUI::IsCalibur = false;
Shop_NeroSkillUI::Shop_NeroSkillUI()
{
}

Shop_NeroSkillUI::~Shop_NeroSkillUI()
{
}


void Shop_NeroSkillUI::Start()
{
	RedQueenButton = GetLevel()->CreateActor<Shop_TitleButton>();
	RedQueenButton->GetTransform()->SetParent(GetTransform());
	RedQueenButton->GetRender()->SetTexture("Shop_RedQueenUI.png");
	RedQueenButton->GetRender()->GetTransform()->SetLocalScale(float4{ 228.0f,62.0f,0.0f }*GameEngineActor::ScreenRatio);
	RedQueenButton->GetRender()->GetTransform()->SetLocalPosition(float4{ -630.0f,250.0f,0.0f }*GameEngineActor::ScreenRatio);
	RedQueenButton->GetRender_Bottom()->SetTexture("Shop_SkillUnderBar.png");
	RedQueenButton->GetRender_Bottom()->GetTransform()->SetLocalScale(float4{ 225.0f,28.0f,0.0f }*GameEngineActor::ScreenRatio);
	RedQueenButton->GetRender_Bottom()->GetTransform()->SetLocalPosition(float4{ -628.0f,205.0f,0.0f }*GameEngineActor::ScreenRatio);
	RedQueenButton->GetRender_Top()->SetTexture("Shop_SkillTopBar.png");
	RedQueenButton->GetRender_Top()->GetTransform()->SetLocalScale(float4{ 84.0f,23.0f,0.0f }*GameEngineActor::ScreenRatio);
	RedQueenButton->GetRender_Top()->GetTransform()->SetLocalPosition(float4{ -558.0f,292.0f,0.0f }*GameEngineActor::ScreenRatio);
	RedQueenButton->GetRender_Select()->SetTexture("Shop_SkillSelect.png");
	RedQueenButton->GetRender_Select()->GetTransform()->SetLocalScale(float4{ 228.0f,62.0f,0.0f }*GameEngineActor::ScreenRatio);
	RedQueenButton->GetRender_Select()->GetTransform()->SetLocalPosition(float4{ -630.0f,250.0f,0.0f }*GameEngineActor::ScreenRatio);
	RedQueenButton->SetSelectValue(true);
	RedQueenButton->SetBlinkValue(true);
	RedQueenButton->GetFont()->SetFont("DMC5Font");
	RedQueenButton->SetIsPosValue(true);
	RedQueenButton->GetFont()->GetTransform()->SetLocalPosition(float4{ -680.0f,224.0f,0.0f }*GameEngineActor::ScreenRatio);
	if (Shop_ItemButton::Skills.size() != 0)
	{
		Shop_ItemButton::Skills.clear();
	}
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_0 * GameEngineActor::ScreenRatio, { ._Name = "STREAK",._Price = "800",._png = "StreakSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_1 * GameEngineActor::ScreenRatio, { ._Name = "SPLIT",._Price = "1200",._png = "SplitSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_2 * GameEngineActor::ScreenRatio, { ._Name = "RED QUEEN COMBO C",._Price = "5000",._png = "RedQuennCSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_3 * GameEngineActor::ScreenRatio, { ._Name = "ROULETTE SPIN",._Price = "7000",._png = "RouletteSpinSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_4 * GameEngineActor::ScreenRatio, { ._Name = "SHUFLE",._Price = "8000",._png = "ShuffleSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_5 * GameEngineActor::ScreenRatio, { ._Name = "CALIBUR",._Price = "25000",._png = "CaliburSkill.png", ._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	ExplanePtr = GetLevel()->CreateActor<Shop_ExplaneUI>();
	ExplanePtr->GetTransform()->SetParent(GetTransform());
	ExplanePtr->GetTransform()->SetWorldPosition({ 0.0f,0.0f,0.0f });
	SetValue();
}

void Shop_NeroSkillUI::Update(float _Delta)
{
	SetIndex();
	Shop_ItemButton::Skills[PrevIndex]->SetSelectValue(false);
	Shop_ItemButton::Skills[Index]->SetSelectValue(true);
	ActivSkill();
	LerpItem(_Delta);
	SetExText();

}

void Shop_NeroSkillUI::LerpItem(float _Delta)
{
	if (DownSwich == true)
	{
		IsValue = true;
		LerpTime += _Delta;
		Shop_ItemButton::Skills[Index]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_5 * GameEngineActor::ScreenRatio, Pos_4 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 1]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_4 * GameEngineActor::ScreenRatio, Pos_3 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 2]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_3 * GameEngineActor::ScreenRatio, Pos_2 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 3]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_2 * GameEngineActor::ScreenRatio, Pos_1 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 4]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_1 * GameEngineActor::ScreenRatio, Pos_0 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 5]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_0 * GameEngineActor::ScreenRatio, Pos_Null * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		if (Shop_ItemButton::Skills[Index - 5]->GetTransform()->GetLocalPosition().y >= (Pos_Null.y * GameEngineActor::ScreenRatio.x + Pos_0.y * GameEngineActor::ScreenRatio.x) / 2)
		{
			Shop_ItemButton::Skills[Index - 5]->Off();
		}
		if (Shop_ItemButton::Skills[Index]->GetTransform()->GetLocalPosition() == Pos_4 * GameEngineActor::ScreenRatio)
		{
			DownSwich = false;
			LerpTime = 0.0f;
			IsValue = false;
		}
	}
	if (UpSwich == true)
	{
		IsValue = true;
		LerpTime += _Delta;
		Shop_ItemButton::Skills[Index]->On();
		Shop_ItemButton::Skills[Index]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_Null * GameEngineActor::ScreenRatio, Pos_0 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 1]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_0 * GameEngineActor::ScreenRatio, Pos_1 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 2]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_1 * GameEngineActor::ScreenRatio, Pos_2 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 3]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_2 * GameEngineActor::ScreenRatio, Pos_3 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 4]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_3 * GameEngineActor::ScreenRatio, Pos_4 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 5]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_4 * GameEngineActor::ScreenRatio, Pos_5 * GameEngineActor::ScreenRatio, LerpTime * 5.0f));

		if (Shop_ItemButton::Skills[Index]->GetTransform()->GetLocalPosition() == Pos_0 * GameEngineActor::ScreenRatio)
		{
			IsValue = false;
			UpSwich = false;
			LerpTime = 0.0f;
		}
	}
}

void Shop_NeroSkillUI::SetIndex()
{
	RedQueenButton->SetUIText("RED QUEEN");
	if (IsValue == false && true == GameEngineInput::IsUp("UI_UP"))
	{
		GameEngineSound::Play("ButtonSound2.wav");
		PrevIndex = Index;
		if (Index == 0)
		{
			Index = 5;
			DownSwich = true;
		}
		else
		{
			Index--;
		}
		if (Shop_ItemButton::Skills[1]->GetTransform()->GetLocalPosition() != Pos_1 * GameEngineActor::ScreenRatio && PrevIndex == 1 && Index == 0)
		{
			UpSwich = true;
		}


	}
	if (IsValue == false && true == GameEngineInput::IsUp("UI_DOWN"))
	{
		GameEngineSound::Play("ButtonSound2.wav");
		PrevIndex = Index;
		if (Index == 5)
		{
			Index = 0;
			UpSwich = true;
		}
		else
		{
			Index++;
		}
		if (Shop_ItemButton::Skills[5]->GetTransform()->GetLocalPosition() != Pos_4 * GameEngineActor::ScreenRatio && PrevIndex == 4 && Index == 5)
		{
			DownSwich = true;
		}


	}
}

void Shop_NeroSkillUI::SetExText()
{
	switch (Index)
	{
	case 0:
		ExplanePtr->SetExplaneText({ ._EName = "STREAK" ,._KName = "��Ʈ��ũ" ,._Explane1 = "���� ������������ �ֺ���",._Explane2 = "������ ��������ϴ�.",._Explane3 = "" });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "SteakCommand.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 523.0f,-138.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	case 1:
		ExplanePtr->SetExplaneText({ ._EName = "SPLIT" ,._KName = "���ø�" ,._Explane1 = "���� ���������� ��������",._Explane2 = "������ ��Ȯ�� �ѷ� ��������ϴ�.",._Explane3 = "" });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "SplitCommand.png"});
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 536.0f,-140.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	case 2:
		ExplanePtr->SetExplaneText({ ._EName = "RED QUEEN COMBO C" ,._KName = "���� �� �޺� C" ,._Explane1 = "ȭ���� �� �޺���",._Explane2 = "���� ���� ������ ����",._Explane3 = "������ ��ġ��ϴ�." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "RedQuennCCommand.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 477.0f,-170.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	case 3:
		ExplanePtr->SetExplaneText({ ._EName = "ROULETTE SPIN" ,._KName = "�귿 ����" ,._Explane1 = "���߿��� ���� ���� Į���� ȸ������",._Explane2 = "���� �������ϴ�.",._Explane3 = "���� �ణ �������ϴ�." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "RouletteSpinCommand.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 461.0f,-153.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	case 4:
		ExplanePtr->SetExplaneText({ ._EName = "SHUFLE" ,._KName = "����" ,._Explane1 = "�齺������ ������ ���� ��,",._Explane2 = "������ �����ϸ鼭 ������.",._Explane3 = "������ ī���͸� �����ϴ�." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "ShuffleSkillCommand.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 557.0f,-141.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	case 5:
		ExplanePtr->SetExplaneText({ ._EName = "CALIBUR" ,._KName = "Į����" ,._Explane1 = "���߿��� ������ ������",._Explane2 = "������ �ѹ��� �����ϴ�.",._Explane3 = "" });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "CaliburCommand.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 570.0f,-139.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;


	default:
		break;
	}
}

void Shop_NeroSkillUI::SetValue()
{
	for (int i = 0; i < Shop_ItemButton::Skills.size(); i++)
	{
		Shop_ItemButton::Skills[i]->SetSizeValue(true);
	}
}


void Shop_NeroSkillUI::ActivSkill()
{
	if (Index == 0)
	{
		if (IsStreak == false && GameEngineInput::IsUp("UI_Enter") == true && IsSelect ==true && RedOrbUI::GetRedOrb() > 800)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(800);
			IsStreak = true;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");
		}
		else if (IsStreak == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");
		}
	}
	else if (Index == 1)
	{
		if (IsSplit == false && GameEngineInput::IsUp("UI_Enter") == true && IsSelect == true && RedOrbUI::GetRedOrb() > 1200)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(1200);
			IsSplit = true;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");
		}
		else if (IsSplit == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");
		}
	}
	else if (Index == 2)
	{
		if (IsRedqueen == false && GameEngineInput::IsUp("UI_Enter") == true && IsSelect == true && RedOrbUI::GetRedOrb() > 5000)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(5000);

			IsRedqueen = true;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");

		}
		else if (IsRedqueen == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");
		}
	}
	else if (Index == 3)
	{
		if (IsRouletteSpin == false && GameEngineInput::IsUp("UI_Enter") == true && IsSelect == true && RedOrbUI::GetRedOrb() > 7000)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(7000);
			IsRouletteSpin = true;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");

		}
		else if (IsRouletteSpin == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");
		}
	}
	else if (Index == 4)
	{
		if (IsShufle == false && GameEngineInput::IsUp("UI_Enter") == true && IsSelect == true && RedOrbUI::GetRedOrb() > 8000)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(8000);
			IsShufle = true;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");

		}
		else if (IsShufle == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");
		}
	}
	else if (Index == 5)
	{
		if (IsCalibur == false && GameEngineInput::IsUp("UI_Enter") == true && IsSelect == true && RedOrbUI::GetRedOrb() > 25000)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(2500);
			IsCalibur = true;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");

		}
		else if (IsCalibur == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("���ſϷ�");
		}
	}
}


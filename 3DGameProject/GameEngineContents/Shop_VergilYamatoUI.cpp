#include "PrecompileHeader.h"
#include "Shop_VergilYamatoUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include "Shop_ExplaneUI.h"
#include "Vergil_ShopUI.h"
#include "RedOrbUI.h"
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
bool Shop_VergilYamatoUI::IsRapidSlash = false;
bool Shop_VergilYamatoUI::IsUpperSlash = false;
bool Shop_VergilYamatoUI::IsAerialRave = false;
bool Shop_VergilYamatoUI::IsYamatoCombo = false;
bool Shop_VergilYamatoUI::IsJudgmentCutEnd = false;
Shop_VergilYamatoUI::Shop_VergilYamatoUI()
{
}

Shop_VergilYamatoUI::~Shop_VergilYamatoUI()
{
}

void Shop_VergilYamatoUI::Start()
{
	if (Shop_ItemButton::Skills.size() != 0)
	{
		Shop_ItemButton::Skills.clear();
	}

	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_0 * GameEngineActor::ScreenRatio, { ._Name = "RAPID SLASH",._Price = "1200",._png = "StreakSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_1 * GameEngineActor::ScreenRatio, { ._Name = "UPPER SLASH",._Price = "4000",._png = "SplitSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_2 * GameEngineActor::ScreenRatio, { ._Name = "AERIAL RAVE B",._Price = "7500",._png = "RedQuennCSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_3 * GameEngineActor::ScreenRatio, { ._Name = "YAMATO COMBO C",._Price = "9000",._png = "RouletteSpinSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_4 * GameEngineActor::ScreenRatio, { ._Name = "JUDGMENT CUT END",._Price = "35000",._png = "ShuffleSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f) * GameEngineActor::ScreenRatio, float4(-160.0f, 0.0f, 0.0f) * GameEngineActor::ScreenRatio);

	ExplanePtr = GetLevel()->CreateActor<Shop_ExplaneUI>();
	ExplanePtr->GetTransform()->SetParent(GetTransform());
	ExplanePtr->GetTransform()->SetWorldPosition({ 0.0f,0.0f,0.0f });
	SetValue();
}

void Shop_VergilYamatoUI::Update(float _Delta)
{
	if (Vergil_ShopUI::Vergil_ShopBar->GetUIIndex() == 1)
	{
		SetIndex();
		Shop_ItemButton::Skills[PrevIndex]->SetSelectValue(false);
		Shop_ItemButton::Skills[Index]->SetSelectValue(true);
		SetExText();
	}
	else
	{
		Shop_ItemButton::Skills[PrevIndex]->SetSelectValue(false);
		Shop_ItemButton::Skills[Index]->SetSelectValue(false);
		PrevIndex = 4;
		Index = 0;
	}
	ActivSkill();
}

void Shop_VergilYamatoUI::SetIndex()
{
	if (true == GameEngineInput::IsUp("UI_UP"))
	{
		GameEngineSound::Play("ButtonSound.wav");
		PrevIndex = Index;
		if (Index == 0)
		{
			Index = 4;
		}
		else
		{
			Index--;
		}


	}
	if (true == GameEngineInput::IsUp("UI_DOWN"))
	{
		GameEngineSound::Play("ButtonSound.wav");
		PrevIndex = Index;
		if (Index == 4)
		{
			Index = 0;
		}
		else
		{
			Index++;
		}
	


	}
}

void Shop_VergilYamatoUI::SetExText()
{
	switch (Index)
	{
	case 0:
		ExplanePtr->SetExplaneText({ ._EName = "RAPID SLASH" ,._KName = "래피드 슬래시" ,._Explane1 = "앞으로 돌진하면서 검 공격으로 모든 것들을",._Explane2 = "가차없이 갈라버립니다.",._Explane3 = "진 데빌 트리거 상태에서는 검 공격과 동시에" 
			,._Explane4 = "미라지 블레이드로 공격해 검을 검자루에 넣을",._Explane5 = "때 폭파시켜 추가 데미지를 줍니다. 몇 번이고"
			,._Explane6 = "계속해서 발동할 수 있습니다."});
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "RapidSlashExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 506.0f,-142.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	case 1:
		ExplanePtr->SetExplaneText({ ._EName = "UPPER SLASH" ,._KName = "어퍼 슬래시" ,._Explane1 = "적을 공격해 공중으로 날려버리고 공격을",._Explane2 = "홀드하여 공중으로 쫓아갑니다."});
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "Uperslah2Explane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 502.0f,-145.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	case 2:
		ExplanePtr->SetExplaneText({ ._EName = "AERIAL RAVE B" ,._KName = "에어리얼 레이브 B" ,._Explane1 = "공중에서 회전하면서 적을 산산조각",._Explane2 = "내고, 혼신의 일격으로 땅에 내리꽂는",._Explane3 = "콤보." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "AerialRaveBExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 502.0f,-143.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	case 3:
		ExplanePtr->SetExplaneText({ ._EName = "YAMATO COMBO C" ,._KName = "야마토 콤보 C" ,._Explane1 = "무수한 공격을 퍼부은 다음 적을",._Explane2 = "둘로 갈라 버립니다.",._Explane3 = "진 데빌 트리거 상태에서는 피니싱"
			,._Explane4 = "블로의 범위가 더 넓어집니다."});
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "YamatoCombocExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 497.0f,-142.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	case 4:
		ExplanePtr->SetExplaneText({ ._EName = "JUDGMENT CUT END" ,._KName = "저지먼트 컷 엔드" ,._Explane1 = "빠른 속도로 동시에 여러 장소에 자신의",._Explane2 = "분신을 만들어 적을 베어 버립니다.",._Explane3 = "발동하면 SDT 게이지를 소모합니다." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "JudgementcutendExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition(float4{ 531.0f,-170.0f,0.0f }*GameEngineActor::ScreenRatio);
		break;
	default:
		break;
	}
}

void Shop_VergilYamatoUI::SetValue()
{
	for (int i = 0; i < Shop_ItemButton::Skills.size(); i++)
	{
		Shop_ItemButton::Skills[i]->SetSizeValue(true);
	}
}

void Shop_VergilYamatoUI::ActivSkill()
{
	if (Index == 0)
	{
		if (IsRapidSlash == false && GameEngineInput::IsUp("UI_Enter") == true && RedOrbUI::GetRedOrb() >= 1200 && IsActive == true)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(1200);
			IsRapidSlash = true;
			Count++;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("구매완료");
		}
		else if (IsRapidSlash == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("구매완료");
		}
	}
	else if (Index == 1)
	{
		if (IsUpperSlash == false && GameEngineInput::IsUp("UI_Enter") == true && RedOrbUI::GetRedOrb() >= 4000 && IsActive == true)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(4000);
			IsUpperSlash = true;
			Count++;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("구매완료");
		}
		else if (IsUpperSlash == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("구매완료");
		}
	}
	else if (Index == 2)
	{
		if (IsAerialRave == false && GameEngineInput::IsUp("UI_Enter") == true && RedOrbUI::GetRedOrb() >= 7500 && IsActive == true)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(7500);
			IsAerialRave = true;
			Count++;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("구매완료");

		}
		else if (IsAerialRave == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("구매완료");
		}
	}
	else if (Index == 3)
	{
		if (IsYamatoCombo == false && GameEngineInput::IsUp("UI_Enter") == true && RedOrbUI::GetRedOrb() >= 9000 && IsActive == true)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(9000);
			IsYamatoCombo = true;
			Count++;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("구매완료");

		}
		else if (IsYamatoCombo == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("구매완료");
		}
	}
	else if (Index == 4)
	{
		if (IsJudgmentCutEnd == false && GameEngineInput::IsUp("UI_Enter") == true && RedOrbUI::GetRedOrb() >= 35000 && IsActive == true)
		{
			GameEngineSound::Play("UIButtonSelectSound.wav");
			RedOrbUI::SubRedOrb(35000);
			IsJudgmentCutEnd = true;
			Count++;
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("구매완료");

		}
		else if (IsJudgmentCutEnd == true && GameEngineInput::IsUp("UI_Enter"))
		{
			GameEngineSound::Play("ButtonLockSound.wav");
			Shop_ItemButton::Skills[Index]->GetBuyRender()->SetText("구매완료");
		}
	}
}


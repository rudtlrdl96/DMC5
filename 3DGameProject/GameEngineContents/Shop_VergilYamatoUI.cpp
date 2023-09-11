#include "PrecompileHeader.h"
#include "Shop_VergilYamatoUI.h"
#include "Shop_TitleButton.h"
#include "Shop_ItemButton.h"
#include "Shop_ExplaneUI.h"

#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineLevel.h>
Shop_VergilYamatoUI::Shop_VergilYamatoUI()
{
}

Shop_VergilYamatoUI::~Shop_VergilYamatoUI()
{
}

void Shop_VergilYamatoUI::Start()
{
	YamatoButton = GetLevel()->CreateActor<Shop_TitleButton>();
	YamatoButton->GetTransform()->SetParent(GetTransform());
	YamatoButton->GetRender()->SetTexture("YamatoBase.png");
	YamatoButton->GetRender()->GetTransform()->SetLocalScale({ 228.0f,62.0f,0.0f });
	YamatoButton->GetRender()->GetTransform()->SetLocalPosition({ -630.0f,250.0f,0.0f });
	YamatoButton->GetRender_Bottom()->SetTexture("Shop_SkillUnderBar.png");
	YamatoButton->GetRender_Bottom()->GetTransform()->SetLocalScale({ 225.0f,28.0f,0.0f });
	YamatoButton->GetRender_Bottom()->GetTransform()->SetLocalPosition({ -628.0f,205.0f,0.0f });
	YamatoButton->GetRender_Top()->SetTexture("Shop_SkillTopBar.png");
	YamatoButton->GetRender_Top()->GetTransform()->SetLocalScale({ 84.0f,23.0f,0.0f });
	YamatoButton->GetRender_Top()->GetTransform()->SetLocalPosition({ -558.0f,292.0f,0.0f });
	YamatoButton->GetRender_Select()->SetTexture("Shop_SkillSelect.png");
	YamatoButton->GetRender_Select()->GetTransform()->SetLocalScale({ 228.0f,62.0f,0.0f });
	YamatoButton->GetRender_Select()->GetTransform()->SetLocalPosition({ -630.0f,250.0f,0.0f });
	YamatoButton->SetSelectValue(true);
	YamatoButton->GetFont()->SetFont("DMC5Font");
	YamatoButton->SetIsPosValue(true);
	YamatoButton->GetFont()->GetTransform()->SetLocalPosition({ -680.0f,224.0f,0.0f });
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_0, { ._Name = "RAPID SLASH",._Price = "1200",._png = "StreakSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_1, { ._Name = "UPPER SLASH",._Price = "4000",._png = "SplitSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_2, { ._Name = "AERIAL RAVE B",._Price = "7500",._png = "RedQuennCSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_3, { ._Name = "YAMATO COMBO C",._Price = "9000",._png = "RouletteSpinSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png" ,.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));
	Shop_ItemButton::CreateItemUI(GetLevel(), Pos_4, { ._Name = "JUDGMENT CUT END",._Price = "35000",._png = "ShuffleSkill.png",._BaseNone = "Shop_SkillMovieNone.png", ._BaseSelect = "Shop_SkillMovie.png",.IsValue = true }, GetTransform(), float4(157.0f, 100.0f, 0.0f), float4(-160.0f, 0.0f, 0.0f));

	ExplanePtr = GetLevel()->CreateActor<Shop_ExplaneUI>();
	ExplanePtr->GetTransform()->SetParent(GetTransform());
	ExplanePtr->GetTransform()->SetWorldPosition({ 0.0f,0.0f,0.0f });
	SetValue();
}

void Shop_VergilYamatoUI::Update(float _Delta)
{
	SetIndex();
	Shop_ItemButton::Skills[PrevIndex]->SetSelectValue(false);
	Shop_ItemButton::Skills[Index]->SetSelectValue(true);
	//LerpItem(_Delta);
	SetExText();

}

void Shop_VergilYamatoUI::LerpItem(float _Delta)
{
	if (DownSwich == true)
	{
		IsValue = true;
		LerpTime += _Delta;
		Shop_ItemButton::Skills[Index]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_5, Pos_4, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 1]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_4, Pos_3, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 2]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_3, Pos_2, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 3]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_2, Pos_1, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 4]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_1, Pos_0, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index - 5]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_0, Pos_Null, LerpTime * 5.0f));

		if (Shop_ItemButton::Skills[Index - 5]->GetTransform()->GetLocalPosition().y >= (Pos_Null.y + Pos_0.y) / 2)
		{
			Shop_ItemButton::Skills[Index - 5]->Off();
		}
		if (Shop_ItemButton::Skills[Index]->GetTransform()->GetLocalPosition() == Pos_4)
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
		Shop_ItemButton::Skills[Index]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_Null, Pos_0, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 1]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_0, Pos_1, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 2]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_1, Pos_2, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 3]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_2, Pos_3, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 4]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_3, Pos_4, LerpTime * 5.0f));

		Shop_ItemButton::Skills[Index + 5]->GetTransform()->SetLocalPosition(float4::LerpClamp(Pos_4, Pos_5, LerpTime * 5.0f));

		if (Shop_ItemButton::Skills[Index]->GetTransform()->GetLocalPosition() == Pos_0)
		{
			IsValue = false;
			UpSwich = false;
			LerpTime = 0.0f;
		}
	}
}

void Shop_VergilYamatoUI::SetIndex()
{
	YamatoButton->SetUIText("YAMATO");
	if (IsValue == false && true == GameEngineInput::IsUp("UI_UP"))
	{
		PrevIndex = Index;
		if (Index == 0)
		{
			Index = 4;
			DownSwich = true;
		}
		else
		{
			Index--;
		}
		if (PrevIndex == 1 && Index == 0)
		{
			UpSwich = true;
		}


	}
	if (IsValue == false && true == GameEngineInput::IsUp("UI_DOWN"))
	{
		PrevIndex = Index;
		if (Index == 4)
		{
			Index = 0;
			UpSwich = true;
		}
		else
		{
			Index++;
		}
		if (PrevIndex == 3 && Index == 4)
		{
			DownSwich = true;
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
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition({ 506.0f,-142.0f,0.0f });
		break;
	case 1:
		ExplanePtr->SetExplaneText({ ._EName = "UPPER SLASH" ,._KName = "어퍼 슬래시" ,._Explane1 = "적을 공격해 공중으로 날려버리고 공격을",._Explane2 = "홀드하여 공중으로 쫓아갑니다."});
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "Uperslah2Explane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition({ 502.0f,-145.0f,0.0f });
		break;
	case 2:
		ExplanePtr->SetExplaneText({ ._EName = "AERIAL RAVE B" ,._KName = "에어리얼 레이브 B" ,._Explane1 = "공중에서 회전하면서 적을 산산조각",._Explane2 = "내고, 혼신의 일격으로 땅에 내리꽂는",._Explane3 = "콤보." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "AerialRaveBExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition({ 502.0f,-143.0f,0.0f });
		break;
	case 3:
		ExplanePtr->SetExplaneText({ ._EName = "YAMATO COMBO C" ,._KName = "야마토 콤보 C" ,._Explane1 = "무수한 공격을 퍼부은 다음 적을",._Explane2 = "둘로 갈라 버립니다.",._Explane3 = "진 데빌 트리거 상태에서는 피니싱"
			,._Explane4 = "블로의 범위가 더 넓어집니다."});
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "YamatoCombocExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition({ 497.0f,-142.0f,0.0f });
		break;
	case 4:
		ExplanePtr->SetExplaneText({ ._EName = "JUDGMENT CUT END" ,._KName = "저지먼트 컷 엔드" ,._Explane1 = "빠른 속도로 동시에 여러 장소에 자신의",._Explane2 = "분신을 만들어 적을 베어 버립니다.",._Explane3 = "발동하면 SDT 게이지를 소모합니다." });
		ExplanePtr->SetSecoundText({ ._EName = "COMMAND" ,._Explane1 = "JudgementcutendExplane.png" });
		ExplanePtr->GetExplane()->GetTransform()->SetLocalPosition({ 531.0f,-170.0f,0.0f });
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


#include "PrecompileHeader.h"
#include "DebugKeyUI.h"
#include <queue>
#include <GameEngineCore/GameEngineUIRenderer.h>
DebugKeyUI::DebugKeyUI() 
{
}

DebugKeyUI::~DebugKeyUI() 
{
}

void DebugKeyUI::Start()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Texture");
	NewDir.Move("UI");
	NewDir.Move("PlayLevelUI");
	NewDir.Move("KeyboardTexture");
	if (nullptr == GameEngineTexture::Find("A.png"))
	{
		std::vector<GameEngineFile> File = NewDir.GetAllFile({ ".Png", });
		for (size_t i = 0; i < File.size(); i++)
		{
			GameEngineTexture::Load(File[i].GetFullPath());
		}
	}
	KeyRender0 = CreateComponent<GameEngineUIRenderer>();
	KeyRender0->GetTransform()->SetLocalPosition(Pos);
	KeyRender0->SetScaleToTexture("Nulltexture.png");
	KeyRender0->Off();
	KeyRenders.push_back(KeyRender0);

	KeyRender1 = CreateComponent<GameEngineUIRenderer>();
	KeyRender1->GetTransform()->SetLocalPosition(Pos-SubPos);
	KeyRender1->SetScaleToTexture("Nulltexture.png");
	KeyRender1->Off();
	KeyRenders.push_back(KeyRender1);

	KeyRender2 = CreateComponent<GameEngineUIRenderer>();
	KeyRender2->GetTransform()->SetLocalPosition(Pos - SubPos*2);
	KeyRender2->SetScaleToTexture("Nulltexture.png");
	KeyRender2->Off();
	KeyRenders.push_back(KeyRender2);

	KeyRender3 = CreateComponent<GameEngineUIRenderer>();
	KeyRender3->GetTransform()->SetLocalPosition(Pos - SubPos*3);
	KeyRender3->SetScaleToTexture("Nulltexture.png");
	KeyRender3->Off();
	KeyRenders.push_back(KeyRender3);

	KeyRender4 = CreateComponent<GameEngineUIRenderer>();
	KeyRender4->GetTransform()->SetLocalPosition(Pos - SubPos*4);
	KeyRender4->SetScaleToTexture("Nulltexture.png");
	KeyRender4->Off();
	KeyRenders.push_back(KeyRender4);
}

void DebugKeyUI::Update(float _Delta)
{
	
	if (true == GameEngineInput::IsDown("UI_KeyBordDebug"))
	{
		Count++;
		if (Count % 2 == 1)
		{
			KeyDebugOn();
		}
		else
		{
			KeyDebugOff();
		}
	}
	if (true == GameEngineInput::IsDown("Player_Up"))
	{
		InputKeys.push_front("W.png");
	}
	else if (true == GameEngineInput::IsDown("Player_Left"))
	{
		InputKeys.push_front("A.png");
	}
	else if (true == GameEngineInput::IsDown("Player_Down"))
	{
		InputKeys.push_front("S.png");
	}
	else if (true == GameEngineInput::IsDown("Player_Right"))
	{
		InputKeys.push_front("D.png");
	}

	UpdateKeyRender();
}

void DebugKeyUI::KeyDebugOff()
{
	for (int i = 0; i < KeyRenders.size(); i++)
	{
		KeyRenders[i]->Off();
	}
}

void DebugKeyUI::KeyDebugOn()
{
	for (int i = 0; i < KeyRenders.size(); i++)
	{
		KeyRenders[i]->On();
	}
}

void DebugKeyUI::UpdateKeyRender()
{
	if (5 < InputKeys.size())
	{
		InputKeys.pop_back();
	}

	int Index = 0;

	for (const std::string& _Ref : InputKeys)
	{
		KeyRenders[Index++]->SetScaleToTexture(_Ref);
	}
}


#include "PrecompileHeader.h"
#include "LevelChangeZone.h"
#include <GameEngineCore/GameEngineCollision.h>
#include "ThreadLoadingLevel.h"
#include "EffectRenderer.h"
#include "FadeEffect.h"
LevelChangeZone::LevelChangeZone()
{

}

LevelChangeZone::~LevelChangeZone()
{

}

void LevelChangeZone::ChangeLevel()
{
	if (ChangeLevelName == "")
	{
		MsgAssert("ChangeLevelName가 정해지지 않았습니다");
	}
	FadeEffect::GetFadeEffect()->FadeOut(0.5f);
	GetLevel()->TimeEvent.AddEvent(.5f, [=](GameEngineTimeEvent::TimeEvent _Event, GameEngineTimeEvent* _Manager)
		{
			ThreadLoadingLevel::ChangeLevel(ChangeLevelName);
		});
}

void LevelChangeZone::Start()
{
	EventZone::Start();
	Option = EventZoneOption::OnePlayer;
	Event = [this]()
		{
			LevelChangeZone::ChangeLevel();
		};
	
	if (nullptr == GameEngineTexture::Find("DistortionSample_02.jpg"))
	{
		GameEngineTexture::Load(GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Texture", "DistortionTexture"
			}, "DistortionSample_02.jpg"));
	}

	DistortionEffect.resize(2);
	for (size_t i = 0; i < DistortionEffect.size(); i++)
	{
		DistortionEffect[i] = CreateComponent<EffectRenderer>();
		DistortionEffect[i]->RectInit("Effect_2D");
		DistortionEffect[i]->SetTexture("DiffuseTexture", "EngineBaseNormal.png");
	}

	DistortionEffect[0]->GetTransform()->SetLocalScale({ 1, 1, 1 });
	DistortionEffect[1]->GetTransform()->SetLocalScale({ 1, 1, 1 });


	DistortionEffect[0]->GetTransform()->SetLocalPosition({ 0, 0, GetTransform()->GetLocalScale().half().z, 0 });
	DistortionEffect[1]->GetTransform()->SetLocalPosition({ 0, 0, -GetTransform()->GetLocalScale().half().z, 0 });
	
	for (size_t i = 0; i < DistortionEffect.size(); i++)
	{
		DistortionEffect[i]->EffectOption.MulColor = { 0.5, 0, 1, 0.2f };
		DistortionEffect[i]->SetDistortionTexture("noise_02.png");
		DistortionEffect[i]->PlayDistortionAnimation({ 4.0f, 2.0f }, 8.0f, 8.0f);
	}
}

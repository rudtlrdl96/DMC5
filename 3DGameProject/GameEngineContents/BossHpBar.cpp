#include "PrecompileHeader.h"
#include "BossHpBar.h"
#include "UIFBXRenderer.h"
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include "UIFBXActorBase.h"
BossHpBar::BossHpBar() 
{
}

BossHpBar::~BossHpBar() 
{
}

void BossHpBar::Start()
{

	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Mesh");
	NewDir.Move("UIMesh");
	NewDir.Move("NeroHPMesh");
	NewDir.Move("Gauge");
	NewDir.Move("BossGauge");
	if (nullptr == GameEngineFBXMesh::Find("BossHPBar.FBX"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}
	GameEngineDirectory TextureDir;
	TextureDir.MoveParentToDirectory("ContentResources");
	TextureDir.Move("ContentResources");
	TextureDir.Move("Texture");
	TextureDir.Move("UI");
	TextureDir.Move("PlayLevelUI");
	if (nullptr == GameEngineSprite::Find("HPGaugeAni.png"))
	{
		GameEngineSprite::LoadSheet(TextureDir.GetPlusFileName("HPGaugeAni.png").GetFullPath(), 4, 4);
	}
	if (nullptr == GameEngineTexture::Find("BossHPBase.png"))
	{
		GameEngineTexture::Load(TextureDir.GetPlusFileName("BossHPBase.png").GetFullPath());
	}
	if (nullptr == GameEngineTexture::Find("BossHitGauge.png"))
	{
		GameEngineTexture::Load(TextureDir.GetPlusFileName("BossHitGauge.png").GetFullPath());
	}
	if (nullptr == GameEngineTexture::Find("BossHPhit.png"))
	{
		GameEngineTexture::Load(TextureDir.GetPlusFileName("BossHPhit.png").GetFullPath());
	}

	BossHpAni = CreateComponent<GameEngineUIRenderer>(4);
	BossHpAni->SetTexture("HPGaugeAni.png");
	BossHpAni->CreateAnimation({ .AnimationName = "BossHPANI", .SpriteName = "HPGaugeAni.png", .Start = 5, .End = 7, .Loop = true, .FrameTime = {.15f, .15f,.15f, .15f,.15f, .15f,.15f, .15f} });
	BossHpAni->GetTransform()->SetLocalScale(AniScale);
	BossHpAni->GetTransform()->SetLocalPosition(AniPos);
	BossHpAni->GetTransform()->SetLocalRotation({ 0.0f,0.0f,90.0f });
	BossHpAni->Off();
	BossHpAni->ChangeAnimation("BossHPANI");
	
	BossHPUI_Front = CreateComponent<GameEngineUIRenderer>(1);
	BossHPUI_Front->SetTexture("BossHPBase.png");
	BossHPUI_Front->GetTransform()->SetLocalScale(Scale);
	BossHPUI_Front->GetTransform()->SetLocalPosition(Pos);
	BossHPUI_Front->BSCControl(0.7f, 0.5f, 0.5f);
	BossHPUI_Front->ColorOptionValue.MulColor.b = 1.2f;
	BossHPUI_Back = CreateComponent<GameEngineUIRenderer>(0);
	BossHPUI_Back->SetTexture("BossHitGauge.png");
	BossHPUI_Back->GetTransform()->SetLocalScale(Scale);
	BossHPUI_Back->GetTransform()->SetLocalPosition(Pos);

	BossHPUI_Base = UIFBXActorBase::CreateNoneLightGaugeBar({ 0.0f ,-400.0f , 0.0f , 0.0f }, { 9.4f,9.7f,5.0f }, { 0.0f,0.0f,0.0f }, "BossHPBar.FBX", "FBX_ClipAlpha");
}

void BossHpBar::Update(float _Delta)
{
	if (CurBossHp != MaxBossHP)
	{
		BossHpAni->On();
	}
	BossHPUI_Front->ImageClippingX(static_cast<float>(CurBossHp) / static_cast<float>(MaxBossHP), ClipXDir::Left);
	float Cal = static_cast<float>(CurBossHp) / static_cast<float>(MaxBossHP);
	float XPos = AniPos.x - Scale.x * (1.0f - Cal);
	BossHpAni->GetTransform()->SetLocalPosition({ XPos,AniPos.y,0.0f });
	if (CurBossHp != RedBossHp)
	{
		Ratio += _Delta;
		RedBossHp = static_cast<int>(GameEngineMath::LerpLimit(static_cast<float>(PrevBossHp), static_cast<float>(CurBossHp), _Delta * Ratio * 30.0f));
		BossHPUI_Back->ImageClippingX(static_cast<float>(RedBossHp) / static_cast<float>(MaxBossHP), ClipXDir::Left);
	}
	else
	{
		PrevBossHp = CurBossHp;
		Ratio = 0.0f;
	}
}


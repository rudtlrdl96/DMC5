#include "PrecompileHeader.h"
#include "PlayUITestLevel.h"
#include "NeroHPUI.h"
#include "NeroItemGlass.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineFont.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include "NeroLoading.h"
#include "C_ChoiceBG.h"
#include "BossHpBar.h"
#include "RankUI.h"
#include "DistortionEffect.h"

PlayUITestLevel::PlayUITestLevel()
{
}

PlayUITestLevel::~PlayUITestLevel()
{
}

void PlayUITestLevel::Start()
{

	std::shared_ptr<DistortionEffect> Distortion = GetCamera(100)->GetCamTarget()->CreateEffect<DistortionEffect>();
	Distortion->SetMaskTexture(GetCamera(0)->GetCamAlphaTarget(), 1);
	Distortion->SetDistortionValue(8, 4.5);
	{
		GameEngineDirectory NewMeshDir;
		NewMeshDir.MoveParentToDirectory("ContentResources");
		NewMeshDir.Move("ContentResources");
		NewMeshDir.Move("Mesh");
		NewMeshDir.Move("UIMesh");
		NewMeshDir.Move("RankMesh");

		if (nullptr == GameEngineFBXMesh::Find("RankAFrame.FBX"))
		{
			std::vector<GameEngineFile> Files = NewMeshDir.GetAllFile({ ".fbx" });
			for (GameEngineFile File : Files)
			{
				GameEngineFBXMesh::Load(File.GetFullPath());
			}
		}
	}
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Texture");
		NewDir.Move("UI");
		NewDir.Move("PlayLevelUI");
		if (nullptr == GameEngineSprite::Find("HPGaugeAni.png"))
		{
			GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("HPGaugeAni.png").GetFullPath(), 4, 4);
			GameEngineTexture::Load(NewDir.GetPlusFileName("BossGaugeBase.png").GetFullPath()); 
			GameEngineTexture::Load(NewDir.GetPlusFileName("BossHitGauge.png").GetFullPath());
			GameEngineTexture::Load(NewDir.GetPlusFileName("123123123.png").GetFullPath());


		}

	}
	
	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -1000.0f });
	//std::shared_ptr<NeroLoading> NeroHPUIPtr = CreateActor<NeroLoading>();
	//std::shared_ptr<NeroItemGlass> NeroItemGlassPtr = CreateActor<NeroItemGlass>();
//	std::shared_ptr<C_ChoiceBG> C_ChoiceBGPtr = CreateActor<C_ChoiceBG>();
	//std::shared_ptr<NeroHPUI> NeroHPUIPtr = CreateActor<NeroHPUI>();
	//std::shared_ptr<BossHpBar> BossHpBarPtr = CreateActor<BossHpBar>();
	std::shared_ptr<RankUI> RankUIPtr = CreateActor<RankUI>();

	
}

void PlayUITestLevel::Update(float _DeltaTime)
{
	BaseLevel::Update(_DeltaTime);

}

void PlayUITestLevel::LevelChangeStart()
{

	GameEngineLevel::LevelChangeStart();
}

void PlayUITestLevel::LevelChangeEnd()
{
	GameEngineLevel::LevelChangeEnd();
}
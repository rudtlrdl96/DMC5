#include "PrecompileHeader.h"
#include "FXSystem.h"
#include "EffectRenderer.h"
#include "UIEffectRenderer.h"
FXSystem::FXSystem()
{
	Off();
}

FXSystem::~FXSystem()
{
}

void FXSystem::EffectTextureLoad()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Effect");
	NewDir.Move("Mesh");
	if (nullptr == GameEngineFBXMesh::Find("Effect_Mesh_01.FBX"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}
	NewDir.MoveParent();
	NewDir.Move("Texture");
	if (nullptr == GameEngineTexture::Find("Effect_Texture_01.tga"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".tga" });
		for (GameEngineFile File : Files)
		{
			GameEngineTexture::Load(File.GetFullPath());
		}
	}

	// 이펙트 Sprite 로드
	if (nullptr == GameEngineSprite::Find("Effect_Impact.tga"))
	{
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Impact.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Impact_01.tga").GetFullPath(), 8, 4);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Muzzle_03.tga").GetFullPath(), 2, 1);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Spark_02.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Magic_01.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Fire_01.tga").GetFullPath(), 8, 4);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Fire_02.tga").GetFullPath(), 8, 4);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Fire_03.tga").GetFullPath(), 8, 4);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Fire_04.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Fire_05.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Explosion_01.tga").GetFullPath(), 8, 8);
		GameEngineSprite::LoadSheet(NewDir.GetPlusFileName("Effect_Explosion_02.tga").GetFullPath(), 8, 8);
	}
	if (nullptr == GameEngineTexture::Find("DistortionSample_00.jpg"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Texture", "DistortionTexture"
			});

		GameEngineTexture::Load(Path + "\\DistortionSample_00.jpg");
		GameEngineTexture::Load(Path + "\\DistortionSample_01.jpg");
		GameEngineTexture::Load(Path + "\\DistortionSample_02.jpg");
	}

}

void FXSystem::SetFX(std::shared_ptr<FXData> _FX)
{
	if (nullptr == _FX)
	{
		MsgAssert("존재하지 않는 FX입니다");
		return;
	}
	CurFX = _FX;
	StartFrame = 0;
	EndFrame = static_cast<int>(CurFX->GetFrameData().size()) - 1;
	FXSetting();
}

void FXSystem::SetFX(const std::string_view& _Name)
{
	SetFX(FXData::Find(_Name));
}

void FXSystem::CreateFX(std::shared_ptr<class FXData> _FX)
{
	std::vector<FXUnitData>& UnitDatas = _FX->GetUnitDatas();
	int SpriteIndex = 0;
	for (int i = 0; i < UnitDatas.size(); i++)
	{
		if (UnitDatas[i].MeshName != "")
		{
			if (false == FXRenders.contains(UnitDatas[i].MeshName))
			{
				// 해당 매쉬가 존재하지 않는경우
				if (true == IsUI)
				{
					FXRenders[UnitDatas[i].MeshName] = GetActor()->CreateComponent<UIEffectRenderer>();
				}
				else
				{
					FXRenders[UnitDatas[i].MeshName] = GetActor()->CreateComponent<EffectRenderer>();
				}
				FXRenders[UnitDatas[i].MeshName]->GetTransform()->SetParent(GetTransform());
				FXRenders[UnitDatas[i].MeshName]->SetFBXMesh(UnitDatas[i].MeshName, "Effect_2D");
			}
		}
		else
		{
			std::string Key = "Sprite" + std::to_string(SpriteIndex++);
			if (false == FXRenders.contains(Key))
			{
				// 렌더러가 부족한 경우

				if (true == IsUI)
				{
					FXRenders[Key] = GetActor()->CreateComponent<UIEffectRenderer>();
				}
				else
				{
					FXRenders[Key] = GetActor()->CreateComponent<EffectRenderer>();
				}
				FXRenders[Key]->GetTransform()->SetParent(GetTransform());
				FXRenders[Key]->RectInit("Effect_2D");
				FXRenders[Key]->LockRotation();
				FXRenders[Key]->GetTransform()->SetWorldRotation(float4::ZERO);
			}

			if (UnitDatas[i].AnimationName != "" && nullptr == FXRenders[Key]->FindAnimation(UnitDatas[i].AnimationName))
			{
				FXRenders[Key]->CreateAnimation({ .AnimationName = UnitDatas[i].AnimationName, .SpriteName = UnitDatas[i].AnimationName, .FrameInter = 0.0166f, .Loop = false });
			}
		}
	}
}

void FXSystem::ChangeFX(const std::string_view& _Name)
{
	ChangeFX(FXData::Find(_Name));
}

void FXSystem::ChangeFX(std::shared_ptr<class FXData> _FX)
{
	if (nullptr == _FX)
	{
		MsgAssert("존재하지 않는 FX입니다");
		return;
	}

	CurFX = _FX;
	StartFrame = 0;
	EndFrame = static_cast<int>(CurFX->GetFrameData().size()) - 1;

	RenderOff();
	std::vector<FXUnitData>& UnitDatas = CurFX->GetUnitDatas();
	int SpriteIndex = 0;
	for (int i = 0; i < UnitDatas.size(); i++)
	{
		if (UnitDatas[i].MeshName != "")
		{
			FXRenders[UnitDatas[i].MeshName]->SetTexture("DiffuseTexture", UnitDatas[i].TextureName);
			FXRenders[UnitDatas[i].MeshName]->Off();

			if (UnitDatas[i].CutX == 1)
			{
				FXRenders[UnitDatas[i].MeshName]->SetDistortionTexture("DistortionSample_00.jpg");
			}
			else
			{
				FXRenders[UnitDatas[i].MeshName]->UnSetDistortionTexture();
			}
		}
		else
		{
			std::string Key = "Sprite" + std::to_string(SpriteIndex++);

			if (UnitDatas[i].AnimationName == "")
			{
				// 애니메이션이 아닌 경우
				FXRenders[Key]->SetTexture("DiffuseTexture", UnitDatas[i].TextureName);
				FXRenders[Key]->AnimationClear();
			}
			else
			{
				FXRenders[Key]->ChangeAnimation(UnitDatas[i].AnimationName);
			}
			FXRenders[Key]->Off();

		}
	}
}

void FXSystem::Play()
{
	On();
	Pause = false;
	CurFrame = 0;
	CurFrameTime = 0;
}

void FXSystem::Start()
{
}

void FXSystem::Update(float _DeltaTime)
{
	_DeltaTime *= TimeScale;

	if (false == Pause)
	{
		// 프레임 DeltaTime 처리
		CurFrameTime += _DeltaTime;

		while (CurFrameTime >= Inter)
		{
			CurFrameTime -= Inter;
			++CurFrame;

			if (EndFrame < CurFrame)
			{
				if (true == Loop)
				{
					CurFrame = StartFrame;
				}
				else
				{
					CurFrame = EndFrame;
					Pause = true;
				}
			}
		}
	}

	int NextFrame = CurFrame + 1;

	if (EndFrame < NextFrame)
	{
		NextFrame = EndFrame;
	}

	if (nullptr == CurFX)
	{
		MsgAssert("FX데이터를 입력하지 않은 FXSystem입니다.");
		return;
	}

	std::vector<std::map<std::string, FXKeyFrame>>& FrameData = CurFX->GetFrameData();
	std::map<std::string, FXKeyFrame>& CurFrameData = FrameData[CurFrame];
	std::map<std::string, FXKeyFrame>& NextFrameData = FrameData[NextFrame];

	std::map<std::string, FXKeyFrame>::iterator StartIter = CurFrameData.begin();
	std::map<std::string, FXKeyFrame>::iterator EndIter = CurFrameData.end();

	float CurRatio = CurFrameTime / Inter;
	for (; StartIter != EndIter; StartIter++)
	{
		std::string Name = StartIter->first;
		std::shared_ptr<EffectRenderer> CurRender = FXRenders[Name];
		FXKeyFrame& CurKeyFrame = CurFrameData[Name];
		FXKeyFrame& NextKeyFrame = NextFrameData[Name];

		if (false == CurKeyFrame.IsVaild || false == NextKeyFrame.IsVaild)
		{
			continue;
		}

		if (true == CurFrameData[Name].IsUpdate)
		{
			CurRender->On();
		}
		else
		{
			CurRender->Off();
			continue;
		}
		CurRender->GetTransform()->SetLocalPosition(float4::Lerp(CurKeyFrame.Position, NextKeyFrame.Position, CurRatio));
		if (false == CurRender->IsLockRotation())
		{
			CurRender->GetTransform()->SetLocalRotation(float4::SLerpQuaternion(CurKeyFrame.Rotation, NextKeyFrame.Rotation, CurRatio));
		}
		CurRender->GetTransform()->SetLocalScale(float4::Lerp(CurKeyFrame.Scale, NextKeyFrame.Scale, CurRatio));

		CurRender->EffectOption = EffectData::Lerp(CurKeyFrame.EffectOption, NextKeyFrame.EffectOption, CurRatio);
	}
}

void FXSystem::FXSetting()
{
	std::vector<FXUnitData>& UnitDatas = CurFX->GetUnitDatas();
	int SpriteIndex = 0;
	for (int i = 0; i < UnitDatas.size(); i++)
	{
		if (UnitDatas[i].MeshName != "")
		{
			if (false == FXRenders.contains(UnitDatas[i].MeshName))
			{
				// 해당 매쉬가 존재하지 않는경우
				FXRenders[UnitDatas[i].MeshName] = GetActor()->CreateComponent<EffectRenderer>();
				FXRenders[UnitDatas[i].MeshName]->GetTransform()->SetParent(GetTransform());
				FXRenders[UnitDatas[i].MeshName]->SetFBXMesh(UnitDatas[i].MeshName, "Effect_2D");
			}
			FXRenders[UnitDatas[i].MeshName]->SetTexture("DiffuseTexture", UnitDatas[i].TextureName);
			FXRenders[UnitDatas[i].MeshName]->Off();

			if (UnitDatas[i].CutX == 1)
			{
				FXRenders[UnitDatas[i].MeshName]->SetDistortionTexture("DistortionSample_00.jpg");
			}
			else
			{
				FXRenders[UnitDatas[i].MeshName]->UnSetDistortionTexture();
			}
		}
		else
		{
			std::string Key = "Sprite" + std::to_string(SpriteIndex++);
			if (false == FXRenders.contains(Key))
			{
				// 렌더러가 부족한 경우
				FXRenders[Key] = GetActor()->CreateComponent<EffectRenderer>();
				FXRenders[Key]->GetTransform()->SetParent(GetTransform());
				FXRenders[Key]->RectInit("Effect_2D");
				FXRenders[Key]->LockRotation();
				FXRenders[Key]->GetTransform()->SetWorldRotation(float4::ZERO);
			}
			if (UnitDatas[i].AnimationName == "")
			{
				// 애니메이션이 아닌 경우
				FXRenders[Key]->SetTexture("DiffuseTexture", UnitDatas[i].TextureName);
			}
			else
			{
				if (nullptr == FXRenders[Key]->FindAnimation(UnitDatas[i].AnimationName))
				{
					// 애니메이션이 없는 경우
					FXRenders[Key]->CreateAnimation({ .AnimationName = UnitDatas[i].AnimationName, .SpriteName = UnitDatas[i].AnimationName, .FrameInter = 0.0166f, .Loop = false });
				}
				FXRenders[Key]->ChangeAnimation(UnitDatas[i].AnimationName);
			}
			FXRenders[Key]->Off();
		}
	}
}

void FXSystem::RenderOff()
{
	for (std::pair<std::string, std::shared_ptr<class EffectRenderer>> _Pair : FXRenders)
	{
		_Pair.second->Off();
	}
}


#include "PrecompileHeader.h"
#include "FXSystem.h"
#include "EffectRenderer.h"

FXSystem::FXSystem()
{
	Off();
}

FXSystem::~FXSystem()
{
}

void FXSystem::SetFX(std::shared_ptr<FXData> _FX)
{
	if (nullptr == _FX)
	{
		MsgAssert("�������� �ʴ� FX�Դϴ�");
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
				// �ش� �Ž��� �������� �ʴ°��
				FXRenders[UnitDatas[i].MeshName] = GetActor()->CreateComponent<EffectRenderer>();
				FXRenders[UnitDatas[i].MeshName]->GetTransform()->SetParent(GetTransform());
				FXRenders[UnitDatas[i].MeshName]->SetFBXMesh(UnitDatas[i].MeshName, "Effect_2D");
			}
		}
		else
		{
			std::string Key = "Sprite" + std::to_string(SpriteIndex++);
			if (false == FXRenders.contains(Key))
			{
				// �������� ������ ���
				FXRenders[Key] = GetActor()->CreateComponent<EffectRenderer>();
				FXRenders[Key]->GetTransform()->SetParent(GetTransform());
				FXRenders[Key]->RectInit("Effect_2D");
				FXRenders[Key]->LockRotation();
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
		MsgAssert("�������� �ʴ� FX�Դϴ�");
		return;
	}

	On();
	Pause = false;
	CurFrame = 0;
	CurFrameTime = 0;

	CurFX = _FX;
	StartFrame = 0;
	EndFrame = static_cast<int>(CurFX->GetFrameData().size()) - 1;

	std::vector<FXUnitData>& UnitDatas = CurFX->GetUnitDatas();
	int SpriteIndex = 0;
	for (int i = 0; i < UnitDatas.size(); i++)
	{
		if (UnitDatas[i].MeshName != "")
		{
			FXRenders[UnitDatas[i].MeshName]->SetTexture("DiffuseTex", UnitDatas[i].TextureName);
			FXRenders[UnitDatas[i].MeshName]->Off();
		}
		else
		{
			std::string Key = "Sprite" + std::to_string(SpriteIndex++);

			if (UnitDatas[i].AnimData.AnimationName == "")
			{
				// �ִϸ��̼��� �ƴ� ���
				FXRenders[Key]->SetTexture("DiffuseTex", UnitDatas[i].TextureName);
			}
			else
			{
				FXRenders[Key]->ChangeAnimation(UnitDatas[i].AnimData.AnimationName);
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
		// ������ DeltaTime ó��
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
		MsgAssert("FX�����͸� �Է����� ���� FXSystem�Դϴ�.");
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
		CurRender->GetTransform()->SetLocalRotation(float4::SLerpQuaternion(CurKeyFrame.Rotation, NextKeyFrame.Rotation, CurRatio));
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
				// �ش� �Ž��� �������� �ʴ°��
				FXRenders[UnitDatas[i].MeshName] = GetActor()->CreateComponent<EffectRenderer>();
				FXRenders[UnitDatas[i].MeshName]->GetTransform()->SetParent(GetTransform());
				FXRenders[UnitDatas[i].MeshName]->SetFBXMesh(UnitDatas[i].MeshName, "Effect_2D");
			}
			FXRenders[UnitDatas[i].MeshName]->SetTexture("DiffuseTex", UnitDatas[i].TextureName);
			FXRenders[UnitDatas[i].MeshName]->Off();
		}
		else
		{
			std::string Key = "Sprite" + std::to_string(SpriteIndex++);
			if (false == FXRenders.contains(Key))
			{
				// �������� ������ ���
				FXRenders[Key] = GetActor()->CreateComponent<EffectRenderer>();
				FXRenders[Key]->GetTransform()->SetParent(GetTransform());
				FXRenders[Key]->RectInit("Effect_2D");
				FXRenders[Key]->LockRotation();
			}
			if (UnitDatas[i].AnimData.AnimationName == "")
			{
				// �ִϸ��̼��� �ƴ� ���
				FXRenders[Key]->SetTexture("DiffuseTex", UnitDatas[i].TextureName);
			}
			else
			{
				if (nullptr == FXRenders[Key]->FindAnimation(UnitDatas[i].AnimData.AnimationName))
				{
					// �ִϸ��̼��� ���� ���
					FXRenders[Key]->CreateAnimation(UnitDatas[i].AnimData);
				}
				FXRenders[Key]->ChangeAnimation(UnitDatas[i].AnimData.AnimationName);
			}
			FXRenders[Key]->Off();
		}
	}
}


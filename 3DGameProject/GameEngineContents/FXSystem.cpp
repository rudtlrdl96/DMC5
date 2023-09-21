#include "PrecompileHeader.h"
#include "FXSystem.h"
#include "FXData.h"
#include "EffectRenderer.h"

FXSystem::FXSystem()
{
}

FXSystem::~FXSystem()
{
}

void FXSystem::SetFX(const std::string_view& _Name)
{
	std::shared_ptr<FXData> _FX = FXData::Find(_Name);
	if (nullptr == _FX)
	{
		MsgAssert("�ش� FX�� �ε���� �ʾҽ��ϴ�." + std::string(_Name));
		return;
	}
	CurFX = _FX;
	FXSetting();
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

	std::vector<std::vector<FXKeyFrame>>& FrameData = CurFX->GetFrameData();
	std::vector<FXKeyFrame>& CurFrameData = FrameData[CurFrame];
	std::vector<FXKeyFrame>& NextFrameData = FrameData[NextFrame];

	float CurRatio = CurFrameTime / Inter;
	for (int i = 0; i < CurFrameData.size(); i++)
	{
		if (true == CurFrameData[i].IsUpdate)
		{
			FXRenders[i]->On();
		}
		else
		{
			FXRenders[i]->Off();
			continue;
		}
		FXRenders[i]->GetTransform()->SetLocalPosition(float4::Lerp(CurFrameData[i].Position, NextFrameData[i].Position, CurRatio));
		FXRenders[i]->GetTransform()->SetLocalRotation(float4::SLerpQuaternion(CurFrameData[i].Rotation, NextFrameData[i].Rotation, CurRatio));
		FXRenders[i]->GetTransform()->SetLocalScale(float4::Lerp(CurFrameData[i].Scale, NextFrameData[i].Scale, CurRatio));

		FXRenders[i]->EffectOption.ClipEndX = std::lerp(CurFrameData[i].EffectOption.ClipEndX, NextFrameData[i].EffectOption.ClipEndX, CurRatio);
		FXRenders[i]->EffectOption.ClipEndY = std::lerp(CurFrameData[i].EffectOption.ClipEndY, NextFrameData[i].EffectOption.ClipEndY, CurRatio);
		FXRenders[i]->EffectOption.ClipStartX = std::lerp(CurFrameData[i].EffectOption.ClipStartX, NextFrameData[i].EffectOption.ClipStartX, CurRatio);
		FXRenders[i]->EffectOption.ClipStartY = std::lerp(CurFrameData[i].EffectOption.ClipStartY, NextFrameData[i].EffectOption.ClipStartY, CurRatio);
	}
}

void FXSystem::FXSetting()
{
	std::vector<FXUnitData>& UnitDatas = CurFX->GetUnitDatas();
	for (int i = 0; i < UnitDatas.size(); i++)
	{
		if (i <= FXRenders.size())
		{
			MsgAssert("FX�� �̸� ������ Renderer�� ������ �ʰ��߽��ϴ�");
			return;
		}

		if (UnitDatas[i].MeshName != "")
		{
			// �޽��� ���
			FXRenders[i]->SetFBXMesh(UnitDatas[i].MeshName, "Effect_2D");
			FXRenders[i]->SetTexture("DiffuseTexture", UnitDatas[i].TextureName);
		}
		else
		{
			// Sprite�� ���
			if (UnitDatas[i].AnimData.AnimationName == "")
			{
				// �ִϸ��̼��� �ƴ� ���
				FXRenders[i]->SetSprite(UnitDatas[i].AnimData.SpriteName);
			}
			if (nullptr == FXRenders[i]->FindAnimation(UnitDatas[i].AnimData.AnimationName))
			{
				FXRenders[i]->CreateAnimation(UnitDatas[i].AnimData);
			}
			FXRenders[i]->ChangeAnimation(UnitDatas[i].AnimData.AnimationName);
		}
		FXRenders[i]->Off();
	}
}


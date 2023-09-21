#include "PrecompileHeader.h"
#include "FXSystem.h"
#include "FXData.h"
FXSystem::FXSystem()
{
}

FXSystem::~FXSystem()
{
}

void FXSystem::SetFX(const std::string_view& _Name)
{
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
		//if (true == Loop)
		//{
		//	NextFrame = StartFrame;
		//}
		//else
		//{
		//	NextFrame = EndFrame - 1;
		//}
	}

	if (nullptr == CurFX)
	{
		MsgAssert("FX데이터를 입력하지 않은 FXSystem입니다.");
		return;
	}

	std::vector<std::vector<FXKeyFrame>>& FrameData = CurFX->GetFrameData();
	std::vector<FXKeyFrame>& CurFrameData = FrameData[CurFrame];
	std::vector<FXKeyFrame>& NextFrameData = FrameData[NextFrame];
	
	for (int i = 0; i < CurFrameData.size(); i++)
	{
		//Renderers[i]->

		//CurFrameData[i].Position
	}
}


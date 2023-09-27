#include "PrecompileHeader.h"
#include "FXData.h"

FXData::FXData() 
{
}

FXData::~FXData() 
{
}

std::shared_ptr<FXData> FXData::CreateData(std::vector<FXUnitData>& _UnitDatas, std::map<int, std::map<int, FXKeyFrame>>& _Data)
{
	std::shared_ptr<FXData> NewData = CreateUnNamed();
	NewData->UnitDatas = _UnitDatas;

	// 렌더러 이름
	std::vector<std::string> Names;
	Names.resize(_UnitDatas.size());
	int SpriteIndex = 0;
	for (int i = 0; i < Names.size(); i++)
	{
		if (_UnitDatas[i].MeshName != "")
		{
			Names[i] = _UnitDatas[i].MeshName;
		}
		else
		{
			Names[i] = "Sprite" + std::to_string(SpriteIndex++);
		}
	}
	// 프레임 데이터 복사
	NewData->FrameData.clear();
	NewData->FrameData.resize((--_Data.end())->first + 1);
	for (int i = 0; i < NewData->FrameData.size(); i++)
	{						  
		if (false == _Data.contains(i)) { continue; }

		for (int j = 0; j < _UnitDatas.size(); j++)
		{
			if (false == _Data[i].contains(j))
			{
				continue;
			}
			NewData->FrameData[i][Names[j]] = _Data[i][j];
			NewData->FrameData[i][Names[j]].IsVaild = true;
		}
	}


	// 프레임 데이터 보간
	for (int i = 0; i < NewData->FrameData.size(); i++)
	{
		std::map<std::string, FXKeyFrame>::iterator StartIter = NewData->FrameData[i].begin();
		std::map<std::string, FXKeyFrame>::iterator EndIter = NewData->FrameData[i].end();

		for (; StartIter != EndIter; StartIter++)
		{
			std::string Name = StartIter->first;
			FXKeyFrame& CurFrame = StartIter->second;
			// 현재 프레임이 유효한 프레임이 아니면 continue;
			if (CurFrame.IsVaild == false) { continue; }
			
			FXKeyFrame NextFrame;
			for (int j = i + 1; j < NewData->FrameData.size(); j++)
			{
				// 현재 유닛의 유효한 다음 프레임을 찾는다. 
				if (NewData->FrameData[j][Name].IsVaild == false)
				{
					continue;
				}
				NextFrame = NewData->FrameData[j][Name];

				// CurFrame i
				// NextFrame j
				// 그 사이 모든 프레임에 내용을 보간하여 채운다
				for (int f = i + 1; f < j; f++)
				{
					float Ratio = (float)(f - i) / (j - i);
					NewData->FrameData[f][Name] = FXKeyFrame::Lerp(CurFrame, NextFrame, Ratio);
					NewData->FrameData[f][Name].IsVaild = true;
				}
				break;
			}

		}
	}


	return NewData;
}

void FXData::WriteFrameData(GameEngineSerializer& _File)
{
	unsigned int Size = static_cast<unsigned int>(FrameData.size());
	_File << Size;

	if (Size <= 0)
	{
		return;
	}

	for (size_t i = 0; i < FrameData.size(); i++)
	{
		_File << FrameData[i];
	}

}


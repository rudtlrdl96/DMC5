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

	// ������ �̸�
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
	// ������ ������ ����
	NewData->FrameData.clear();
	NewData->FrameData.resize((--_Data.end())->first + 1);
	for (int i = 0; i < NewData->FrameData.size(); i++)
	{						  
		if (false == _Data.contains(i)) { continue; }

		std::map<int, FXKeyFrame>::iterator StartIter = _Data[i].begin();
		std::map<int, FXKeyFrame>::iterator EndIter = _Data[i].end();

		for (int j = 0; j < _Data[i].size(); j++)
		{
			NewData->FrameData[i][Names[j]] = _Data[i][j];
			NewData->FrameData[i][Names[j]].IsVaild = true;
		}
	}


	// ������ ������ ����
	for (int i = 0; i < NewData->FrameData.size(); i++)
	{
		std::map<std::string, FXKeyFrame>::iterator StartIter = NewData->FrameData[i].begin();
		std::map<std::string, FXKeyFrame>::iterator EndIter = NewData->FrameData[i].end();

		for (; StartIter != EndIter; StartIter++)
		{
			std::string Name = StartIter->first;
			FXKeyFrame& CurFrame = StartIter->second;
			// ���� �������� ��ȿ�� �������� �ƴϸ� continue;
			if (CurFrame.IsVaild == false) { continue; }
			
			FXKeyFrame NextFrame;
			for (int j = i + 1; j < NewData->FrameData.size(); j++)
			{
				// ���� ������ ��ȿ�� ���� �������� ã�´�. 
				if (NewData->FrameData[j][Name].IsVaild == false)
				{
					continue;
				}
				NextFrame = NewData->FrameData[j][Name];

				// CurFrame i
				// NextFrame j
				// �� ���� ��� �����ӿ� ������ �����Ͽ� ä���
				for (int f = i + 1; f < j; f++)
				{
					float Ratio = (float)(f - i) / (j - i);
					NewData->FrameData[f][Name] = FXKeyFrame::Lerp(CurFrame, NextFrame, Ratio);
					NewData->FrameData[f][Name].IsVaild = true;
				}
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


#include "PrecompileHeader.h"
#include "AnimationEvent.h"
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>

AnimationEvent::~AnimationEvent()
{
}

void AnimationEvent::EventsWrite(GameEngineSerializer& _File)
{
	unsigned int Size = static_cast<unsigned int>(Events.size());
	_File << Size;

	if (Size <= 0)
	{
		return;
	}

	for (std::pair<size_t, std::vector<EventData>> Pair : Events)
	{
		_File << Pair.first;
		std::vector<EventData>& Vector = Pair.second;
		unsigned int EventSize = static_cast<unsigned int>(Vector.size());
		_File << EventSize;

		if (EventSize <= 0)
		{
			continue;
		}

		for (size_t i = 0; i < EventSize; i++)
		{
			_File << Vector[i];
		}
	}
}
void AnimationEvent::EventsRead(GameEngineSerializer& _File)
{
	unsigned int Size = 0;
	_File >> Size;

	if (Size <= 0)
	{
		return;
	}

	for (unsigned int i = 0; i < Size; ++i)
	{
		std::pair<size_t, std::vector<EventData>> Pair;
		_File >> Pair.first;

		unsigned int EventSize = 0;
		_File >> EventSize;

		if (EventSize <= 0)
		{
			continue;
		}

		Pair.second.resize(EventSize);

		for (unsigned int i = 0; i < EventSize; i++)
		{
			_File >> Pair.second[i];
		}

		Events[Pair.first] = Pair.second;
	}
}

void AnimationEvent::LoadAll(const AnimLoadParameter& _Parameter)
{
	GameEngineDirectory Dir;
	Dir.SetPath(_Parameter.Dir);

	// Dir ��ο� ��� .animation ������ �о� �ִϸ��̼� �̺�Ʈ�� �����մϴ�
	std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".FBX" });
	std::vector<GameEngineFile> AnimFiles = Dir.GetAllFile({ ".ANIMATION" });

	AnimationEvent Event;
	for (size_t EventIndex = 0; EventIndex < AnimFiles.size(); EventIndex++)
	{
		// ���� �ε�
		GameEngineSerializer Ser;
		AnimFiles[EventIndex].LoadBin(Ser);
		Event.Read(Ser);

		// �ִϸ��̼� �ε�
		for (size_t j = 0; j < FBXFiles.size(); j++)
		{
			std::string FileName = GameEngineString::ToUpper(FBXFiles[j].GetFileName());
			if (FileName != GameEngineString::ToUpper(Event.AnimationName + ".FBX"))
			{
				continue;
			}
			if (nullptr != GameEngineFBXAnimation::Find(FileName))
			{
				continue;
			}
			GameEngineFBXAnimation::Load(FBXFiles[j].GetFullPath());
		}
		// �ִϸ��̼� ����
		_Parameter.Renderer->CreateFBXAnimation(Event.AnimationName, Event.AnimationName + ".FBX");
		// .animation ���� ����
		std::shared_ptr<GameEngineFBXAnimationInfo> AnimInfo = _Parameter.Renderer->GetAnimation(Event.AnimationName);
		AnimInfo->TimeScale = Event.Speed; // �ӵ� ����

		// �ִϸ��̼� �̺�Ʈ ����
		for (int i = 0; i < Event.Events.size(); i++)
		{
			for (int j = 0; j < Event.Events[i].size(); j++)
			{
				EventData& Data = Event.Events[i][j];

				if (Data.Type == EventType::ObjectUpdate)
				{
					// ������Ʈ ������Ʈ ���� ����
					if (_Parameter.Objects.size() < Data.Index || _Parameter.Objects[Data.Index] == nullptr)
					{
						// Index���� �Ķ���Ϳ� ���� ������Ʈ ������ ������ ���� Ŭ ��
						// Index���� ������Ʈ�� nullptr�� �� continue
						continue;
					}
					// OnOff ����
					if (Data.IsUpdate == true)
					{
						AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineObject::On, _Parameter.Objects[Data.Index]));
					}
					else
					{
						AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineObject::Off, _Parameter.Objects[Data.Index]));
					}
					// Transform ����
					AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineTransform::SetLocalPosition, _Parameter.Objects[Data.Index]->GetTransform(), Data.Position));
					AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineTransform::SetLocalRotation, _Parameter.Objects[Data.Index]->GetTransform(), Data.Rotation));
					AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineTransform::SetLocalScale, _Parameter.Objects[Data.Index]->GetTransform(), Data.Scale));
				}
				else if (Data.Type == EventType::CallBackVoid)
				{
					// void �ݹ��Լ�
					if (_Parameter.CallBacks_void[Data.Index] == nullptr)
					{
						continue;
					}
					AnimInfo->AnimationEvent[i].push_back(_Parameter.CallBacks_void[Data.Index]);
				}
				else if (Data.Type == EventType::CallBackInt)
				{
					// int �ݹ��Լ�
					if (_Parameter.CallBacks_void[Data.Index] == nullptr)
					{
						continue;
					}
					AnimInfo->AnimationEvent[i].push_back(std::bind(_Parameter.CallBacks_int[Data.Index], Data.IntValue));
				}
				else if (Data.Type == EventType::CallBackFloat)
				{
					// float �ݹ��Լ�
					if (_Parameter.CallBacks_void[Data.Index] == nullptr)
					{
						continue;
					}
					AnimInfo->AnimationEvent[i].push_back(std::bind(_Parameter.CallBacks_float[Data.Index], Data.FloatValue));
				}
			}
		}
	}

}

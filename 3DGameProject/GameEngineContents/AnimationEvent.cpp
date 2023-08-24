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
	{
		std::vector<GameEngineFile> Files = Dir.GetAllFile({ ".FBX" });
		for (size_t i = 0; i < Files.size(); i++)
		{
			if (nullptr != GameEngineFBXAnimation::Find(Files[i].GetFileName()))
			{
				continue;
			}
			GameEngineFBXAnimation::Load(Files[i].GetFullPath());
		}
	}

	{
		std::vector<GameEngineFile> Files = Dir.GetAllFile({ ".ANIMATION" });
		AnimationEvent Event;
		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineSerializer Ser;
			Files[i].LoadBin(Ser);
			Event.Read(Ser);

			_Parameter.Renderer->CreateFBXAnimation(Event.AnimationName, Event.AnimationName + ".FBX");
			std::shared_ptr<GameEngineFBXAnimationInfo> AnimInfo = _Parameter.Renderer->GetAnimation(Event.AnimationName);
			AnimInfo->TimeScale = Event.Speed;

			for (int i = 0; i < Event.Events.size(); i++)
			{
				for (int j = 0; j < Event.Events[i].size(); j++)
				{
					EventData& Data = Event.Events[i][j];
					if (Data.Type == EventType::ObjectUpdate)
					{
						if (_Parameter.Objects[Data.Index] == nullptr)
						{
							continue;
						}
						if (Data.IsUpdate == true)
						{
							AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineObject::On, _Parameter.Objects[Data.Index]));
						}
						else
						{
							AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineObject::Off, _Parameter.Objects[Data.Index]));
						}
						AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineTransform::SetLocalPosition, _Parameter.Objects[Data.Index]->GetTransform(), Data.Position));
						AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineTransform::SetLocalRotation, _Parameter.Objects[Data.Index]->GetTransform(), Data.Rotation));
						AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineTransform::SetLocalScale, _Parameter.Objects[Data.Index]->GetTransform(), Data.Scale));
					}
					else if (Data.Type == EventType::CallBackVoid)
					{
						if (_Parameter.CallBacks_void[Data.Index] == nullptr)
						{
							continue;
						}
						AnimInfo->AnimationEvent[i].push_back(_Parameter.CallBacks_void[Data.Index]);
					}	
					else if (Data.Type == EventType::CallBackInt)
					{
						if (_Parameter.CallBacks_void[Data.Index] == nullptr)
						{
							continue;
						}
						AnimInfo->AnimationEvent[i].push_back(std::bind(_Parameter.CallBacks_int[Data.Index], static_cast<int>(Data.Position.x)));
					}
					else if (Data.Type == EventType::CallBackFloat)
					{
						if (_Parameter.CallBacks_void[Data.Index] == nullptr)
						{
							continue;
						}
						AnimInfo->AnimationEvent[i].push_back(std::bind(_Parameter.CallBacks_float[Data.Index], Data.Position.x));
					}
				}
			}
			AnimInfo->AnimationEvent;

			_Parameter.Renderer->ChangeAnimation(Event.AnimationName);

			
		}


	}

}

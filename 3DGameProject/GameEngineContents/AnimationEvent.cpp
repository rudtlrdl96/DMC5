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

	// Dir 경로에 모든 .animation 파일을 읽어 애니메이션 이벤트를 구현합니다
	std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".FBX" });
	std::vector<GameEngineFile> AnimFiles = Dir.GetAllFile({ ".ANIMATION" });

	for (size_t EventIndex = 0; EventIndex < AnimFiles.size(); EventIndex++)
	{   
		// 파일 로드
		GameEngineSerializer Ser;
		AnimFiles[EventIndex].LoadBin(Ser);
		AnimationEvent Event;
		Event.Read(Ser);

		for (GameEngineFile _File : FBXFiles)
		{
			std::string FileName = GameEngineString::ToUpper(_File.GetFileName());
			if (FileName != GameEngineString::ToUpper(Event.AnimationName + ".FBX"))
			{
				continue;
			}
			if (nullptr != GameEngineFBXAnimation::Find(FileName))
			{
				continue;
			}
			GameEngineFBXAnimation::Load(_File.GetFullPath());
			break;
		}
		// 애니메이션 생성
		_Parameter.Renderer->CreateFBXAnimation(Event.AnimationName, Event.AnimationName + ".FBX", {.Inter = 0.01666f,});
		// .animation 내용 적용
		std::shared_ptr<GameEngineFBXAnimationInfo> AnimInfo = _Parameter.Renderer->GetAnimation(Event.AnimationName);
		AnimInfo->Loop = Event.Loop; // 루프 적용
		AnimInfo->TimeScale = Event.Speed; // 속도 적용
		AnimInfo->BlendIn = Event.BlendIn; // 블렌드 적용
		AnimInfo->BlendOut = Event.BlendOut;

		// 애니메이션 이벤트 구현
		for (int i = 0; i < Event.Events.size(); i++)
		{
			for (int j = 0; j < Event.Events[i].size(); j++)
			{
				EventData& Data = Event.Events[i][j];

				if (Data.Type == EventType::ObjectUpdate)
				{
					// 오브젝트 업데이트 유형 적용
					if (_Parameter.Objects.size() <= Data.Index || _Parameter.Objects[Data.Index] == nullptr)
					{
						// Index값이 파라미터에 넣은 오브젝트 벡터의 사이즈 보다 클 시
						// Index값의 오브젝트가 nullptr일 시 continue
						continue;
					}
					// OnOff 적용
					if (Data.IsUpdate == true)
					{
						AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineObject::On, _Parameter.Objects[Data.Index]));
					}
					else
					{
						AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineObject::Off, _Parameter.Objects[Data.Index]));
					}
					// Transform 적용
					AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineTransform::SetLocalPosition, _Parameter.Objects[Data.Index]->GetTransform(), Data.Position + _Parameter.RendererLocalPos));
					AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineTransform::SetLocalRotation, _Parameter.Objects[Data.Index]->GetTransform(), Data.Rotation));
					AnimInfo->AnimationEvent[i].push_back(std::bind(&GameEngineTransform::SetLocalScale, _Parameter.Objects[Data.Index]->GetTransform(), Data.Scale));
				}
				else if (Data.Type == EventType::CallBackVoid)
				{
					// void 콜백함수
					if (_Parameter.CallBacks_void.size() <= Data.Index || _Parameter.CallBacks_void[Data.Index] == nullptr)
					{
						continue;
					}
					AnimInfo->AnimationEvent[i].push_back(_Parameter.CallBacks_void[Data.Index]);
				}
				else if (Data.Type == EventType::CallBackInt)
				{
					// int 콜백함수
					if (_Parameter.CallBacks_int.size() <= Data.Index || _Parameter.CallBacks_int[Data.Index] == nullptr)
					{
						continue;
					}
					AnimInfo->AnimationEvent[i].push_back(std::bind(_Parameter.CallBacks_int[Data.Index], Data.IntValue));
				}
				else if (Data.Type == EventType::CallBackFloat)
				{
					// float 콜백함수
					if (_Parameter.CallBacks_float.size() <= Data.Index || _Parameter.CallBacks_float[Data.Index] == nullptr)
					{
						continue;
					}
					AnimInfo->AnimationEvent[i].push_back(std::bind(_Parameter.CallBacks_float[Data.Index], Data.FloatValue));
				}
				else if (Data.Type == EventType::CallBackFloat4)
				{
					// float4 콜백함수
					if (_Parameter.CallBacks_float4.size() <= Data.Index || _Parameter.CallBacks_float4[Data.Index] == nullptr)
					{
						continue;
					}
					AnimInfo->AnimationEvent[i].push_back(std::bind(_Parameter.CallBacks_float4[Data.Index], Data.Position));
				}
			}
		}
	}

}

void AnimationEvent::AnimationAllBuild(std::shared_ptr<GameEngineLevel> _Level, const std::string_view& _Dir, const std::string& MeshName)
{
	GameEngineDirectory Dir;
	Dir.SetPath(_Dir);

	// Dir 경로에 모든 .animation 파일을 읽어 애니메이션 이벤트를 구현합니다
	std::vector<GameEngineFile> FBXFiles = Dir.GetAllFile({ ".FBX" });
	std::vector<GameEngineFile> AnimFiles = Dir.GetAllFile({ ".ANIMATION" });
	std::shared_ptr<GameEngineFBXRenderer> Renderer = nullptr;
	for (GameEngineFile _File : FBXFiles)
	{
		if (_File.GetFileName() == MeshName)
		{
			GameEngineFBXMesh::Load(_File.GetFullPath());
			Renderer = _Level->CreateActor<GameEngineActor>()->CreateComponent<GameEngineFBXRenderer>();
			break;
		}
	}

	for (GameEngineFile _File : AnimFiles)
	{
		GameEngineFBXAnimation::Load(_File.GetFullPath());
		Renderer->CreateFBXAnimation(_File.GetFileName(), {});
	}
}

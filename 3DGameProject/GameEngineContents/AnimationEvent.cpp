#include "PrecompileHeader.h"
#include "AnimationEvent.h"


AnimationEvent::~AnimationEvent()
{
}

void AnimationEvent::EventsWrite(GameEngineSerializer& _File)
{
	//_File << Events[0][0];
	Events[0][0].Write(_File);
	return;
	unsigned int Size = static_cast<unsigned int>(Events.size());
	_File << Size;
	//_File.Write(Size);

	if (Size <= 0)
	{
		return;
	}

	for (std::pair<size_t, std::vector<EventData>> Pair : Events)
	{
		_File << Pair.first;
		//_File.Write(Pair.first);
		std::vector<EventData>& Vector = Pair.second;
		unsigned int Size = static_cast<unsigned int>(Vector.size());
		//_File.Write(Size);
		_File << Size;

		if (Size <= 0)
		{
			return;
		}

		for (size_t i = 0; i < Vector.size(); i++)
		{
			_File << Vector[i];
			//_File.Write(&Vector[i], sizeof(EventData));
		}
		//_File.Write(Pair.second);
		//_File.Write(Pair.second);
	}
}
void AnimationEvent::EventsRead(GameEngineSerializer& _File)
{
	EventData NewEventData;
	//_File >> NewEventData;
	NewEventData.Read(_File);
	Events[0].push_back(NewEventData);
	return;

	unsigned int Size = 0;
	_File >> Size;
	//_File.Read(Size);

	if (Size <= 0)
	{
		return;
	}

	for (unsigned int i = 0; i < Size; ++i)
	{
		std::pair<size_t, std::vector<EventData>> Pair;
		_File >> Pair.first;
		//_File.Read(Pair.first);

		unsigned int VectorSize = 0;
		_File >> VectorSize;
		//_File.Read(VectorSize);

		if (VectorSize <= 0)
		{
			return;
		}

		Pair.second.resize(VectorSize);

		for (unsigned int i = 0; i < Size; i++)
		{
			_File >> Pair.second[i];
			//_File.Read(&Pair.second[i], sizeof(EventData));
		}

		Events.insert(Pair);
	}
}

void AnimationEvent::LoadAll(const AnimLoadParameter& _Parameter)
{

}

/*

GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Mesh");
		NewDir.Move("TestMesh");
		// 테스트 메쉬 폴더는 자동으로 로드합니다
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".FBX" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineFBXMesh::Load(Files[i].GetFullPath());
		}

*/


#include "PrecompileHeader.h"
#include "LevelChangeZone.h"
#include <GameEngineCore/GameEngineCollision.h>
#include "ThreadLoadingLevel.h"

LevelChangeZone::LevelChangeZone()
{

}

LevelChangeZone::~LevelChangeZone()
{

}

void LevelChangeZone::ChangeLevel()
{
	if (ChangeLevelName == "")
	{
		MsgAssert("ChangeLevelName가 정해지지 않았습니다");
	}
	ThreadLoadingLevel::ChangeLevel(ChangeLevelName);
}

void LevelChangeZone::Start()
{
	EventZone::Start();
	Event = [this]()
		{
			LevelChangeZone::ChangeLevel();
		};
}

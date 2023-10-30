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
		MsgAssert("ChangeLevelName�� �������� �ʾҽ��ϴ�");
	}
	ThreadLoadingLevel::ChangeLevel(ChangeLevelName);
}

void LevelChangeZone::Start()
{
	EventZone::Start();
	Option = EventZoneOption::OnePlayer;
	Event = [this]()
		{
			LevelChangeZone::ChangeLevel();
		};
}

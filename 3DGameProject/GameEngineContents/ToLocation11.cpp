#include "PrecompileHeader.h"
#include "ToLocation11.h"

ToLocation11::ToLocation11()
{

}

ToLocation11::~ToLocation11()
{

}

void ToLocation11::Start()
{
	LevelChangeZone::Start();
	ChangeLevelName = "Location11_Level";
	Event = [this]()
		{
			LevelChangeZone::ChangeLevel();
		};
}

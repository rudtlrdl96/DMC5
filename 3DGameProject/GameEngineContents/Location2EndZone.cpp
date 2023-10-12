#include "PrecompileHeader.h"
#include "Location2EndZone.h"

Location2EndZone::Location2EndZone()
{

}

Location2EndZone::~Location2EndZone()
{

}

void Location2EndZone::Start()
{
	EventZone::Start();
	Event = []()
		{
			GameEngineCore::ChangeLevel("Location11_Level");
		};
}

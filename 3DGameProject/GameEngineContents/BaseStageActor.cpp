#include "PrecompileHeader.h"
#include "BaseStageActor.h"

BaseStageActor::BaseStageActor()
{

}

BaseStageActor::~BaseStageActor()
{

}

bool BaseStageActor::alphaCheck(const std::string_view& _FBXName)
{
	if(	_FBXName[0] == 'A' &&
		_FBXName[1] == 'L' &&
		_FBXName[2] == 'P' &&
		_FBXName[3] == 'H' &&
		_FBXName[4] == 'A')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BaseStageActor::waterCheck(const std::string_view& _FBXName)
{
	if (_FBXName[0] == 'W' &&
		_FBXName[1] == 'A' &&
		_FBXName[2] == 'T' &&
		_FBXName[3] == 'E' &&
		_FBXName[4] == 'R')
	{
		return true;
	}
	else
	{
		return false;
	}
}

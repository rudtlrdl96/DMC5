#include "PrecompileHeader.h"
#include "GameEngineRandom.h"
#include <chrono>


GameEngineRandom GameEngineRandom::MainRandom;

GameEngineRandom::GameEngineRandom() 
	: MtGen(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
{

	// 34789312789312789321

	// 크로노는 std에서 제공하는 시간을 재기위한 함수이다.
	
}

GameEngineRandom::~GameEngineRandom() 
{
}


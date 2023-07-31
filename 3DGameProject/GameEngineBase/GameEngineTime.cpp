#include "PrecompileHeader.h"
#include "GameEngineTime.h"

GameEngineTime GameEngineTime::GlobalTime;

GameEngineTime::GameEngineTime() 
{
	// 1초를 기준으로한 정수 단위입니다. 
	// 시작할때 1번만 정하면 된다.
	QueryPerformanceFrequency(&Second);
	QueryPerformanceCounter(&Prev);
}

GameEngineTime::~GameEngineTime() 
{
}
//
//void GameEngineTime::TimeCheckStart() 
//{
//	// 0월 0시 0분 0초
//
//	// 지금 이 함수가 실행됐을때 기준 시간부터 이 함수를 실행한 시점까지를 정수로 나타낸 숫자.
//	// 컴퓨터가 잴수있는 그때까지 잰 시간.
//	// 100이 나왔어요.
//	// QueryPerformanceCounter(&Prev);
//
//	// 1초를 200으로 표현할수 있다.
//
//		//QueryPerformanceCounter()
//
//	// 이제부터 시간을 잴 준비를 할께
//}


void GameEngineTime::Reset()
{
	QueryPerformanceCounter(&Prev);
}

float GameEngineTime::TimeCheck()
{
	// 그래서 이 함수가 실행될때까지 걸린 시간은....

	QueryPerformanceCounter(&Current);

	Tick = Current.QuadPart - Prev.QuadPart;

	// 현재시간이 나오겠죠?
	//                          200                               100                                              100
	DoubleDeltaTime = (static_cast<double>(Current.QuadPart) - static_cast<double>(Prev.QuadPart)) / static_cast<double>(Second.QuadPart);

	Prev.QuadPart = Current.QuadPart;

	floatDeltaTime = static_cast<float>(DoubleDeltaTime);

	return GetTimeScaleDeltaTime();
}

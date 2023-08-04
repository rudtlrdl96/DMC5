#include "PrecompileHeader.h"
#include "GameEngineDebug.h"
// #include <iostream>
#include <crtdbg.h>            // _CrtSetDbgFlag(), _CrtSetBreakAlloc() 함수 호출을 위한 include

namespace GameEngineDebug
{
	void LeakCheck()
	{
		// LeakCheak용 함수
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	void LeakPointBreak(int _Point)
	{
		// Leak 발생 지점을 대략적으로 확인할 수 있는 함수
		// 릭 발생 시 우측에 생성되는 오류발생 숫자를 입력하면 해당 지점으로 이동
		_CrtSetBreakAlloc(_Point);
	}
}
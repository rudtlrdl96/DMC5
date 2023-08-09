#include "PrecompileHeader.h"
#include "GameEngineDebug.h"
#include <crtdbg.h>

namespace GameEngineDebug
{
	// 릭 체크 함수 실행
	void LeakCheck()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	// 릭 발생 지점 확인 함수
	void LeakPointBreak(int _Point)
	{
		_CrtSetBreakAlloc(_Point);
	}

	void OutPutString(const std::string& _Text)
	{
		OutputDebugStringA((_Text + "\n").c_str());
	}
}
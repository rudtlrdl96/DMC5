#include "PrecompileHeader.h"
#include "GameEngineDebug.h"
// #include <iostream>
#include <crtdbg.h>

namespace GameEngineDebug
{
	void LeakCheck()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	void LeakPointBreak(int _Point)
	{
		_CrtSetBreakAlloc(_Point);
	}
}
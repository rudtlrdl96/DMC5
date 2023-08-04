#include "PrecompileHeader.h"
#include "GameEngineDebug.h"
// #include <iostream>
#include <crtdbg.h>            // _CrtSetDbgFlag(), _CrtSetBreakAlloc() �Լ� ȣ���� ���� include

namespace GameEngineDebug
{
	void LeakCheck()
	{
		// LeakCheak�� �Լ�
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	void LeakPointBreak(int _Point)
	{
		// Leak �߻� ������ �뷫������ Ȯ���� �� �ִ� �Լ�
		// �� �߻� �� ������ �����Ǵ� �����߻� ���ڸ� �Է��ϸ� �ش� �������� �̵�
		_CrtSetBreakAlloc(_Point);
	}
}
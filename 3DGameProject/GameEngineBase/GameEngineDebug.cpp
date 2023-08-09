#include "PrecompileHeader.h"
#include "GameEngineDebug.h"
#include <crtdbg.h>

namespace GameEngineDebug
{
	// �� üũ �Լ� ����
	void LeakCheck()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	// �� �߻� ���� Ȯ�� �Լ�
	void LeakPointBreak(int _Point)
	{
		_CrtSetBreakAlloc(_Point);
	}

	void OutPutString(const std::string& _Text)
	{
		OutputDebugStringA((_Text + "\n").c_str());
	}
}
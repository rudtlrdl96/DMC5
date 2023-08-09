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

	// ������ �Լ��� ����ߴµ� ������ ���ų� ����� �̻��ϴٸ� �ش� �Լ��� Ȱ���Ѵ�. ������ ���� �����ڵ�� �ؽ�Ʈ(����)�� �� �� ����
	void GetLastErrorPrint()
	{
		DWORD error = GetLastError();
		char* message = nullptr;

		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&message,
			0,
			nullptr);

		if (nullptr != message)
		{
			std::string Text = "Code : ";
			Text += std::to_string(error);
			Text += " Message : ";
			Text += message;

			MessageBoxA(nullptr, (Text).c_str(), "Error", MB_OK);
			// �ü���� �ذ� ������� �Ѵ�.
			// ������ ��������������
			// ������ �߻��ϴ� ���� ���� �����ϴ�.
			// ���� ��������� �����մϴ�.
			// ���� �ʰ��ϸ鼭 ������.
			LocalFree(message);
		}
	}

	void OutPutString(const std::string& _Text)
	{
		OutputDebugStringA((_Text + "\n").c_str());
	}
}
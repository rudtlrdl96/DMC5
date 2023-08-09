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

	// 윈도우 함수를 사용했는데 에러가 나거나 결과가 이상하다면 해당 함수를 활용한다. 에러의 실제 에러코드와 텍스트(정보)를 알 수 있음
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
			// 운영체제가 준건 해제헤야 한다.
			// 서버면 더더더더더더욱
			// 실행중 발생하는 릭이 가장 위험하다.
			// 힙이 비대해지기 시작합니다.
			// 램을 초과하면서 터진다.
			LocalFree(message);
		}
	}

	void OutPutString(const std::string& _Text)
	{
		OutputDebugStringA((_Text + "\n").c_str());
	}
}
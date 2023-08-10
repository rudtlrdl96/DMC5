#include "PrecompileHeader.h"
#include <Windows.h>
#include <GameEngineBase/GameEngineFile.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineContents/ContentsCore.h>

#pragma comment(lib, "GameEngineCore.lib")
#pragma comment(lib, "GameEngineContents.lib")

#pragma comment(lib, "zlib-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "libfbxsdk-md.lib")

#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(lib, "PhysXTask_static_64.lib")
//#pragma comment(lib, "PhysXExtensions_static_64.lib")
//#pragma comment(lib, "PhysXPvdSDK_static_64.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	GameEngineCore::Start(hInstance, ContentsCore::GameStart, ContentsCore::GameEnd);
}

// 이제 릭 남을 것인데 GECore에 Release 추가함
// 쉐어드ptr로 만들어진 것은 릭이 절대 남을 수 없게 되어잇다.
// 근데 우리의 실행 흐름은 데이터 영역 소멸 이전에 leak체크 함수를 실행하기 때문에, 오류 목록에 출력된다.
// 이게 출력은 되지만, 쉐어드ptr로 선언한 자료형이라면 없어짐.
// 
// 원래 winapi에서 했던것 처럼 소멸자에서 delete와 nullptr을 처리했지만
// 지금은 쉐어드ptr이기 때문에 릴리즈 함수 내부에서 그렇게 실시한 것.
// 
// 
// 이제 랜더러 기반에서 랜더유니트 기반으로 랜더링 형식을 바꿀 것
// 기존의 랜더러는 트랜스폼업데이트까지만 실시한다.
//  
// 카메라에 map으로 units 선언
// 
// 서버를 하기 위해서는 winso2 헤더가 필요하다.
// 근데 우리는 프리컴파일 헤더 만들었으니까, 거기에 넣으면 됨
// GetLastError는 윈도우가 에러를 내면 어떤 에러가 나는지(보통 s_ok 등 사용해서 자세히 모르지만, 이걸 사용하면) 알려주는 함수임.
// 
// 
// 
// 
// 
// 
// 
// 
// 
// 
//
//
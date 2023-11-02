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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	GameEngineCore::Start(hInstance, ContentsCore::GameStart, ContentsCore::GameEnd, {0, 0}, {1920, 1080}, true);
	//GameEngineCore::Start(hInstance, ContentsCore::GameStart, ContentsCore::GameEnd, {0, 0}, {1600 / 2, 900 / 2}, false);
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
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 0821
// 
// structuredBuffer
// 상수버퍼는 동적할당이 안댐. 용량 제한도 있음 (약 700바이트)
// 애니메이션 본이 많으면 행렬 하나에 200~300개 될 수 있음
// 때문에 상수버퍼로는 보낼 수 없음
// 
// 애니메이션 파일 안에 들어있는 것은 수학 공식이 아님
// 본 갯수 x 프레임 (전신) // 어떤 본에 몇퍼센트 영향 받는다는 정보들
// 검을 휘두르는 애니메이션이나 가만히 서있는 애니메이션이나 어차피 행렬의 갯수(본의 갯수)는 동일하기 때문에 값이 똑같음
// 이제 이걸 쉐이더로 보내야하는데, 너무 많아서 다 못보냄
// 정점에 추가로 BLENDWEIGHT BLENDINDICES 추가해놨음
// BLENDWEIGHT는 몇퍼센트 영향 받는다.
// BLENDINDICES는 어떤 본한테 영향받는다.
// 
// 4개의 배열로 되어있는데, 종종 영향받는 본이 4개가 넘는 본이 있다. 그런것도 일단 만들어놓긴 했음
// 
// ////////////////////////////////////////
// 
// 
// ////////////////////////////////////////
// 
//
//
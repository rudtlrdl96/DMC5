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
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	GameEngineCore::Start(hInstance, ContentsCore::GameStart, ContentsCore::GameEnd);
}

// MeshLoad 함수 내부
// MeshNodeCheck 에서는 실질적으로 버텍스를 로드하는게 아니고 행렬을 가진 노드만 조사하는 곳
// 그렇게 나온 결과물은, 핵심은 emplace_back << 우리가 정보를 뭐로 들고있는가 이다.
// 일단 MeshInfo로 들고 있음 (삼각형이 몇개고 이런 info들)
//
// 우리의 엔진은 애초에 버텍스 버퍼가 없으면 화면에 보일 수 없는 구조이다.
// VertexbufferCheck 함수는 이러한 버텍스를 체크해주는 함수이다.
// 전 단계에서 주목해야할 메쉬가 두개 있다고 조사한 상태(MeshNodeCheck)
// MeshInfo로 정보를 복사했다면 버텍스버퍼 인덱스버퍼를 표현해야하는데, 이때 필요한게 랜더유니트임.
// 여기서 주목할점이, 인덱스버퍼가 희한하게 되어있는데, 벡터의 벡터 형태이다.
// GEVertex 클래스 멤버 변수에는 이전에도 UV, 버텍스 위치, 이런건 썼지만 
// Normal, Binormal, Tangent는 빛계산에 이용,
// Weight랑 blendindices는 애니메이션에 활용한다.
// 
// VertexbufferCheck로 들어가면,
// 메쉬 정보만큼 사이즈를 가져오고 랜더유니트를 하나 만든다.
// MapWI는 현재 큰 의미는 없어서 넘어간다.
// LodLv == 마트료시카
// 이후 버텍스 정보를 가져온다.
// 그 버텍스 갯수만큼 리사이즈를 해서 로드를 실시한다
// 여기서 ComputeToTotalMatrix로 넘어가는데, 버텍스가 나오면 이 버텍스에 곱해주려고 하는것(?)
// 근데 곱해지든 안곱해지는 딱히 상관없다.
// 이후 Position을 전부 가져온다.
// pControlPoint 버텍스 전체의 포인터, 플룻4 배열을 우리한테 주는 것이다.
// for 문을 돌면서 controlpoint만큼 곱하고, 다시 float4로 convert 해줌
// 0, 1, 2, 3, 4 ~ 이러면서 하는 것
// 
// BoundScaleBox는 점의 최상최하최측 점들의 정보를 가져오는것
// 
// 일반적으로 FBX는 이 매쉬에 이 버텍스에 이 인덱스에 정보를 로드하려면 이 텍스쳐를 로드해야하되 라는 정보가 있어야한다.
// 근데 애초에 텍스쳐가 존재하지 않거나(애셋스토어 색만 들어있는 졸라맨 액터 같은 경우),
// 텍스쳐와 매쉬가 있는데, 매핑시켜야하는 것을 지칭해주는 정보가 없을 수 있음 << 랜더할때 텍스쳐 이거 써라 하면 나오는게 없다는 뜻
// 
// 매쉬 폴더 내부에 매쉬랑 텍스쳐가 같이 있으면 로드가 가능하도록 설정해둠
// 
// 머티리얼 갯수가 여러개일 때는 무조건 인덱스 버퍼가 여러개임
// 현재 엔진은 버텍스버퍼 하나 있을때 인덱스버퍼 하나 있는 것을 매쉬라고 부르면서 그렇게 보고 있다.
// FBX에서 하고있는 것은 MeshNode가 있고, 그 노드 안에 보통 인덱스랑 버텍스가 한쌍(일반적인 정상적인 경우)으로 존재한다.
// 두번째로는 MeshNode가 여러개, 인덱스버퍼, 버텍스버퍼가 여러개인 경우가 있다. >> 이렇게 해야 검 팔 몸 방패 얼굴 이런식으로 로드하는 것
// 마지막으로 인덱스버퍼가 여러개인 경우가 있다. 인덱스 버퍼는 점을 그리는 순서이다. 이게 하나의 버텍스 버퍼에 연속적으로 묶여있는 경우
// 
// 고로 버텍스버퍼가 두개라면 매쉬노드가 두개라고 보면 됨.
// 우리 엔진의 FBX 로드는 버텍스버퍼가 하나. 다음으로 인덱스버퍼는 벡터의 벡터임.
// 인덱스버퍼가 여러개 담겨있다면, 하나의 매쉬를 이용해서 다른 인덱스버퍼의 정보는 스킵한 상태로 하나의 부위만 표현할 수 있음
// >> 애니메이션 정보가 있는데, 얼굴만 표현하고 싶다면 인덱스 버퍼에서 얼굴에 해당하는 인덱스 버퍼만 키는 그런 것
// 
// ImportBone << FBX가 애니매이션을 가지는 경우가 있고, 본이 애니메이션을 가지는 경우가 있는데, 일단 나중에 얘기(애니메이션 할때)
//
// 아까 말했던 인덱스버퍼가 하나의 버텍스버퍼에 여러개 존재하는 경우가 subset 상황인 것이다 (캐릭터가 조각조각인 상태라는 뜻)
// SetFBXMesh에서 SetFBXMesh를 호출하게 된다.
// 여기서 랜더 유니트를 만든다.
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
// 
// 
//
//
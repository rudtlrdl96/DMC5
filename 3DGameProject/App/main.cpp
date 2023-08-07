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

// MeshLoad �Լ� ����
// MeshNodeCheck ������ ���������� ���ؽ��� �ε��ϴ°� �ƴϰ� ����� ���� ��常 �����ϴ� ��
// �׷��� ���� �������, �ٽ��� emplace_back << �츮�� ������ ���� ����ִ°� �̴�.
// �ϴ� MeshInfo�� ��� ���� (�ﰢ���� ��� �̷� info��)
//
// �츮�� ������ ���ʿ� ���ؽ� ���۰� ������ ȭ�鿡 ���� �� ���� �����̴�.
// VertexbufferCheck �Լ��� �̷��� ���ؽ��� üũ���ִ� �Լ��̴�.
// �� �ܰ迡�� �ָ��ؾ��� �޽��� �ΰ� �ִٰ� ������ ����(MeshNodeCheck)
// MeshInfo�� ������ �����ߴٸ� ���ؽ����� �ε������۸� ǥ���ؾ��ϴµ�, �̶� �ʿ��Ѱ� ��������Ʈ��.
// ���⼭ �ָ�������, �ε������۰� �����ϰ� �Ǿ��ִµ�, ������ ���� �����̴�.
// GEVertex Ŭ���� ��� �������� �������� UV, ���ؽ� ��ġ, �̷��� ������ 
// Normal, Binormal, Tangent�� ����꿡 �̿�,
// Weight�� blendindices�� �ִϸ��̼ǿ� Ȱ���Ѵ�.
// 
// VertexbufferCheck�� ����,
// �޽� ������ŭ ����� �������� ��������Ʈ�� �ϳ� �����.
// MapWI�� ���� ū �ǹ̴� ��� �Ѿ��.
// LodLv == ��Ʈ���ī
// ���� ���ؽ� ������ �����´�.
// �� ���ؽ� ������ŭ ������� �ؼ� �ε带 �ǽ��Ѵ�
// ���⼭ ComputeToTotalMatrix�� �Ѿ�µ�, ���ؽ��� ������ �� ���ؽ��� �����ַ��� �ϴ°�(?)
// �ٵ� �������� �Ȱ������� ���� �������.
// ���� Position�� ���� �����´�.
// pControlPoint ���ؽ� ��ü�� ������, �÷�4 �迭�� �츮���� �ִ� ���̴�.
// for ���� ���鼭 controlpoint��ŭ ���ϰ�, �ٽ� float4�� convert ����
// 0, 1, 2, 3, 4 ~ �̷��鼭 �ϴ� ��
// 
// BoundScaleBox�� ���� �ֻ��������� ������ ������ �������°�
// 
// �Ϲ������� FBX�� �� �Ž��� �� ���ؽ��� �� �ε����� ������ �ε��Ϸ��� �� �ؽ��ĸ� �ε��ؾ��ϵ� ��� ������ �־���Ѵ�.
// �ٵ� ���ʿ� �ؽ��İ� �������� �ʰų�(�ּ½���� ���� ����ִ� ����� ���� ���� ���),
// �ؽ��Ŀ� �Ž��� �ִµ�, ���ν��Ѿ��ϴ� ���� ��Ī���ִ� ������ ���� �� ���� << �����Ҷ� �ؽ��� �̰� ��� �ϸ� �����°� ���ٴ� ��
// 
// �Ž� ���� ���ο� �Ž��� �ؽ��İ� ���� ������ �ε尡 �����ϵ��� �����ص�
// 
// ��Ƽ���� ������ �������� ���� ������ �ε��� ���۰� ��������
// ���� ������ ���ؽ����� �ϳ� ������ �ε������� �ϳ� �ִ� ���� �Ž���� �θ��鼭 �׷��� ���� �ִ�.
// FBX���� �ϰ��ִ� ���� MeshNode�� �ְ�, �� ��� �ȿ� ���� �ε����� ���ؽ��� �ѽ�(�Ϲ����� �������� ���)���� �����Ѵ�.
// �ι�°�δ� MeshNode�� ������, �ε�������, ���ؽ����۰� �������� ��찡 �ִ�. >> �̷��� �ؾ� �� �� �� ���� �� �̷������� �ε��ϴ� ��
// ���������� �ε������۰� �������� ��찡 �ִ�. �ε��� ���۴� ���� �׸��� �����̴�. �̰� �ϳ��� ���ؽ� ���ۿ� ���������� �����ִ� ���
// 
// ��� ���ؽ����۰� �ΰ���� �Ž���尡 �ΰ���� ���� ��.
// �츮 ������ FBX �ε�� ���ؽ����۰� �ϳ�. �������� �ε������۴� ������ ������.
// �ε������۰� ������ ����ִٸ�, �ϳ��� �Ž��� �̿��ؼ� �ٸ� �ε��������� ������ ��ŵ�� ���·� �ϳ��� ������ ǥ���� �� ����
// >> �ִϸ��̼� ������ �ִµ�, �󱼸� ǥ���ϰ� �ʹٸ� �ε��� ���ۿ��� �󱼿� �ش��ϴ� �ε��� ���۸� Ű�� �׷� ��
// 
// ImportBone << FBX�� �ִϸ��̼��� ������ ��찡 �ְ�, ���� �ִϸ��̼��� ������ ��찡 �ִµ�, �ϴ� ���߿� ���(�ִϸ��̼� �Ҷ�)
//
// �Ʊ� ���ߴ� �ε������۰� �ϳ��� ���ؽ����ۿ� ������ �����ϴ� ��찡 subset ��Ȳ�� ���̴� (ĳ���Ͱ� ���������� ���¶�� ��)
// SetFBXMesh���� SetFBXMesh�� ȣ���ϰ� �ȴ�.
// ���⼭ ���� ����Ʈ�� �����.
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
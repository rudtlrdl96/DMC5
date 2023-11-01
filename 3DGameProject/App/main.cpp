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
	GameEngineCore::Start(hInstance, ContentsCore::GameStart, ContentsCore::GameEnd, {0, 0}, {1600, 900}, false);
	//GameEngineCore::Start(hInstance, ContentsCore::GameStart, ContentsCore::GameEnd, {0, 0}, {1600 / 2, 900 / 2}, false);
}

// ���� �� ���� ���ε� GECore�� Release �߰���
// �����ptr�� ������� ���� ���� ���� ���� �� ���� �Ǿ��մ�.
// �ٵ� �츮�� ���� �帧�� ������ ���� �Ҹ� ������ leaküũ �Լ��� �����ϱ� ������, ���� ��Ͽ� ��µȴ�.
// �̰� ����� ������, �����ptr�� ������ �ڷ����̶�� ������.
// 
// ���� winapi���� �ߴ��� ó�� �Ҹ��ڿ��� delete�� nullptr�� ó��������
// ������ �����ptr�̱� ������ ������ �Լ� ���ο��� �׷��� �ǽ��� ��.
// 
// 
// ���� ������ ��ݿ��� ��������Ʈ ������� ������ ������ �ٲ� ��
// ������ �������� Ʈ������������Ʈ������ �ǽ��Ѵ�.
//  
// ī�޶� map���� units ����
// 
// ������ �ϱ� ���ؼ��� winso2 ����� �ʿ��ϴ�.
// �ٵ� �츮�� ���������� ��� ��������ϱ�, �ű⿡ ������ ��
// GetLastError�� �����찡 ������ ���� � ������ ������(���� s_ok �� ����ؼ� �ڼ��� ������, �̰� ����ϸ�) �˷��ִ� �Լ���.
// 
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 0821
// 
// structuredBuffer
// ������۴� �����Ҵ��� �ȴ�. �뷮 ���ѵ� ���� (�� 700����Ʈ)
// �ִϸ��̼� ���� ������ ��� �ϳ��� 200~300�� �� �� ����
// ������ ������۷δ� ���� �� ����
// 
// �ִϸ��̼� ���� �ȿ� ����ִ� ���� ���� ������ �ƴ�
// �� ���� x ������ (����) // � ���� ���ۼ�Ʈ ���� �޴´ٴ� ������
// ���� �ֵθ��� �ִϸ��̼��̳� ������ ���ִ� �ִϸ��̼��̳� ������ ����� ����(���� ����)�� �����ϱ� ������ ���� �Ȱ���
// ���� �̰� ���̴��� �������ϴµ�, �ʹ� ���Ƽ� �� ������
// ������ �߰��� BLENDWEIGHT BLENDINDICES �߰��س���
// BLENDWEIGHT�� ���ۼ�Ʈ ���� �޴´�.
// BLENDINDICES�� � ������ ����޴´�.
// 
// 4���� �迭�� �Ǿ��ִµ�, ���� ����޴� ���� 4���� �Ѵ� ���� �ִ�. �׷��͵� �ϴ� �������� ����
// 
// ////////////////////////////////////////
// 
// 
// ////////////////////////////////////////
// 
//
//
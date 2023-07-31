#include "PrecompileHeader.h"
#include "GameEngineFBX.h"
#include <GameEngineBase/GameEngineString.h>

GameEngineFBX::GameEngineFBX() 
{
}

GameEngineFBX::~GameEngineFBX() 
{
}

void GameEngineFBX::FBXInit(std::string _Path)
{
	if (false == FBXSystemInitialize(_Path))
	{
	}
}

bool GameEngineFBX::FBXSystemInitialize(std::string _Path)
{
	// fbx���� ����ϴ� �⺻����� �����ϴ� �������̽�
	Manager = fbxsdk::FbxManager::Create();

	if (nullptr == Manager)
	{
		MsgAssert("FBX �Ŵ��� ������ �����߽��ϴ�.");
		return false;
	}

	// �˰�ġ�°� �ƴմϴ�
	// ���� �ڵ忡 �̷��� �������Ѵٰ� �ؼ� ���� �׷��� �Ѱͻ�
	IOSetting = fbxsdk::FbxIOSettings::Create(Manager, IOSROOT);

	Importer = fbxsdk::FbxImporter::Create(Manager, "");

	if (false ==
		Importer->Initialize(GameEngineString::AnsiToUTF8(_Path).c_str(), -1, IOSetting))
	{
		Importer->Destroy();
		IOSetting->Destroy();
		MsgAssert("FBX �ε� �̴ϼȶ����� ����.");
		return false;
	}

	// ����ִ� scene�� �����
	Scene = fbxsdk::FbxScene::Create(Manager, "");

	if (nullptr == Scene)
	{
		MsgAssert("FBX ������ ����.");
		return false;
	}

	// ����ִ� Scene�� �⺻���� ������ ��� ä��ϴ�.
	if (false == Importer->Import(Scene))
	{
		MsgAssert("FBX ����Ʈ ����.");
		return false;
	}

	return true;
}
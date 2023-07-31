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
	// fbx에서 사용하는 기본기능을 제공하는 인터페이스
	Manager = fbxsdk::FbxManager::Create();

	if (nullptr == Manager)
	{
		MsgAssert("FBX 매니저 생성에 실패했습니다.");
		return false;
	}

	// 알고치는게 아닙니다
	// 예제 코드에 이렇게 만들어야한다고 해서 나도 그렇게 한것뿐
	IOSetting = fbxsdk::FbxIOSettings::Create(Manager, IOSROOT);

	Importer = fbxsdk::FbxImporter::Create(Manager, "");

	if (false ==
		Importer->Initialize(GameEngineString::AnsiToUTF8(_Path).c_str(), -1, IOSetting))
	{
		Importer->Destroy();
		IOSetting->Destroy();
		MsgAssert("FBX 로드 이니셜라이즈 실패.");
		return false;
	}

	// 비엉있는 scene을 만들고
	Scene = fbxsdk::FbxScene::Create(Manager, "");

	if (nullptr == Scene)
	{
		MsgAssert("FBX 씬생성 실패.");
		return false;
	}

	// 비어있는 Scene에 기본적인 내용을 모두 채웁니다.
	if (false == Importer->Import(Scene))
	{
		MsgAssert("FBX 임포트 실패.");
		return false;
	}

	return true;
}
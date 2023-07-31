#pragma once

#include <GameEngineCore/ThirdParty/FBX/Inc/fbxsdk.h>

#pragma comment(lib, "zlib-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "libfbxsdk-md.lib")

#include <functional>

// fbx������ �⺻ ������
// scene => fbx���� 1���� ����ϴ� ��� ������ �����̰�
// fbxscene�� ���״�� ���� ǥ���ϴ� 3d�����̴�.

// ACity => Scene
//       => �ǹ�000
//            =>  â��1111
//                  =>  â�� 111
//            =>  �ι�111
//                  =>  �ι�111
// �� �ֻ����� �ִ� ���簡 scene�̴�.
// RootNode�� �ֽ��ϴ�.

// �ִϸ��̼ǰ� �Ž� ���ʿ��� ����� �ڵ���� �������̴�.
// ���� :
class GameEngineFBX
{
public:
	// constrcuter destructer
	GameEngineFBX();
	~GameEngineFBX();

	// delete Function
	GameEngineFBX(const GameEngineFBX& _Other) = delete;
	GameEngineFBX(GameEngineFBX&& _Other) noexcept = delete;
	GameEngineFBX& operator=(const GameEngineFBX& _Other) = delete;
	GameEngineFBX& operator=(GameEngineFBX&& _Other) noexcept = delete;

protected:
	void FBXInit(std::string _Path);
	bool FBXSystemInitialize(std::string _Path);

private:
	fbxsdk::FbxManager* Manager = nullptr;

	fbxsdk::FbxIOSettings* IOSetting = nullptr;
	fbxsdk::FbxImporter* Importer = nullptr;
	fbxsdk::FbxScene* Scene = nullptr;
	fbxsdk::FbxNode* RootNode = nullptr;

	fbxsdk::FbxAMatrix ConvertMatrix;
	fbxsdk::FbxAMatrix JointMatrix;
	fbxsdk::FbxVector4 AxisVector;
};


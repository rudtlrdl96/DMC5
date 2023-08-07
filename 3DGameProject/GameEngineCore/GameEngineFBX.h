#pragma once

#include <GameEngineCore/ThirdParty/FBX/Inc/fbxsdk.h>

#pragma comment(lib, "zlib-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "libfbxsdk-md.lib")

#include <functional>

// fbx구조의 기본 구조는
// scene => fbx파일 1개를 담당하는 모든 정보의 시작이고
// fbxscene은 말그대로 모든걸 표현하는 3d포맷이다.

// ACity => Scene
//       => 건물000
//            =>  창문1111
//                  =>  창살 111
//            =>  인물111
//                  =>  인물111
// 그 최상위에 있는 존재가 scene이다.
// RootNode가 있습니다.

// 애니메이션과 매쉬 양쪽에서 사용할 코드들을 넣을것이다.
// 설명 :
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

	float4x4 FbxMatTofloat4x4(const fbxsdk::FbxAMatrix& _BaseTrans);
	fbxsdk::FbxAMatrix float4x4ToFbxAMatrix(const float4x4& _MATRIX);
	float4 FbxVecTofloat4(const fbxsdk::FbxVector4& _BaseVector);
	float4 FbxVecToTransform(const fbxsdk::FbxVector4& _BaseVector);
	float4 FbxQuaternionTofloat4(const fbxsdk::FbxQuaternion& _BaseVector);

protected:
	void FBXInit(std::string _Path);
	void FBXConvertScene();
	bool FBXSystemInitialize(std::string _Path);

	fbxsdk::FbxManager* Manager = nullptr;

	fbxsdk::FbxIOSettings* IOSetting = nullptr;
	fbxsdk::FbxImporter* Importer = nullptr;
	fbxsdk::FbxScene* Scene = nullptr;
	fbxsdk::FbxNode* RootNode = nullptr;

	fbxsdk::FbxAMatrix ConvertMatrix;
	fbxsdk::FbxAMatrix JointMatrix;
	fbxsdk::FbxVector4 AxisVector;

private:
};


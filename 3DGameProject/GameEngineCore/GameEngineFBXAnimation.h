#pragma once
#include "GameEngineFBXMesh.h"



class FbxExBoneFrameData : public GameEngineSerializObject
{
public:
	float4 S; // ũ
	float4 Q; // ��
	float4 T; // ��
	double Time;
	float4x4 FrameMat;

	fbxsdk::FbxAMatrix GlobalAnimation;
	fbxsdk::FbxAMatrix LocalAnimation;

	FbxExBoneFrameData() {

	}

	void Write(GameEngineSerializer& _File) override
	{
		_File << S;
		_File << Q;
		_File << T;
		_File << Time;
		_File << FrameMat;
		_File << GlobalAnimation;
		_File << LocalAnimation;
	}

	void Read(GameEngineSerializer& _File) override
	{
		_File >> S;
		_File >> Q;
		_File >> T;
		_File >> Time;
		_File >> FrameMat;
		_File >> GlobalAnimation;
		_File >> LocalAnimation;
	}

};

class FbxExBoneFrame : public GameEngineSerializObject
{
public:
	int BoneIndex;
	int BoneParentIndex;

	// 120�������̾�.
	std::vector<FbxExBoneFrameData> BoneMatData;

	FbxExBoneFrame() {

	}

	void Write(GameEngineSerializer& _File) override
	{
		_File << BoneIndex;
		_File << BoneParentIndex;
		_File << BoneMatData;
	}

	void Read(GameEngineSerializer& _File) override
	{
		_File >> BoneIndex ;
		_File >> BoneParentIndex;
		_File >> BoneMatData;
	}
};

// Take 001�̶�� �ִϸ��̼��� �ִµ�.
class FbxExAniData : public GameEngineSerializObject
{
public:
	std::string				AniName;
	fbxsdk::FbxTime			StartTime; // ������ �ִϸ��̼� �ð�
	fbxsdk::FbxTime			EndTime; // 
	fbxsdk::FbxLongLong		TimeStartCount;
	fbxsdk::FbxLongLong		TimeEndCount; // 1414
	fbxsdk::FbxLongLong		FrameCount;
	fbxsdk::FbxTime::EMode	TimeMode;
	__int64					FbxModeCount;
	double					FbxModeRate;

	// 0~100
	// meshInfo 1�� ��ü 20
	// meshInfo 1�� ��ü
	// meshInfo 1�� ���� 20
	std::map<size_t, std::vector<FbxExBoneFrame>> AniFrameData;

	void Write(GameEngineSerializer& _File) override
	{
		_File << AniName;
		_File << StartTime;
		_File << EndTime;
		_File << TimeStartCount;
		_File << TimeEndCount;
		_File << FrameCount;
		_File << TimeMode;
		_File << FbxModeCount;
		_File << FbxModeRate;
		_File << AniFrameData;
	}

	void Read(GameEngineSerializer& _File) override
	{
		_File >> AniName;
		_File >> StartTime;
		_File >> EndTime;
		_File >> TimeStartCount;
		_File >> TimeEndCount;
		_File >> FrameCount;
		_File >> TimeMode;
		_File >> FbxModeCount;
		_File >> FbxModeRate;
		_File >> AniFrameData;
	}


public:
	float FrameTime(int _Frame)
	{
		fbxsdk::FbxTime tTime = {};
		tTime.SetFrame(_Frame, TimeMode);

		return (float)tTime.GetSecondDouble();
	}

public:
	__int64 FrameModeRateInt()
	{
		return static_cast<__int64>(fbxsdk::FbxGetFrameRate(TimeMode));
	}
	double FrameModeRateDouble()
	{
		return fbxsdk::FbxGetFrameRate(TimeMode);
	}

	float FrameModeRateFloat()
	{
		return static_cast<float>(fbxsdk::FbxGetFrameRate(TimeMode));
	}


public:
	FbxExAniData() : AniName(""), StartTime(0), EndTime(0), TimeStartCount(0), TimeEndCount(0), FrameCount(0), FbxModeCount(0), FbxModeRate(0.0), TimeMode()
	{}
	~FbxExAniData() {}
};


// ���� :
class GameEngineFBXAnimation : public GameEngineFBX, public GameEngineResource<GameEngineFBXAnimation>
{
public:
	// constrcuter destructer
	GameEngineFBXAnimation();
	~GameEngineFBXAnimation();

	// delete Function
	GameEngineFBXAnimation(const GameEngineFBXAnimation& _Other) = delete;
	GameEngineFBXAnimation(GameEngineFBXAnimation&& _Other) noexcept = delete;
	GameEngineFBXAnimation& operator=(const GameEngineFBXAnimation& _Other) = delete;
	GameEngineFBXAnimation& operator=(GameEngineFBXAnimation&& _Other) noexcept = delete;

	static std::shared_ptr<GameEngineFBXAnimation> Load(const std::string& _Path)
	{
		return Load(_Path, GameEnginePath::GetFileName(_Path));
	}

	static std::shared_ptr<GameEngineFBXAnimation> Load(const std::string& _Path, const std::string& _Name);

	void AnimationMatrixLoad(std::shared_ptr <GameEngineFBXMesh> _Mesh, const std::string_view& _Name, int _AnimationIndex);

	FbxExAniData* GetAnimationData(int _Index)
	{
		if (AnimationDatas.size() <= _Index)
		{
			MsgAssert("�ִϸ��̼� ������ ������ �Ѿ���ϴ�");
		}

		return &AnimationDatas[_Index];
	}

protected:
	void LoadMesh(const std::string& _Path, const std::string& _Name);

	// �ִϸ��̼��� ����� ��Ʈ����Ʈ�� �� eMesh�� �༮���� ��������Ƿ� �׳༮���Լ� �ִϸ��̼� �ε� �Լ��� �����Ű�� ������ �Ѵ�.
	void ProcessAnimationLoad(std::shared_ptr <GameEngineFBXMesh> _Mesh, fbxsdk::FbxNode* pNode, int _index);
	bool AnimationLoad(std::shared_ptr <GameEngineFBXMesh> _Mesh, fbxsdk::FbxNode* _Node, int AnimationIndex);
	void ProcessAnimationCheckState(std::shared_ptr <GameEngineFBXMesh> _Fbx, int userAniDataIndex);
	fbxsdk::FbxAMatrix GetGeometryTransformation(fbxsdk::FbxNode* pNode);

	// ��
	// ���̵�
	// ����
	std::vector<FbxExAniData> AnimationDatas;

private:
	bool CheckAnimation();
};


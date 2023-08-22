#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineFBXMesh.h"


class FbxExAniData;
class GameEngineFBXMesh;
class GameEngineFBXAnimation;
class GameEngineFBXRenderer;
class GameEngineFBXAnimationInfo : public std::enable_shared_from_this<GameEngineFBXAnimationInfo>
{
	friend class AnimationToolWindow;

public:
	GameEngineFBXRenderer* ParentRenderer;
	// SetFBX ���� ������ �ִ� �Ž�
	std::shared_ptr<GameEngineFBXMesh> Mesh;
	// �ִϸ��̼��� ������ �ִ� FBX
	std::shared_ptr<GameEngineFBXAnimation> Aniamtion;
	FbxExAniData* FBXAnimationData;

	float PlayTime = 0.0f;
	float CurFrameTime = 0.0f;
	float Inter = 0.1f;
	float TimeScale = 1.0f;

	std::vector<unsigned int> Frames;
	UINT CurFrame = 0;
	UINT Start = -1;
	UINT End = -1;

	bool bOnceStart = true;
	bool bOnceEnd = true;
	bool Loop = true;

	// Event

	void Init(const std::string_view& _Name, int _Index);
	void Reset();
	void Update(float _DeltaTime);

public:
	GameEngineFBXAnimationInfo()
		: Start(0)
		, End(0)
		, bOnceStart(false)
		, bOnceEnd(false)
	{
	}

	~GameEngineFBXAnimationInfo()
	{
	}
};


struct AnimationBoneData
{
public:
	int Index = -1;
	float4 Scale;
	float4 RotQuaternion;
	float4 Pos;
	float4 RotEuler;
};

// ���� :
class GameEngineFBXRenderer : public GameEngineRenderer
{
	friend class AnimationToolWindow;
	friend class GameEngineFBXAnimationInfo;

public:
	// constrcuter destructer
	GameEngineFBXRenderer();
	~GameEngineFBXRenderer();

	// delete Function
	GameEngineFBXRenderer(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer(GameEngineFBXRenderer&& _Other) noexcept = delete;
	GameEngineFBXRenderer& operator=(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer& operator=(GameEngineFBXRenderer&& _Other) noexcept = delete;

	virtual void SetFBXMesh(const std::string& _Name, std::string _Material);
	virtual void SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex);
	virtual std::shared_ptr<GameEngineRenderUnit> SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex, size_t _SubSetIndex);

	void CreateFBXAnimation(const std::string& _AnimationFBXName, int _Index = 0)
	{
		CreateFBXAnimation(_AnimationFBXName, _AnimationFBXName, _Index);
	}

	void CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, int _Index = 0);


	void Update(float _DeltaTime) override;

	void PauseSwtich();

	void ChangeAnimation(const std::string& _AnimationName);

	inline std::shared_ptr<GameEngineFBXMesh> GetFBXMesh()
	{
		return FBXMesh;
	}

	std::vector<std::vector<std::shared_ptr<GameEngineRenderUnit>>>& GetAllRenderUnit()
	{
		return Unit;
	}

	float4 GetMeshScale()
	{
		float4 f4MinBox = float4::ZERO;
		float4 f4MaxBox = float4::ZERO;
		float4 ResultBox = float4::ZERO;
		for (size_t i = 0; i < FBXMesh->GetRenderUnitCount(); i++)
		{
			float4 f4TempMinBox = float4::ZERO;
			float4 f4TempMaxBox = float4::ZERO;
			f4TempMinBox = FBXMesh->GetRenderUnit(i)->MinBoundBox;
			f4TempMaxBox = FBXMesh->GetRenderUnit(i)->MaxBoundBox;
			if (f4MinBox.x > f4TempMinBox.x)
			{
				f4MinBox.x = f4TempMinBox.x;
			}
			if (f4MinBox.y > f4TempMinBox.y)
			{
				f4MinBox.y = f4TempMinBox.y;
			}
			if (f4MinBox.z > f4TempMinBox.z)
			{
				f4MinBox.z = f4TempMinBox.z;
			}

			if (f4MaxBox.x < f4TempMaxBox.x)
			{
				f4MaxBox.x = f4TempMaxBox.x;
			}
			if (f4MaxBox.y < f4TempMaxBox.y)
			{
				f4MaxBox.y = f4TempMaxBox.y;
			}
			if (f4MaxBox.z < f4TempMaxBox.z)
			{
				f4MaxBox.z = f4TempMaxBox.z;
			}
		}
		ResultBox.x = f4MaxBox.x - f4MinBox.x;
		ResultBox.y = f4MaxBox.y - f4MinBox.y;
		ResultBox.z = f4MaxBox.z - f4MinBox.z;

		return ResultBox;
	}

protected:
	// void Render(float _DeltaTime) override;

private:
	bool Pause = false;

	// �װ� �Ұ����ϴ�.
	// ��ó�� �������� ���� �Ž��Ϳ����� �����Ǿ� �ִ� �Ž����߸� �����ϴ�.
	std::shared_ptr<GameEngineFBXMesh> FBXMesh;

	// vertexbuffer1       indexbuffer          
	// 0���Ž���            0��° �����
	std::vector<         std::vector<std::shared_ptr<GameEngineRenderUnit>>> Unit;

	std::map<std::string, std::shared_ptr<GameEngineFBXAnimationInfo>> Animations;
	std::shared_ptr<GameEngineFBXAnimationInfo> CurAnimation;

	// ó������ �׳� ��� �ִٰ�
	// �ִϸ��̼��� �Ǵ¼���
	// Ȯ��Ǿ�� �Ѵ�.
	// ������ ��ŭ
	// ���ʿ� Ű�� �Ž� ���� x �������� ���� ������ �ȴ�.
	std::map<size_t, std::vector<float4x4>> AnimationBoneMatrixs;
	std::map<size_t, std::vector<AnimationBoneData>> AnimationBoneDatas;



};


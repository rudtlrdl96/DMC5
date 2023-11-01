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
	// SetFBX 본을 가지고 있는 매쉬
	std::shared_ptr<GameEngineFBXMesh> Mesh;
	// 애니메이션을 가지고 있는 FBX
	std::shared_ptr<GameEngineFBXAnimation> Aniamtion;
	FbxExAniData* FBXAnimationData;

	float PlayTime = 0.0f;
	float CurFrameTime = 0.0f;
	float Inter = 0.1f;
	float TimeScale = 1.0f;
	float BlendIn = -1.0f;
	float BlendOut = -1.0f;

	std::vector<unsigned int> Frames;
	UINT CurFrame = 0;
	UINT Start = -1;
	UINT End = -1;

	bool bOnceStart = true;
	bool bOnceEnd = true;
	bool Loop = true;
	bool IsEnd = true;

	// Event

	void Init(std::shared_ptr<GameEngineFBXMesh> _Mesh, std::shared_ptr<GameEngineFBXAnimation> _Animation, const std::string_view& _Name, int _Index);
	void Reset();
	void Update(float _DeltaTime);

	std::map<UINT, std::vector<std::function<void()>>> AnimationEvent;
	std::map<size_t, std::function<void()>> StartEventFunction;

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

class AnimationCreateParams
{
public:
	float Inter = 0.0f;
	float BlendIn = -1.0f;
	float BlendOut = -1.0f;
	bool Loop = true;
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

struct AttachTransformInfo
{
	int Index = -1;
	std::shared_ptr<GameEngineObject> Object;
	float4 OffsetPos = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 OffsetRot = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4x4 OffsetMat;
	bool RotEffect = false;
	bool DegCalculation = false;
};

// 설명 :
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

	void Update(float _DeltaTime) override;

	virtual void SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material);
	virtual void SetFBXMesh(const std::string_view& _Name, const std::vector<std::vector<std::string>>& _Materials);
	virtual void SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material, size_t MeshIndex);
	virtual void SetFBXMesh(const std::string_view& _Name, std::vector<std::string> _Material, size_t MeshIndex);

	void SetShadowFBXMesh(const std::string_view& _Name, bool _IsStatic);

	virtual std::shared_ptr<GameEngineRenderUnit> SetFBXMesh(const std::string_view& _Name, const std::string_view& _Material, size_t MeshIndex, size_t _SubSetIndex);

	std::shared_ptr<GameEngineRenderUnit> SetShadowFBXMesh(const std::string_view& _Name, size_t _MeshIndex, size_t _SubSetIndex = 0);

	inline std::shared_ptr<GameEngineFBXMesh> GetFBXMesh()
	{
		return FBXMesh;
	}

	inline std::shared_ptr<GameEngineFBXMesh> GetFBXMesh() const
	{
		return FBXMesh;
	}

	AnimationBoneData GetBoneData(std::string _Name);

	AnimationBoneData GetBoneData(int _Index)
	{
		return AnimationBoneDatas[_Index];
	}

	inline std::vector<float4x4> GetAnimationBoneMatrixs() const
	{
		return AnimationBoneMatrixs;
	}

	std::vector<std::shared_ptr<GameEngineRenderUnit>>* GetRenderUnit(size_t _MeshIndex)
	{
		if (_MeshIndex >= Unit.size())
		{
			return nullptr;
		}

		return &Unit[_MeshIndex];
	}

	std::shared_ptr<GameEngineRenderUnit> GetRenderUnit(size_t _MeshIndex, size_t _SubsetIndex)
	{
		if (_MeshIndex >= Unit.size())
		{
			return nullptr;
		}

		if (_SubsetIndex >= Unit[_MeshIndex].size())
		{
			return nullptr;
		}

		return Unit[_MeshIndex][_SubsetIndex];
	}

	std::vector<std::vector<std::shared_ptr<GameEngineRenderUnit>>>& GetAllRenderUnit()
	{
		return Unit;
	}

	// 모든 랜더 유닛의 텍스쳐를 변경합니다.
	void SetTexture(const std::string_view& _SettingName, std::shared_ptr<GameEngineTexture> _Texture);
	void SetTexture(const std::string_view& _SettingName, const std::string_view& _ImageName);

	// 해당 이름으로 DeffuseTexture를 사용하고 있는 랜더 유닛의 머티리얼을 변경합니다.
	void SetMaterial(const std::string_view& _DeffuseTextureName, const std::string_view& _MaterialName, std::function<void(std::shared_ptr<GameEngineRenderUnit>)> _Setting);

	// 모든 랜더 유닛을 탐색하며 DiffuseTexture 의 이름이 일치하는 유닛의 _DiffuseTexture를 세팅합니다.
	// 반드시 DiffuseTexture가 먼저 세팅되어야 합니다
	void SetDiffuseTexture(const std::string_view& _OldDiffuseTexture, const std::string_view& _NewsDiffuseTexture);

	// 모든 랜더 유닛을 탐색하며 DiffuseTexture 의 이름이 일치하는 유닛의 SpecularTexture를 세팅합니다.
	// 반드시 DiffuseTexture가 먼저 세팅되어야 합니다.
	void SetSpecularTexture(const std::string_view& _DiffuseTexture, const std::string_view& _SpecularTexture);

	inline void NormalOn()
	{
		BaseValue.IsNormal = true;
	}

	inline void NormalOff()
	{
		BaseValue.IsNormal = false;
	}

	// 모든 랜더 유닛을 Static 으로 설정합니다.
	void SetStatic();

	// 모든 랜더 유닛을 Dynamic 으로 설정합니다.
	void SetDynamic();

	std::shared_ptr<GameEngineFBXAnimationInfo> GetAnimation(std::string _Name)
	{
		std::string UpperName = GameEngineString::ToUpper(_Name);
		return Animations[UpperName];
	}

	std::shared_ptr<GameEngineFBXAnimationInfo> FindAnimation(std::string _Name)
	{
		std::string UpperName = GameEngineString::ToUpper(_Name);
		if (false == Animations.contains(UpperName)) { return nullptr; }
		return Animations.find(UpperName)->second;
	}

	void CreateFBXAnimation(const std::string& _AnimationFBXName, const AnimationCreateParams& _Params, int _Index = 0)
	{
		CreateFBXAnimation(_AnimationFBXName, _AnimationFBXName, _Params, _Index);
	}

	void CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, const AnimationCreateParams& _Params, int _Index = 0);

	void ChangeAnimation(const std::string& _AnimationName, bool _Force = false);

	void SetAnimationStartEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event);

	void SetTimeScale(float _Value)
	{
		TimeScale = _Value;
	}

	void PauseSwtich()
	{
		Pause = !Pause;
	}

	void SetPlay()
	{
		Pause = false;
	}

	void SetPause()
	{
		Pause = true;
	}

	bool IsAnimationEnd()
	{
		return CurAnimation->IsEnd;
	}

	inline void SetCurFrame(UINT _Frame)
	{
		if (nullptr == CurAnimation) { return; }
		CurAnimation->CurFrame = _Frame;
	}

	inline UINT GetCurFrame()
	{
		return CurAnimation->CurFrame;
	}

	inline UINT GetEndFrame()
	{
		return CurAnimation->End;
	}

	void SetAttachTransform(std::string_view _Name, std::shared_ptr<GameEngineObject> _Object, float4 _OffsetPos = float4(0.0f, 0.0f, 0.0f, 1.0f), float4 _OffsetRot = float4(0.0f, 0.0f, 0.0f, 0.0f), bool _RotEffect = false, bool _IsDeg = false);

	void SetAttachTransform(int Index, std::shared_ptr<GameEngineObject> _Object, float4 _OffsetPos = float4(0.0f, 0.0f, 0.0f, 1.0f), float4 _OffsetRot = float4(0.0f, 0.0f, 0.0f, 0.0f), bool _RotEffect = false, bool _IsDeg = false);

	void SetDettachTransform();

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
	// vertexbuffer1       indexbuffer          
	// 0번매쉬의            0번째 서브셋
	std::vector<         std::vector<std::shared_ptr<GameEngineRenderUnit>>> Unit;
private:
	bool Pause = false;

	std::shared_ptr<GameEngineFBXMesh> FBXMesh = nullptr;
	std::map<std::string, std::shared_ptr<GameEngineFBXAnimationInfo>> Animations;
	std::shared_ptr<GameEngineFBXAnimationInfo> CurAnimation = nullptr;
	std::vector<float4x4> AnimationBoneMatrixs;		        // 스트럭처드 버퍼랑 링크
	std::vector<float4x4> AnimationBoneMatrixsNotOffset;
	std::vector<AnimationBoneData> AnimationBoneDatas;
	std::vector<AttachTransformInfo> AttachTransformValue;
	std::vector<AnimationBoneData> PrevAnimationBoneDatas;  // 이전 애니메이션 정보. 블렌드 기능에 사용
	
	float CurBlendTime = 0.0f;
	float BlendTime = 0.0f;
	float TimeScale = 1.0f;
};


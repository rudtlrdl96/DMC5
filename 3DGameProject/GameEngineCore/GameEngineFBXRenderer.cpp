#include "PrecompileHeader.h"
#include "GameEngineFBXRenderer.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineMaterial.h"


void GameEngineFBXAnimationInfo::Init(const std::string_view& _Name, int _Index)
{
	// GameENgineFBXAnimation의 행렬 정보가 완전해지는것은 
	// CalFbxExBoneFrameTransMatrix가 호출되고 난 후입니다.
	// 애니메이션의 행렬이 계산되는겁니다.
	Aniamtion->AnimationMatrixLoad(Mesh, _Name, _Index);
	FBXAnimationData = Aniamtion->GetAnimationData(_Index);
	Start = 0;
	End = static_cast<unsigned int>(FBXAnimationData->TimeEndCount);
}


// 이걸 통해서 애니메이션을 진행시키고.
void GameEngineFBXAnimationInfo::Update(float _DeltaTime)
{
	_DeltaTime *= TimeScale;
	// 0~24진행이죠?
	if (false == ParentRenderer->Pause)
	{
		CurFrameTime += _DeltaTime;
		PlayTime += _DeltaTime;
		//                      0.1
		// 1
		while (CurFrameTime >= Inter)
		{
			// 여분의 시간이 남게되죠?
			// 여분의 시간이 중요합니다.
			CurFrameTime -= Inter;
			++CurFrame;

			if (AnimationEvent.contains(CurFrame))
			{
				for (int i = 0; i < AnimationEvent[CurFrame].size(); i++)
				{
					AnimationEvent[CurFrame][i]();
				}
			}

			if (false == bOnceStart && CurFrame == 0)
			{
				bOnceStart = true;
				bOnceEnd = false;
			}

			if (CurFrame == (Frames.size() - 1)
				&& false == bOnceEnd)
			{
				bOnceEnd = true;
				bOnceStart = false;
				break;
			}

			if (CurFrame >= Frames.size() - 1)
			{
				IsEnd = true;
				if (true == Loop)
				{
					CurFrame = Start;
				}
				else
				{
					CurFrame = static_cast<unsigned int>(Frames.size()) - 1;
				}
			}
		}
	}

	unsigned int NextFrame = CurFrame;

	++NextFrame;

	if (NextFrame >= End)
	{
		IsEnd = true;
		NextFrame = 0;
	}

	if (CurFrame >= End)
	{
		IsEnd = true;
		CurFrame = 0;
	}

	// mesh      subset
	std::vector<std::vector< std::shared_ptr<GameEngineRenderUnit>>>& Units = ParentRenderer->GetAllRenderUnit();

	for (size_t UnitSetIndex = 0; UnitSetIndex < Units.size(); ++UnitSetIndex)
	{
		for (size_t RenderUnitIndex = 0; RenderUnitIndex < Units[UnitSetIndex].size(); ++RenderUnitIndex)
		{
			std::shared_ptr<GameEngineRenderUnit>& Render = Units[UnitSetIndex][RenderUnitIndex];

			// 위험!!!! 위험!!!! 뭔가 기분이 멜랑꽇ㄹㅁㄴ어ㅏ림ㄴㅇ엉라ㅣㅁㄴ
			std::map<size_t, std::vector<float4x4>>::iterator MatrixIter = ParentRenderer->AnimationBoneMatrixs.find(UnitSetIndex);

			if (MatrixIter == ParentRenderer->AnimationBoneMatrixs.end())
			{
				continue;
			}

			// 68개 
			std::vector<float4x4>& AnimationBoneMatrix = MatrixIter->second;

			std::map<size_t, std::vector<AnimationBoneData>>::iterator AnimationDataIter = ParentRenderer->AnimationBoneDatas.find(UnitSetIndex);
			// 68개 
			std::vector<AnimationBoneData>& AnimationBoneData = AnimationDataIter->second;

			size_t MeshIndex = 0;

			for (int i = 0; i < AnimationBoneMatrix.size(); i++)
			{
				if (0 == FBXAnimationData->AniFrameData[MeshIndex].size())
				{
					continue;
				}

				Bone* BoneData = ParentRenderer->GetFBXMesh()->FindBone(MeshIndex, i);

				if (true == FBXAnimationData->AniFrameData[MeshIndex][i].BoneMatData.empty())
				{
					AnimationBoneMatrix[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
					return;
				}

				// 애니메이션 블랜드 등은 하나도 안들어가 있다.

				// CurFrame + 1

				FbxExBoneFrameData& CurData = FBXAnimationData->AniFrameData[MeshIndex][i].BoneMatData[CurFrame];
				FbxExBoneFrameData& NextData = FBXAnimationData->AniFrameData[MeshIndex][i].BoneMatData[NextFrame];

				AnimationBoneData[i].Scale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
				AnimationBoneData[i].RotQuaternion = float4::SLerpQuaternion(CurData.Q, NextData.Q, CurFrameTime);
				AnimationBoneData[i].Pos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

				size_t Size = sizeof(float4x4);

				float4x4 Mat = float4x4::Affine(AnimationBoneData[i].Scale, AnimationBoneData[i].RotQuaternion, AnimationBoneData[i].Pos);

				AnimationBoneMatrix[i] = BoneData->BonePos.Offset * Mat;
			}
		}

	}
}

void GameEngineFBXAnimationInfo::Reset()
{
	CurFrameTime = 0.0f;
	CurFrame = 0;
	PlayTime = 0.0f;
	// Start = 0;
}

GameEngineFBXRenderer::GameEngineFBXRenderer()
{
	SetName("GameEngineFBXRenderer");
}

GameEngineFBXRenderer::~GameEngineFBXRenderer()
{
}

void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Name, std::string _Material)
{
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	if (nullptr == FindFBXMesh)
	{
		MsgAssert("로드하지 않은 FBX 매쉬를 사용하려고 했습니다.");
	}

	FindFBXMesh->Initialize();

	// 너 몇개 가지고 있어.
	for (size_t UnitCount = 0; UnitCount < FindFBXMesh->GetRenderUnitCount(); UnitCount++)
	{
		SetFBXMesh(_Name, _Material, UnitCount);
	}
}

void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Name, const std::vector<std::vector<std::string>>& _Materials)
{
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	if (nullptr == FindFBXMesh)
	{
		MsgAssert("로드하지 않은 FBX 매쉬를 사용하려고 했습니다.");
	}

	FindFBXMesh->Initialize();

	if (0 == _Materials.size())
	{
		MsgAssert("매쉬 머티리얼 정보가 존재 하지 않습니다.");
	}

	for (size_t UnitCount = 0; UnitCount < FindFBXMesh->GetRenderUnitCount(); UnitCount++)
	{
		if (_Materials.size() <= UnitCount)
		{
			SetFBXMesh(_Name, _Materials[0], UnitCount);
		}
		else
		{
			SetFBXMesh(_Name, _Materials[UnitCount], UnitCount);
		}
	}
}

void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex)
{
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	for (size_t SubSetCount = 0; SubSetCount < FindFBXMesh->GetSubSetCount(MeshIndex); SubSetCount++)
	{
		size_t SubSet = FindFBXMesh->GetSubSetCount(MeshIndex);

		SetFBXMesh(_Name, _Material, MeshIndex, SubSetCount);
	}
}

void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Name, std::vector<std::string> _Material, size_t MeshIndex)
{
	if (0 == _Material.size())
	{
		MsgAssert("서브셋 머티리얼 정보가 존재 하지 않습니다.");
	}

	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	for (size_t SubSetCount = 0; SubSetCount < FindFBXMesh->GetSubSetCount(MeshIndex); SubSetCount++)
	{
		if (SubSetCount >= _Material.size())
		{
			SetFBXMesh(_Name, _Material[0], MeshIndex, SubSetCount);
		}
		else
		{
			SetFBXMesh(_Name, _Material[SubSetCount], MeshIndex, SubSetCount);
		}
	}
}

// SetFbxMesh를 해서 만들어진 랜더 유니트를 사용하게 하기 위해서 리턴해준다.
std::shared_ptr<GameEngineRenderUnit> GameEngineFBXRenderer::SetFBXMesh(const std::string& _Name,
	std::string _Material,
	size_t _MeshIndex,
	size_t _SubSetIndex /*= 0*/)
{
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	if (nullptr == FindFBXMesh)
	{
		MsgAssert("존재하지 않는 FBXMesh를 세팅했습니다.");
		return nullptr;
	}


	if (nullptr == FBXMesh && nullptr != FindFBXMesh)
	{
		FBXMesh = FindFBXMesh;
	}
	else if (nullptr != FBXMesh && FBXMesh != FindFBXMesh)
	{
		// 지금까지 만든거 다 날립니다.
	}

	FindFBXMesh->Initialize();
	// return nullptr;

	if (Unit.empty())
	{
		Unit.resize(FBXMesh->GetRenderUnitCount());
		for (size_t i = 0; i < Unit.size(); i++)
		{
			size_t Count = FBXMesh->GetSubSetCount(i);

			Unit[i].resize(Count);
			for (size_t j = 0; j < Count; j++)
			{
				Unit[i][j] = CreateRenderUnit();
			}
		}
	}

	std::shared_ptr<GameEngineRenderUnit> RenderUnit = Unit[_MeshIndex][_SubSetIndex];
	std::shared_ptr <GameEngineMesh> GetFBXMesh = FBXMesh->GetGameEngineMesh(_MeshIndex, _SubSetIndex);

	RenderUnit->SetMesh(GetFBXMesh);
	RenderUnit->SetMaterial(_Material);

	if (RenderUnit->ShaderResHelper.IsStructuredBuffer("ArrAniMationMatrix"))
	{
		if (AnimationBoneMatrixs.end() == AnimationBoneMatrixs.find(_MeshIndex))
		{
			size_t Count = FBXMesh->GetBoneCount(_MeshIndex);
			AnimationBoneMatrixs[_MeshIndex].resize(Count);
			AnimationBoneDatas[_MeshIndex].resize(Count);
		}

		GameEngineStructuredBufferSetter* AnimationBuffer = RenderUnit->ShaderResHelper.GetStructuredBufferSetter("ArrAniMationMatrix");

		AnimationBuffer->Res = FBXMesh->GetAnimationStructuredBuffer(_MeshIndex);

		if (nullptr == AnimationBuffer->Res)
		{
			MsgAssert("애니메이션용 스트럭처드 버퍼가 존재하지 않습니다.");
			return RenderUnit;
		}

		if (0 == AnimationBoneMatrixs[_MeshIndex].size())
		{
			return RenderUnit;
		}

		// 링크를 걸어준것.
		AnimationBuffer->SetData = &AnimationBoneMatrixs[_MeshIndex][0];
		AnimationBuffer->Size = sizeof(float4x4);
		AnimationBuffer->Count = AnimationBoneMatrixs[_MeshIndex].size();
	}


	if (RenderUnit->ShaderResHelper.IsTexture("DiffuseTexture"))
	{
		const FbxExMaterialSettingData& MatData = FBXMesh->GetMaterialSettingData(_MeshIndex, _SubSetIndex);

		if (nullptr != GameEngineTexture::Find(MatData.DifTextureName))
		{
			RenderUnit->ShaderResHelper.SetTexture("DiffuseTexture", MatData.DifTextureName);
		}
	}

	if (RenderUnit->ShaderResHelper.IsTexture("NormalTexture"))
	{
		const FbxExMaterialSettingData& MatData = FBXMesh->GetMaterialSettingData(_MeshIndex, _SubSetIndex);

		if (nullptr != GameEngineTexture::Find(MatData.NorTextureName))
		{
			RenderUnit->ShaderResHelper.SetTexture("NormalTexture", MatData.NorTextureName);
		}

		BaseValue.IsNormal = 1;
	}	
	
	if (RenderUnit->ShaderResHelper.IsTexture("SpecularTexture"))
	{
		const FbxExMaterialSettingData& MatData = FBXMesh->GetMaterialSettingData(_MeshIndex, _SubSetIndex);

		if (nullptr != GameEngineTexture::Find(MatData.SpcTextureName))
		{
			RenderUnit->ShaderResHelper.SetTexture("SpecularTexture", MatData.SpcTextureName);
		}

		BaseValue.IsNormal = 1;
	}


	return RenderUnit;
}

//void GameEngineFBXRenderer::Render(float _DeltaTime)
//{
	// GameEngineRenderer::Render(_DeltaTime);

	//for (size_t UnitIndex = 0; UnitIndex < Unit.size(); UnitIndex++)
	//{
	//	for (size_t SubSetIndex = 0; SubSetIndex < Unit[UnitIndex].size(); SubSetIndex++)
	//	{
	//		if (nullptr == Unit[UnitIndex][SubSetIndex]->Pipe)
	//		{
	//			continue;
	//		}

	//		//if (nullptr == Unit[UnitIndex][SubSetIndex].GetMaterial()->GetPixelShader()->GetIsDeffered())
	//		//{
	//		//	continue;
	//		//}

	//		Unit[UnitIndex][SubSetIndex]->Render(_DeltaTime);
	//	}
	//}
//}

void GameEngineFBXRenderer::CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, int _Index)
{
	// 애니메이션 방식은 무조건 1개일것이라고 보고.
	std::string UpperName = GameEngineString::ToUpper(_AnimationName);

	// 본을 가진 fbx가 세팅되어 있는지 검사한다.
	if (nullptr == GetFBXMesh())
	{
		MsgAssert("골격 FBX가 세팅되어 있지 않습니다.");
		return;
	}

	// 여기있는 함수를 외부에서 호출하면 됩니다.

	if (Animations.end() != Animations.find(UpperName))
	{
		MsgAssert("이미 존재하는 이름의 애니메이션입니다." + UpperName);
		return;
	}

	std::shared_ptr<GameEngineFBXAnimation> Animation = GameEngineFBXAnimation::Find(_AnimationFBXName);

	if (nullptr == Animation)
	{
		MsgAssert("GameEngineFBXAnimation이 존재하지 않습니다. " + std::string(_AnimationFBXName));
		return;
	}

	Animation->Initialize();

	std::shared_ptr<GameEngineFBXAnimationInfo> NewAnimation = std::make_shared<GameEngineFBXAnimationInfo>();
	FbxExAniData* AnimData = Animation->GetAnimationData(_Index);
	NewAnimation->Start = static_cast<UINT>(AnimData->TimeStartCount);
	NewAnimation->End = static_cast<UINT>(AnimData->TimeEndCount);
	NewAnimation->Mesh = GetFBXMesh();
	NewAnimation->Aniamtion = Animation;
	NewAnimation->ParentRenderer = this;
	NewAnimation->Reset();
	NewAnimation->Init(_AnimationName, _Index);

	BaseValue.IsAnimation = 1;
	Animations.insert(std::make_pair(UpperName, NewAnimation));

	Animation;
}



void GameEngineFBXRenderer::PauseSwtich()
{
	Pause = !Pause;
}

void GameEngineFBXRenderer::ChangeAnimation(const std::string& _AnimationName)
{
	std::string UpperName = GameEngineString::ToUpper(_AnimationName);

	std::map<std::string, std::shared_ptr<GameEngineFBXAnimationInfo>>::iterator FindIter = Animations.find(UpperName);

	if (Animations.end() == FindIter)
	{
		MsgAssert("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
		return;
	}

	CurAnimation = FindIter->second;
	CurAnimation->CurFrame = 0;
	CurAnimation->IsEnd = false;

	if (CurAnimation->AnimationEvent.contains(0))
	{
		for (int i = 0; i < CurAnimation->AnimationEvent[0].size(); i++)
		{
			CurAnimation->AnimationEvent[0][i]();
		}
	}
}

bool GameEngineFBXRenderer::IsAnimationEnd()
{
	return CurAnimation->IsEnd;
}


void GameEngineFBXRenderer::Update(float _DeltaTime)
{
	if (nullptr == CurAnimation)
	{
		return;
	}

	CurAnimation->Update(_DeltaTime);
}
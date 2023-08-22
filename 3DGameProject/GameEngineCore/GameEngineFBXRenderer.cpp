#include "PrecompileHeader.h"
#include "GameEngineFBXRenderer.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineMaterial.h"


void GameEngineFBXAnimationInfo::Init(const std::string_view& _Name, int _Index)
{
	// GameENgineFBXAnimation�� ��� ������ ���������°��� 
	// CalFbxExBoneFrameTransMatrix�� ȣ��ǰ� �� ���Դϴ�.
	// �ִϸ��̼��� ����� ���Ǵ°̴ϴ�.
	Aniamtion->AnimationMatrixLoad(Mesh, _Name, _Index);
	FBXAnimationData = Aniamtion->GetAnimationData(_Index);
	Start = 0;
	End = static_cast<unsigned int>(FBXAnimationData->TimeEndCount);
}


// �̰� ���ؼ� �ִϸ��̼��� �����Ű��.
void GameEngineFBXAnimationInfo::Update(float _DeltaTime)
{
	// 0~24��������?
	if (false == ParentRenderer->Pause)
	{
		CurFrameTime += _DeltaTime;
		PlayTime += _DeltaTime;
		//                      0.1
		// 1
		while (CurFrameTime >= Inter)
		{
			// ������ �ð��� ���Ե���?
			// ������ �ð��� �߿��մϴ�.
			CurFrameTime -= Inter;
			++CurFrame;

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
		NextFrame = 0;
	}

	if (CurFrame >= End)
	{
		CurFrame = 0;
	}

	// mesh      subset
	std::vector<std::vector< std::shared_ptr<GameEngineRenderUnit>>>& Units = ParentRenderer->GetAllRenderUnit();

	for (size_t UnitSetIndex = 0; UnitSetIndex < Units.size(); ++UnitSetIndex)
	{
		for (size_t RenderUnitIndex = 0; RenderUnitIndex < Units[UnitSetIndex].size(); ++RenderUnitIndex)
		{
			std::shared_ptr<GameEngineRenderUnit>& Render = Units[UnitSetIndex][RenderUnitIndex];

			// ����!!!! ����!!!! ���� ����� ����������������������Ӥ���
			std::map<size_t, std::vector<float4x4>>::iterator MatrixIter = ParentRenderer->AnimationBoneMatrixs.find(UnitSetIndex);

			if (MatrixIter == ParentRenderer->AnimationBoneMatrixs.end())
			{
				continue;
			}

			// 68�� 
			std::vector<float4x4>& AnimationBoneMatrix = MatrixIter->second;

			std::map<size_t, std::vector<AnimationBoneData>>::iterator AnimationDataIter = ParentRenderer->AnimationBoneDatas.find(UnitSetIndex);
			// 68�� 
			std::vector<AnimationBoneData>& AnimationBoneData = AnimationDataIter->second;

			size_t MeshIndex = MatrixIter->first;

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

				// �ִϸ��̼� ���� ���� �ϳ��� �ȵ� �ִ�.

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
}

GameEngineFBXRenderer::~GameEngineFBXRenderer()
{
}


void GameEngineFBXRenderer::SetFBXMesh(const std::string& _Name, std::string _Material)
{
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	if (nullptr == FindFBXMesh)
	{
		MsgAssert("�ε����� ���� FBX �Ž��� ����Ϸ��� �߽��ϴ�.");
	}

	// �� � ������ �־�.
	for (size_t UnitCount = 0; UnitCount < FindFBXMesh->GetRenderUnitCount(); UnitCount++)
	{
		SetFBXMesh(_Name, _Material, UnitCount);
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

// SetFbxMesh�� �ؼ� ������� ���� ����Ʈ�� ����ϰ� �ϱ� ���ؼ� �������ش�.
std::shared_ptr<GameEngineRenderUnit> GameEngineFBXRenderer::SetFBXMesh(const std::string& _Name,
	std::string _Material,
	size_t _MeshIndex,
	size_t _SubSetIndex /*= 0*/)
{
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_Name);

	if (nullptr == FindFBXMesh)
	{
		MsgAssert("�������� �ʴ� FBXMesh�� �����߽��ϴ�.");
		return nullptr;
	}


	if (nullptr == FBXMesh && nullptr != FindFBXMesh)
	{
		FBXMesh = FindFBXMesh;
	}
	else if (nullptr != FBXMesh && FBXMesh != FindFBXMesh)
	{
		// ���ݱ��� ����� �� �����ϴ�.
	}

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
			MsgAssert("�ִϸ��̼ǿ� ��Ʈ��ó�� ���۰� �������� �ʽ��ϴ�.");
			return RenderUnit;
		}

		if (0 == AnimationBoneMatrixs[_MeshIndex].size())
		{
			return RenderUnit;
		}

		// ��ũ�� �ɾ��ذ�.
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
	// �ִϸ��̼� ����� ������ 1���ϰ��̶�� ����.
	std::string UpperName = GameEngineString::ToUpper(_AnimationName);

	// ���� ���� fbx�� ���õǾ� �ִ��� �˻��Ѵ�.
	if (nullptr == GetFBXMesh())
	{
		MsgAssert("��� FBX�� ���õǾ� ���� �ʽ��ϴ�.");
		return;
	}

	// �����ִ� �Լ��� �ܺο��� ȣ���ϸ� �˴ϴ�.

	if (Animations.end() != Animations.find(UpperName))
	{
		MsgAssert("�̹� �����ϴ� �̸��� �ִϸ��̼��Դϴ�." + UpperName);
		return;
	}

	std::shared_ptr<GameEngineFBXAnimation> Animation = GameEngineFBXAnimation::Find(_AnimationFBXName);

	if (nullptr == Animation)
	{
		MsgAssert("GameEngineFBXAnimation�� �������� �ʽ��ϴ�. " + std::string(_AnimationFBXName));
		return;
	}


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
		MsgAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
		return;
	}

	CurAnimation = FindIter->second;
}


void GameEngineFBXRenderer::Update(float _DeltaTime)
{
	if (nullptr == CurAnimation)
	{
		return;
	}

	CurAnimation->Update(_DeltaTime);
}
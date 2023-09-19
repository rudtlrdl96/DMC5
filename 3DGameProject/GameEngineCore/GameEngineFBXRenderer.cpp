#include "PrecompileHeader.h"
#include "GameEngineFBXRenderer.h"
#include "GameEngineFBXAnimation.h"
#include "GameEngineMaterial.h"


void GameEngineFBXAnimationInfo::Init(std::shared_ptr<GameEngineFBXMesh> _Mesh, std::shared_ptr<GameEngineFBXAnimation> _Animation, const std::string_view& _Name, int _Index)
{
	// GameENgineFBXAnimation�� ��� ������ ���������°��� 
	// CalFbxExBoneFrameTransMatrix�� ȣ��ǰ� �� ���Դϴ�.
	// �ִϸ��̼��� ����� ���Ǵ°̴ϴ�.

	_Animation->AnimationMatrixLoad(_Mesh, _Name, _Index);
	Aniamtion = _Animation;
	FBXAnimationData = Aniamtion->GetAnimationData(_Index);
	Start = static_cast<UINT>(FBXAnimationData->TimeStartCount);
	End = static_cast<UINT>(FBXAnimationData->TimeEndCount);
	Mesh = _Mesh;
	Aniamtion = _Animation;


	Start = 0;
	End = static_cast<unsigned int>(FBXAnimationData->TimeEndCount);
}


// �̰� ���ؼ� �ִϸ��̼��� �����Ű��.
void GameEngineFBXAnimationInfo::Update(float _DeltaTime)
{
	_DeltaTime *= TimeScale;

	if (false == ParentRenderer->Pause)
	{
		CurFrameTime += _DeltaTime;
		PlayTime += _DeltaTime;

		while (CurFrameTime >= Inter)
		{
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

			if ((Frames.size() - 1) == CurFrame
				&& false == bOnceEnd)
			{
				bOnceEnd = true;
				bOnceStart = false;
				break;
			}

			if ((Frames.size() - 1) <= CurFrame)
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
			else
			{
				if (StartEventFunction.end() != StartEventFunction.find(CurFrame))
				{
					StartEventFunction[CurFrame]();
				}
			}
		}
	}

	unsigned int NextFrame = CurFrame;

	++NextFrame;

	if (NextFrame >= End)
	{
		IsEnd = true;
		if (true == Loop)
		{
			NextFrame = Start;
		}
		else
		{
			NextFrame = End - 1;
		}
	}

	if (CurFrame >= End)
	{
		IsEnd = true;
		if (true == Loop)
		{
			CurFrame = Start;
		}
		else
		{
			CurFrame = End - 1;
		}
	}

	// mesh      subset
	std::vector<std::vector< std::shared_ptr<GameEngineRenderUnit>>>& Units = ParentRenderer->GetAllRenderUnit();
	std::vector<float4x4>& AnimationBoneMatrix = ParentRenderer->AnimationBoneMatrixs;

	std::vector<AnimationBoneData>& AnimationBoneData = ParentRenderer->AnimationBoneDatas;

	for (int i = 0; i < AnimationBoneMatrix.size(); i++)
	{
		if (0 == FBXAnimationData->AniFrameData.size())
		{
			continue;
		}

		Bone* BoneData = ParentRenderer->GetFBXMesh()->FindBone(i);

		if (true == FBXAnimationData->AniFrameData[i].BoneMatData.empty())
		{
			AnimationBoneMatrix[i] = float4x4::Affine(BoneData->BonePos.GlobalScale, BoneData->BonePos.GlobalRotation, BoneData->BonePos.GlobalTranslation);
			return;
		}

		// �ִϸ��̼� ���� ���� �ϳ��� �ȵ� �ִ�.

		// CurFrame + 1

		FbxExBoneFrameData& CurData = FBXAnimationData->AniFrameData[i].BoneMatData[CurFrame];
		FbxExBoneFrameData& NextData = FBXAnimationData->AniFrameData[i].BoneMatData[NextFrame];

		AnimationBoneData[i].Scale = float4::Lerp(CurData.S, NextData.S, CurFrameTime);
		AnimationBoneData[i].RotQuaternion = float4::SLerpQuaternion(CurData.Q, NextData.Q, CurFrameTime);
		AnimationBoneData[i].Pos = float4::Lerp(CurData.T, NextData.T, CurFrameTime);

		size_t Size = sizeof(float4x4);

		float4x4 Mat = float4x4::Affine(AnimationBoneData[i].Scale, AnimationBoneData[i].RotQuaternion, AnimationBoneData[i].Pos);

		AnimationBoneMatrix[i] = BoneData->BonePos.Offset * Mat;
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
		MsgAssert("�ε����� ���� FBX �Ž��� ����Ϸ��� �߽��ϴ�.");
	}

	FindFBXMesh->Initialize();

	// �� � ������ �־�.
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
		MsgAssert("�ε����� ���� FBX �Ž��� ����Ϸ��� �߽��ϴ�.");
	}

	FindFBXMesh->Initialize();

	if (0 == _Materials.size())
	{
		MsgAssert("�Ž� ��Ƽ���� ������ ���� ���� �ʽ��ϴ�.");
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
		MsgAssert("����� ��Ƽ���� ������ ���� ���� �ʽ��ϴ�.");
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

	if (0 == AnimationBoneMatrixs.size())
	{
		size_t Count = FBXMesh->GetBoneCount();

		if (nullptr == FBXMesh)
		{
			MsgAssert("���� �������� �ʴ� �Ž��� �ִϸ��̼��� �������� �߽��ϴ�.");
		}

		AnimationBoneMatrixs.resize(Count);
		AnimationBoneDatas.resize(Count);
	}


	if (RenderUnit->ShaderResHelper.IsStructuredBuffer("ArrAniMationMatrix"))
	{
		GameEngineStructuredBufferSetter* AnimationBuffer = RenderUnit->ShaderResHelper.GetStructuredBufferSetter("ArrAniMationMatrix");

		AnimationBuffer->Res = FBXMesh->GetAnimationStructuredBuffer();

		if (nullptr == AnimationBuffer->Res)
		{
			MsgAssert("�ִϸ��̼ǿ� ��Ʈ��ó�� ���۰� �������� �ʽ��ϴ�.");
			return RenderUnit;
		}

		if (0 == AnimationBoneMatrixs.size())
		{
			return RenderUnit; 
		}

		// ��ũ�� �ɾ��ذ�.
		AnimationBuffer->SetData = &AnimationBoneMatrixs[0];
		AnimationBuffer->Size = sizeof(float4x4);
		AnimationBuffer->Count = AnimationBoneMatrixs.size();
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
			BaseValue.IsNormal = 1;
		}
		else
		{
			RenderUnit->ShaderResHelper.SetTexture("NormalTexture", "EngineBaseNormal.tga");
		}
	}

	if (RenderUnit->ShaderResHelper.IsTexture("SpecularTexture"))
	{
		const FbxExMaterialSettingData& MatData = FBXMesh->GetMaterialSettingData(_MeshIndex, _SubSetIndex);

		if (nullptr != GameEngineTexture::Find(MatData.SpcTextureName))
		{
			RenderUnit->ShaderResHelper.SetTexture("SpecularTexture", MatData.SpcTextureName);
			BaseValue.IsSpecular = 1;
		}
		else
		{
			RenderUnit->ShaderResHelper.SetTexture("SpecularTexture", "EngineBaseSpecular.tga");
		}
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

void GameEngineFBXRenderer::CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, const AnimationCreateParams& _Params, int _Index)
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

	//GameEngineFile File;
	//File.SetPath(Animation->GetPath());
	//File.ChangeExtension(".AnimationFBX");

	//if (false == File.IsExists())
	//{
	//	Animation->Initialize();
	//}

	std::shared_ptr<GameEngineFBXAnimationInfo> NewAnimation = std::make_shared<GameEngineFBXAnimationInfo>();
	NewAnimation->Init(FBXMesh, Animation, _AnimationName, _Index);
	NewAnimation->ParentRenderer = this;
	NewAnimation->Inter = _Params.Inter;
	NewAnimation->Loop = _Params.Loop;
	NewAnimation->Reset();

	BaseValue.IsAnimation = 1;
	Animations.insert(std::make_pair(UpperName, NewAnimation));

	Animation;
}


void GameEngineFBXRenderer::PauseSwtich()
{
	Pause = !Pause;
}

bool GameEngineFBXRenderer::IsAnimationEnd()
{
	return CurAnimation->IsEnd;
}


void GameEngineFBXRenderer::SetTexture(const std::string_view& _SettingName, std::shared_ptr<GameEngineTexture> _Texture)
{
	for (size_t i = 0; i < Unit.size(); i++)
	{
		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			if (false == Unit[i][j]->ShaderResHelper.IsTexture(_SettingName.data()))
			{
				continue;
			}

			Unit[i][j]->ShaderResHelper.SetTexture(_SettingName, _Texture);
		}
	}
}

void GameEngineFBXRenderer::SetTexture(const std::string_view& _SettingName, const std::string_view& _ImageName)
{
	SetTexture(_SettingName, GameEngineTexture::Find(_ImageName));
}

void GameEngineFBXRenderer::SetMaterial(const std::string_view& _DeffuseTextureName, const std::string_view& _MaterialName, std::function<void(std::shared_ptr<GameEngineRenderUnit>)> _Setting)
{
	std::string UpperName = GameEngineString::ToUpper(_DeffuseTextureName);

	for (size_t i = 0; i < Unit.size(); i++)
	{
		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			std::string DiffuseTextureName = Unit[i][j]->ShaderResHelper.GetTextureSetter("DiffuseTexture")->Res->GetNameToString();

			if (UpperName == DiffuseTextureName)
			{
				Unit[i][j]->SetMaterial(_MaterialName);
				_Setting(Unit[i][j]);
			}
		}
	}
}

void GameEngineFBXRenderer::SetDiffuseTexture(const std::string_view& _OldDiffuseTexture, const std::string_view& _NewsDiffuseTexture)
{
	std::string UpperOldName = GameEngineString::ToUpper(_OldDiffuseTexture);
	std::string UpperNewName = GameEngineString::ToUpper(_NewsDiffuseTexture);

	for (size_t i = 0; i < Unit.size(); i++)
	{
		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			std::string DiffuseTextureName = Unit[i][j]->ShaderResHelper.GetTextureSetter("DiffuseTexture")->Res->GetNameToString();

			if (UpperOldName == DiffuseTextureName)
			{
				Unit[i][j]->ShaderResHelper.SetTexture("DiffuseTexture", UpperNewName);
			}
		}
	}
}

void GameEngineFBXRenderer::SetSpecularTexture(const std::string_view& _DiffuseTexture, const std::string_view& _SpecularTexture)
{
	std::string UpperTexName = GameEngineString::ToUpper(_DiffuseTexture);
	std::string UpperSpeName = GameEngineString::ToUpper(_SpecularTexture);

	for (size_t i = 0; i < Unit.size(); i++)
	{
		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			std::string DiffuseTextureName = Unit[i][j]->ShaderResHelper.GetTextureSetter("DiffuseTexture")->Res->GetNameToString();

			if (UpperTexName == DiffuseTextureName)
			{
				Unit[i][j]->ShaderResHelper.SetTexture("SpecularTexture", UpperSpeName);
			}
		}
	}
}

void GameEngineFBXRenderer::Update(float _DeltaTime)
{
	if (nullptr == CurAnimation)
	{
		return;
	}

	CurAnimation->Update(_DeltaTime);
}

void GameEngineFBXRenderer::ChangeAnimation(const std::string& _AnimationName, bool _Force /*= false*/)
{
	std::string UpperName = GameEngineString::ToUpper(_AnimationName);

	std::map<std::string, std::shared_ptr<GameEngineFBXAnimationInfo>>::iterator FindIter = Animations.find(UpperName);

	if (Animations.end() == FindIter)
	{
		MsgAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
		return;
	}

	if (false == _Force && CurAnimation == FindIter->second)
	{
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

	// �����Ҷ� Start �̺�Ʈ üũ ������Ʈ
	if (CurAnimation->StartEventFunction.end() != CurAnimation->StartEventFunction.find(0))
	{
		CurAnimation->StartEventFunction[0]();
	}
}

void GameEngineFBXRenderer::SetAnimationStartEvent(const std::string_view& _AnimationName, size_t _Frame, std::function<void()> _Event)
{
	std::string UpperName = GameEngineString::ToUpper(_AnimationName);

	std::map<std::string, std::shared_ptr<GameEngineFBXAnimationInfo>>::iterator FindInfo = Animations.find(UpperName);

	if (Animations.end() == FindInfo)
	{
		MsgAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
		return;
	}

	FindInfo->second->StartEventFunction[_Frame] = _Event;
}

void GameEngineFBXRenderer::SetStatic()
{
	for (size_t i = 0; i < Unit.size(); i++)
	{
		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			Unit[i][j]->IsStatic = true;
		}
	}
}

void GameEngineFBXRenderer::SetDynamic()
{
	for (size_t i = 0; i < Unit.size(); i++)
	{
		for (size_t j = 0; j < Unit[i].size(); j++)
		{
			Unit[i][j]->IsStatic = false;
		}
	}
}
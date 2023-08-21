#include "PrecompileHeader.h"
#include "GameEngineFBXRenderer.h"


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
	// �� � ������ �־�.
	for (size_t UnitCount = 0; UnitCount < FindFBXMesh->GetRenderUnitCount(); UnitCount++)
	{
		// size_t SubSet = FindFBXMesh->GetSubSetCount(UnitCount);

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
	size_t Index,
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

	std::shared_ptr<GameEngineRenderUnit> RenderUnit = Unit[Index][_SubSetIndex];
	std::shared_ptr <GameEngineMesh> GetFBXMesh = FBXMesh->GetGameEngineMesh(Index, _SubSetIndex);

	RenderUnit->SetMesh(GetFBXMesh);
	RenderUnit->SetMaterial(_Material);

	if (RenderUnit->ShaderResHelper.IsTexture("DiffuseTexture"))
	{
		const FbxExMaterialSettingData& MatData = FBXMesh->GetMaterialSettingData(Index, _SubSetIndex);

		if (nullptr != GameEngineTexture::Find(MatData.DifTextureName))
		{
			RenderUnit->ShaderResHelper.SetTexture("DiffuseTexture", MatData.DifTextureName);
		}
	}


	if (RenderUnit->ShaderResHelper.IsTexture("NormalTexture"))
	{
		const FbxExMaterialSettingData& MatData = FBXMesh->GetMaterialSettingData(Index, _SubSetIndex);

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

#include "PrecompileHeader.h"
#include "GameEngineFBX.h"
#include <GameEngineBase/GameEngineString.h>

bool GameEngineFBX::IsCheckAnimationFBX(std::string_view _Path)
{
	GameEngineFBX FBX;

	FBX.FBXInit(_Path.data());

	return FBX.CheckAnimationFBX(_Path);
}

bool GameEngineFBX::CheckAnimationFBX(std::string_view _Path)
{
	int geometryCount = Scene->GetGeometryCount();
	for (int i = 0; i < geometryCount; i++)
	{
		// ����߿��� ���ϱ����� �����༮���� �̾Ƴ��°��̰�.
		fbxsdk::FbxGeometry* geoMetry = Scene->GetGeometry(i);
		fbxsdk::FbxNode* geoMetryNode = geoMetry->GetNode();

		// FBXInfoDebugFunction(geoMetryNode);

		if (nullptr == geoMetry)
		{
			continue;
		}

		// �̾Ƴ� �ֵ��߿��� �� Ÿ����
		if (geoMetry->GetAttributeType() != fbxsdk::FbxNodeAttribute::eMesh)
		{
			continue;
		}

		fbxsdk::FbxMesh* Mesh = reinterpret_cast<fbxsdk::FbxMesh*>(geoMetry);

		fbxsdk::FbxNode* pNode = Mesh->GetNode();
		fbxsdk::FbxMesh* FbxMesh = Mesh;

		const int SkinDeformerCount = FbxMesh->GetDeformerCount(fbxsdk::FbxDeformer::eSkin);
		for (int DeformerIndex = 0; DeformerIndex < SkinDeformerCount; DeformerIndex++)
		{
			fbxsdk::FbxSkin* Skin = (fbxsdk::FbxSkin*)FbxMesh->GetDeformer(DeformerIndex, fbxsdk::FbxDeformer::eSkin);
			for (int ClusterIndex = 0; ClusterIndex < Skin->GetClusterCount(); ClusterIndex++)
			{
				_Path;
				// ���� ���� �Ž���
				return false;
			}
		}
	}

	return true;
}

GameEngineFBX::GameEngineFBX()
{
}

GameEngineFBX::~GameEngineFBX()
{
	if (nullptr != Scene)
	{
		Scene->Destroy();
		Scene = nullptr;
	}
	if (nullptr != Importer)
	{
		Importer->Destroy();
		Importer = nullptr;
	}
	if (nullptr != IOSetting)
	{
		IOSetting->Destroy();
		IOSetting = nullptr;
	}
	if (nullptr != Manager)
	{
		Manager->Destroy();
		Manager = nullptr;
	}
}

std::vector<FBXNodeInfo> GameEngineFBX::CheckAllNode()
{
	std::vector<FBXNodeInfo> AllNode;

	RecursiveAllNode(RootNode, [&](fbxsdk::FbxNode* _Node)
		{
			FBXNodeInfo& _NodeInfo = AllNode.emplace_back();

			_NodeInfo.Name = _Node->GetName();
			_NodeInfo.Node = _Node;
		});

	return AllNode;

}

void GameEngineFBX::RecursiveAllNode(fbxsdk::FbxNode* _Node, std::function<void(fbxsdk::FbxNode*)> _Function /*= nullptr*/)
{
	if (nullptr != _Function)
	{
		_Function(_Node);
	}

	fbxsdk::FbxNodeAttribute* Info = _Node->GetNodeAttribute();

	if (nullptr != Info)
	{
		fbxsdk::FbxNodeAttribute::EType Type = Info->GetAttributeType();
	}


	int Count = _Node->GetChildCount();

	for (int i = 0; i < Count; i++)
	{
		fbxsdk::FbxNode* Node = _Node->GetChild(i);
		RecursiveAllNode(Node, _Function);
	}

}

void GameEngineFBX::FBXInit(std::string _Path)
{
	if (false == FBXSystemInitialize(_Path))
	{
		MsgAssert("�ý��� �ε忡 �����߽��ϴ�.");
	}

	FBXConvertScene();
}

bool GameEngineFBX::FBXSystemInitialize(std::string _Path)
{
	// fbx���� ����ϴ� �⺻����� �����ϴ� �������̽�
	Manager = fbxsdk::FbxManager::Create();

	if (nullptr == Manager)
	{
		MsgAssert("FBX �Ŵ��� ������ �����߽��ϴ�.");
		return false;
	}

	// �˰�ġ�°� �ƴմϴ�
	// ���� �ڵ忡 �̷��� �������Ѵٰ� �ؼ� ���� �׷��� �Ѱͻ�
	IOSetting = fbxsdk::FbxIOSettings::Create(Manager, IOSROOT);

	Importer = fbxsdk::FbxImporter::Create(Manager, "");

	if (false ==
		Importer->Initialize(GameEngineString::AnsiToUTF8(_Path).c_str(), -1, IOSetting))
	{
		Importer->Destroy();
		IOSetting->Destroy();
		MsgAssert("FBX �ε� �̴ϼȶ����� ����.");
		return false;
	}

	// ����ִ� scene�� �����
	Scene = fbxsdk::FbxScene::Create(Manager, "");

	if (nullptr == Scene)
	{
		MsgAssert("FBX ������ ����.");
		return false;
	}

	// ����ִ� Scene�� �⺻���� ������ ��� ä��ϴ�.
	if (false == Importer->Import(Scene))
	{
		MsgAssert("FBX ����Ʈ ����.");
		return false;
	}

	return true;
}



float4x4 GameEngineFBX::FbxMatTofloat4x4(const fbxsdk::FbxAMatrix& _BaseTrans)
{
	float4x4 Mat;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			Mat.Arr2D[y][x] = (float)_BaseTrans.Get(y, x);
		}
	}

	return Mat;
}

fbxsdk::FbxAMatrix GameEngineFBX::float4x4ToFbxAMatrix(const float4x4& _MATRIX)
{
	fbxsdk::FbxAMatrix mat;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			mat.mData[y].mData[x] = _MATRIX.Arr2D[y][x];
		}
	}

	return mat;
}

float4 GameEngineFBX::FbxVecTofloat4(const fbxsdk::FbxVector4& _BaseVector)
{
	float4 Vec;
	Vec.Arr1D[0] = (float)_BaseVector.mData[0];
	Vec.Arr1D[1] = (float)_BaseVector.mData[1];
	Vec.Arr1D[2] = (float)_BaseVector.mData[2];
	Vec.Arr1D[3] = (float)_BaseVector.mData[3];

	return Vec;

}

float4 GameEngineFBX::FbxVecToTransform(const fbxsdk::FbxVector4& _BaseVector)
{
	float4 Vec;
	Vec.Arr1D[0] = (float)_BaseVector.mData[0];
	Vec.Arr1D[1] = (float)_BaseVector.mData[1];
	Vec.Arr1D[2] = -(float)_BaseVector.mData[2];
	Vec.Arr1D[3] = (float)_BaseVector.mData[3];
	return Vec;
}

float4 GameEngineFBX::FbxQuaternionTofloat4(const fbxsdk::FbxQuaternion& _BaseQ)
{
	float4 Vec;
	Vec.Arr1D[0] = (float)_BaseQ.mData[0];
	Vec.Arr1D[1] = (float)_BaseQ.mData[1];
	Vec.Arr1D[2] = -(float)_BaseQ.mData[2];
	Vec.Arr1D[3] = -(float)_BaseQ.mData[3];
	return Vec;
}

void GameEngineFBX::FBXConvertScene()
{
	AxisVector = { 0.0f, 0.0f, 0.0f, 1.0f };

	fbxsdk::FbxAxisSystem::EFrontVector FrontVector = (fbxsdk::FbxAxisSystem::EFrontVector)-fbxsdk::FbxAxisSystem::eParityOdd;
	fbxsdk::FbxAxisSystem::EUpVector UpVector = fbxsdk::FbxAxisSystem::eYAxis;
	fbxsdk::FbxAxisSystem::ECoordSystem CooreSystem = fbxsdk::FbxAxisSystem::ECoordSystem::eRightHanded;

	fbxsdk::FbxAxisSystem EngineAxisSystem(UpVector, FrontVector, CooreSystem);
	// fbxsdk::FbxAxisSystem EngineAxisSystem(fbxsdk::FbxAxisSystem::EPreDefinedAxisSystem::eDirectX);

	fbxsdk::FbxAxisSystem SourceAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();

	// �̰� ���� ����Ʈ ����� �ϴ°̴ϴ�.
	if (SourceAxisSystem != EngineAxisSystem)
	{
		// ���� ��Ʈ�� ����.
		fbxsdk::FbxRootNodeUtility::RemoveAllFbxRoots(Scene);
		// ��Ʈ�� ���Ӱ� ���������.
		EngineAxisSystem.ConvertScene(Scene);

		// �Ž��� ���� ��Ʈ������ �����ɴϴ�.
		fbxsdk::FbxAMatrix SourceMatrix;
		SourceAxisSystem.GetMatrix(SourceMatrix);

		fbxsdk::FbxAMatrix EngineMatrix;
		EngineAxisSystem.GetMatrix(EngineMatrix);

		ConvertMatrix = SourceMatrix.Inverse() * EngineMatrix;
	}

	int OriginUpSign = 0;
	int OriginFrontSign = 0;
	int EngineUpSign = 0;
	int EngineFrontSign = 0;

	fbxsdk::FbxAxisSystem::EFrontVector OriginFrontVector = SourceAxisSystem.GetFrontVector(OriginFrontSign);
	fbxsdk::FbxAxisSystem::EFrontVector EngineFrontVector = EngineAxisSystem.GetFrontVector(EngineFrontSign);

	fbxsdk::FbxAxisSystem::EUpVector OriginUpVector = SourceAxisSystem.GetUpVector(OriginUpSign);
	fbxsdk::FbxAxisSystem::EUpVector EngineUpVector = EngineAxisSystem.GetUpVector(EngineUpSign);

	fbxsdk::FbxAxisSystem::ECoordSystem  OriginCoordSystem = SourceAxisSystem.GetCoorSystem();
	fbxsdk::FbxAxisSystem::ECoordSystem  EngineCoordSystem = EngineAxisSystem.GetCoorSystem();

	if (OriginUpVector == EngineUpVector && OriginUpSign == EngineUpSign)
	{
		if (OriginFrontSign != EngineFrontSign && OriginCoordSystem == EngineCoordSystem)
		{
			AxisVector.mData[static_cast<int>(OriginUpVector) - 1] += 180;
		}
	}
	else if (OriginUpVector == EngineUpVector && OriginUpSign != EngineUpSign)
	{
		if (OriginUpVector == fbxsdk::FbxAxisSystem::EUpVector::eXAxis)
		{
			AxisVector.mData[1] += 180;
		}
		else
		{
			AxisVector.mData[0] += 180;
		}

		if (OriginFrontSign != EngineFrontSign && OriginCoordSystem == EngineCoordSystem)
		{
			AxisVector.mData[static_cast<int>(OriginUpVector) - 1] += 180;
		}
	}
	else if (OriginUpVector == fbxsdk::FbxAxisSystem::EUpVector::eXAxis)
	{
		//origin up vector �� x up vector �϶��� ���� ���������� ���ߴ�.

		AxisVector.mData[1] += OriginUpSign * 90;
	}
	else
	{
		AxisVector.mData[0] += OriginUpSign * 90;
		if (OriginUpSign != EngineFrontSign)
		{
			AxisVector.mData[static_cast<int>(OriginUpVector) - 1] += OriginUpSign * 180;
		}
	}

	JointMatrix.SetR(AxisVector);

	if (true == JointMatrix.IsIdentity())
	{
		JointMatrix = ConvertMatrix;
	}

	// �ִϸ��̼ǵ� ����� �����ε�.
	Scene->GetAnimationEvaluator()->Reset();

	// �ٽ��� ��Ʈ��带 ���;� �Ѵ�.
	RootNode = Scene->GetRootNode();

	if (nullptr == RootNode)
	{
		MsgAssert("��Ʈ�������� �����߽��ϴ�.");
	}

	fbxsdk::FbxGeometryConverter Con(Manager);

	// �⺻������ ��� FBX�� �������� ���� �ﰢ������ �ȵǾ��ִ� �����ε�
	// �� �ﰢ���� �Լ��� ���ؼ� �� ���ؽ����� �츮�� ����� �� �ִ� ������ ������ش�.
	if (false == Con.Triangulate(Scene, true))
	{
		MsgAssert("�ﰢȭ�� �����߽��ϴ�.");
	}

	return;
}

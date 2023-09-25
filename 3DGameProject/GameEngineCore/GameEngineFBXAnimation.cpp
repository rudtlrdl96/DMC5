#include "PrecompileHeader.h"
#include "GameEngineFBXAnimation.h"

GameEngineFBXAnimation::GameEngineFBXAnimation()
{
}

GameEngineFBXAnimation::~GameEngineFBXAnimation()
{
	Release();
}

void GameEngineFBXAnimation::Release()
{
	AnimationDatas.clear();
	AnimationDatas.shrink_to_fit();
}

std::shared_ptr<GameEngineFBXAnimation> GameEngineFBXAnimation::Load(const std::string& _Path, const std::string& _Name)
{
	std::shared_ptr<GameEngineFBXAnimation> NewRes = Create(_Name);
	NewRes->SetPath(_Path);
	NewRes->LoadMesh(_Path, _Name);
	return NewRes;
}

void GameEngineFBXAnimation::Initialize()
{
	GameEngineFile File;
	File.SetPath(GetPathToString());
	File.ChangeExtension(".AnimationFBX");

	if (true == IsInit)
	{
		return;
	}

	FBXInit(GetPathToString());
	CheckAnimation();



	// std::string FileName = _Name.data();
	// FileName += ".AnimationFBX";

	//GameEngineFile SaveFile = Dir.GetPlusFileName(FileName);
	//if (SaveFile.IsExists())
	//{
	//	UserLoad(SaveFile.GetFullPath());
	//	return;
	//}


	IsInit = true;
}

void GameEngineFBXAnimation::LoadMesh(const std::string& _Path, const std::string& _Name)
{
	SetPath(_Path);

	//FBXInit(_Path);

	//// �Ž��� ���� �Ž��� �ְ�
	//// �ִϸ��̼� �ȿ� 

	//CheckAnimation();

	// importbone

	// �ִϸ��̼��� �����Ѵ� => �װɷ� ���� ������ ���� �ʴ´�.

	AnimationDatas;
}

bool GameEngineFBXAnimation::CheckAnimation()
{
	std::string_view Path = GetPath();

	fbxsdk::FbxArray<FbxString*> AniNameArray;
	Scene->FillAnimStackNameArray(AniNameArray);

	if (0 == AniNameArray.Size())
	{
		return false;
	}

	AnimationDatas.resize(AniNameArray.Size());

	for (unsigned int i = 0; i < AnimationDatas.size(); i++)
	{
		AnimationDatas[i].AniName = GameEngineString::UTF8ToAnsi(AniNameArray[i]->Buffer());

		FbxTakeInfo* TakeInfo = Scene->GetTakeInfo(AnimationDatas[i].AniName.c_str());
		AnimationDatas[i].StartTime = TakeInfo->mLocalTimeSpan.GetStart();
		AnimationDatas[i].EndTime = TakeInfo->mLocalTimeSpan.GetStop();
		AnimationDatas[i].TimeMode = Scene->GetGlobalSettings().GetTimeMode();
		AnimationDatas[i].TimeStartCount = AnimationDatas[i].StartTime.GetFrameCount(AnimationDatas[i].TimeMode);

		if (0 >= AnimationDatas[i].TimeStartCount)
		{
			AnimationDatas[i].TimeStartCount *= (FbxLongLong)-1;
		}

		AnimationDatas[i].TimeEndCount = AnimationDatas[i].EndTime.GetFrameCount(AnimationDatas[i].TimeMode);
		AnimationDatas[i].FrameCount = AnimationDatas[i].TimeEndCount - AnimationDatas[i].TimeStartCount;

		AnimationDatas[i].FbxModeCount = (long long)fbxsdk::FbxTime::GetFrameRate(AnimationDatas[i].TimeMode);
		AnimationDatas[i].FbxModeRate = (double)fbxsdk::FbxTime::GetFrameRate(AnimationDatas[i].TimeMode);
	}

	for (int i = 0; i < AniNameArray.Size(); i++)
	{
		delete AniNameArray[i];
	}

	return true;
}


// �� �� �Ž��� ���� ���� ����� �������°�.
fbxsdk::FbxAMatrix GameEngineFBXAnimation::GetGeometryTransformation(fbxsdk::FbxNode* pNode)
{
	fbxsdk::FbxVector4 translation = pNode->GetGeometricTranslation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 rotation = pNode->GetGeometricRotation(fbxsdk::FbxNode::eSourcePivot);
	fbxsdk::FbxVector4 scale = pNode->GetGeometricScaling(fbxsdk::FbxNode::eSourcePivot);

	return fbxsdk::FbxAMatrix(translation, rotation, scale);
}


bool GameEngineFBXAnimation::AnimationLoad(std::shared_ptr <GameEngineFBXMesh> _Mesh, fbxsdk::FbxNode* _Node, int AnimationIndex)
{
	FbxAnimStack* stack = Scene->GetSrcObject<FbxAnimStack>(AnimationIndex);
	// �� ���� �ִϸ��̼��� �� ������ �ִϸ��̼����� �������ش�.
	Scene->SetCurrentAnimationStack(stack);

	fbxsdk::FbxMesh* pCurrMesh = _Node->GetMesh();
	int deformerCount = pCurrMesh->GetDeformerCount();
	fbxsdk::FbxAMatrix geometryTransform = GetGeometryTransformation(_Node);



	fbxsdk::FbxTakeInfo* takeInfo = Scene->GetTakeInfo(AnimationDatas[AnimationIndex].AniName.c_str());
	fbxsdk::FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
	fbxsdk::FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
	fbxsdk::FbxTime::EMode timeMode = Scene->GetGlobalSettings().GetTimeMode();

	fbxsdk::FbxAMatrix currentTransformOffset;
	fbxsdk::FbxAMatrix localTransform;
	fbxsdk::FbxAMatrix globalTransform;

	fbxsdk::FbxCluster* pCurrCluster;
	std::string currJointName;
	Bone* pBone;

	fbxsdk::FbxAMatrix transformMatrix;
	fbxsdk::FbxAMatrix transformLinkMatrix;
	fbxsdk::FbxAMatrix globalBindposeInverseMatrix;

	fbxsdk::FbxLongLong fixIndex;

	std::string linkName;

	fbxsdk::FbxLongLong endTime;
	fbxsdk::FbxLongLong startTime;

	fbxsdk::FbxTime currTime;


	for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
	{
		fbxsdk::FbxSkin* pCurrSkin = reinterpret_cast<fbxsdk::FbxSkin*>(pCurrMesh->GetDeformer(deformerIndex, fbxsdk::FbxDeformer::eSkin));
		if (nullptr == pCurrSkin)
		{
			continue;
		}

		unsigned int numOfClusters = pCurrSkin->GetClusterCount();

		endTime = end.GetFrameCount(timeMode);
		startTime = start.GetFrameCount(timeMode);

		FbxExAniData& CurAniData = AnimationDatas[AnimationIndex];
		CurAniData.EndTime = endTime;
		CurAniData.StartTime = startTime;
		CurAniData.TimeMode = timeMode;

		FbxExBoneFrame& RootFrame = CurAniData.AniFrameData[0];

		if (0 == CurAniData.AniFrameData.size())
		{
			return false;
		}

		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			pCurrCluster = pCurrSkin->GetCluster(clusterIndex);
			currJointName = pCurrCluster->GetLink()->GetName();
			pBone = _Mesh->FindBone(currJointName);

			pCurrCluster->GetTransformMatrix(transformMatrix);
			pCurrCluster->GetTransformLinkMatrix(transformLinkMatrix);
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse();

			linkName = pCurrCluster->GetLink()->GetName();
			fbxsdk::FbxNode* pLinkNode = Scene->FindNodeByName(linkName.c_str());

			FbxExBoneFrame& Frame = CurAniData.AniFrameData[pBone->Index];
			Frame.BoneMatData.resize(endTime - startTime + 1);
			Frame.BoneIndex = pBone->Index;
			Frame.BoneParentIndex = pBone->ParentIndex;

			for (fbxsdk::FbxLongLong i = startTime; i <= endTime; ++i)
			{
				fixIndex = i - startTime;

				FbxExBoneFrameData& FrameData = Frame.BoneMatData[fixIndex];
				FbxExBoneFrameData& RootFrameData = RootFrame.BoneMatData[fixIndex];

				currTime.SetFrame(fixIndex, timeMode);
				// ��
				currentTransformOffset = _Node->EvaluateGlobalTransform(currTime) * JointMatrix * geometryTransform;
				// �ð��� �־��ָ� �׶��� ���� ����� ������ �ش�.
				// Ŀ�� 
				globalTransform = currentTransformOffset.Inverse() * pLinkNode->EvaluateGlobalTransform(currTime);
				float4 ResetRoot = float4(-RootFrameData.T.x, -RootFrameData.T.y, RootFrameData.T.z);
				globalTransform.SetT(globalTransform.GetT() + float4ToFbxVec(ResetRoot));

				localTransform.SetS(pLinkNode->EvaluateLocalScaling(currTime));
				localTransform.SetR(pLinkNode->EvaluateLocalRotation(currTime));
				localTransform.SetT(pLinkNode->EvaluateLocalTranslation(currTime));

				FrameData.Time = currTime.GetSecondDouble();
				FrameData.LocalAnimation = localTransform;
				FrameData.GlobalAnimation = globalTransform;
				FrameData.FrameMat = FbxMatTofloat4x4(FrameData.GlobalAnimation);
				FrameData.S = FbxVecTofloat4(FrameData.GlobalAnimation.GetS());
				FrameData.Q = FbxQuaternionTofloat4(FrameData.GlobalAnimation.GetQ());
				FrameData.T = FbxVecToTransform(FrameData.GlobalAnimation.GetT());
			}
		}
	}

	if (0 != deformerCount)
	{
		return true;
	}

	// return;

	{
		endTime = end.GetFrameCount(timeMode);
		startTime = start.GetFrameCount(timeMode);

		FbxExAniData& CurAniData = AnimationDatas[AnimationIndex];
		CurAniData.EndTime = endTime;
		CurAniData.StartTime = startTime;
		CurAniData.TimeMode = timeMode;


		std::vector<FBXNodeInfo> ALLNODE = CheckAllNode();

		_Mesh->AllBones;


		fbxsdk::FbxNode* pLinkNode = Scene->FindNodeByName(linkName.c_str());

		// ALLNODE[0].Node->GetAnimationEvaluator();

		//for (size_t i = 0; i < ALLNODE.size(); i++)
		//{

			//FbxAnimEvaluator* AnimEvaluator = ALLNODE[i].Node->GetAnimationEvaluator();

			//// ALLNODE[i].Node->GetAnimationInterval()

			//if (nullptr == AnimEvaluator)
			//{
			//	continue;
			//}

			for (size_t boneIndex = 0; boneIndex < CurAniData.AniFrameData.size(); boneIndex++)
			{
				FbxExBoneFrame& Frame = CurAniData.AniFrameData[boneIndex];
				Frame.BoneMatData.resize(endTime - startTime + 1);
				Frame.BoneIndex = static_cast<int>(boneIndex);
				Frame.BoneParentIndex = static_cast<int>(boneIndex);

				for (fbxsdk::FbxLongLong j = startTime; j <= endTime; ++j)
				{
					fixIndex = j - startTime;

					FbxExBoneFrameData& FrameData = Frame.BoneMatData[fixIndex];

					currTime.SetFrame(fixIndex, timeMode);
					// ��
					currentTransformOffset = _Node->EvaluateGlobalTransform(currTime) * JointMatrix * geometryTransform;
					// �ð��� �־��ָ� �׶��� ���� ����� ������ �ش�.
					// Ŀ�� 

					fbxsdk::FbxNode* Node = _Node;

					globalTransform = currentTransformOffset.Inverse() * Node->EvaluateGlobalTransform(currTime);

					localTransform.SetS(Node->EvaluateLocalScaling(currTime));
					localTransform.SetR(Node->EvaluateLocalRotation(currTime));
					localTransform.SetT(Node->EvaluateLocalTranslation(currTime));

					FrameData.Time = currTime.GetSecondDouble();
					FrameData.LocalAnimation = localTransform;
					FrameData.GlobalAnimation = globalTransform;
					FrameData.FrameMat = FbxMatTofloat4x4(FrameData.GlobalAnimation);
					FrameData.S = FbxVecTofloat4(FrameData.GlobalAnimation.GetS());
					FrameData.Q = FbxQuaternionTofloat4(FrameData.GlobalAnimation.GetQ());
					FrameData.T = FbxVecToTransform(FrameData.GlobalAnimation.GetT());
				}
			}

		//}
	}




	return false;
}


void GameEngineFBXAnimation::ProcessAnimationCheckState(std::shared_ptr <GameEngineFBXMesh> _Fbx, int userAniDataIndex)
{
	// �ڴ�
	FbxExAniData& userAniData = AnimationDatas.at(userAniDataIndex);
	fbxsdk::FbxLongLong fbxTime = userAniData.EndTime.Get() - userAniData.StartTime.Get() + 1;

	// �ִϸ��̼� ������ ����ִ� �༮���� ���� offset�̶�� �ϴ� T
	// 
	// ����
	size_t aniFrameDataSize = userAniData.AniFrameData.size();
	for (size_t aniFrameDataIndex = 0; aniFrameDataIndex < aniFrameDataSize; ++aniFrameDataIndex)
	{
		FbxExBoneFrame& aniFrameData = userAniData.AniFrameData.at(aniFrameDataIndex);
		// ���������.
		if (0 == aniFrameData.BoneMatData.size())
		{
			aniFrameData.BoneMatData.resize(fbxTime);
			Bone& curBone = _Fbx->AllBones[aniFrameDataIndex];
			aniFrameData.BoneIndex = curBone.Index;
			aniFrameData.BoneParentIndex = curBone.ParentIndex;
			if (-1 != curBone.ParentIndex)
			{
				FbxExBoneFrame& parentAniFrameData = userAniData.AniFrameData.at(curBone.ParentIndex);
				for (fbxsdk::FbxLongLong start = 0; start < fbxTime; ++start)
				{
					aniFrameData.BoneMatData[start].Time = parentAniFrameData.BoneMatData[start].Time;
					aniFrameData.BoneMatData[start].LocalAnimation = float4x4ToFbxAMatrix(curBone.BonePos.Local);
					aniFrameData.BoneMatData[start].GlobalAnimation = parentAniFrameData.BoneMatData[start].GlobalAnimation * aniFrameData.BoneMatData[start].LocalAnimation;
					aniFrameData.BoneMatData[start].FrameMat = FbxMatTofloat4x4(aniFrameData.BoneMatData[start].GlobalAnimation);
					aniFrameData.BoneMatData[start].S = FbxVecTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetS());
					aniFrameData.BoneMatData[start].Q = FbxQuaternionTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetQ());
					aniFrameData.BoneMatData[start].T = FbxVecToTransform(aniFrameData.BoneMatData[start].GlobalAnimation.GetT());
				}
			}
			else
			{
				for (fbxsdk::FbxLongLong start = 0; start < fbxTime; ++start)
				{
					aniFrameData.BoneMatData[start].Time = 0;
					aniFrameData.BoneMatData[start].LocalAnimation = float4x4ToFbxAMatrix(curBone.BonePos.Local);
					aniFrameData.BoneMatData[start].GlobalAnimation = aniFrameData.BoneMatData[start].LocalAnimation;
					aniFrameData.BoneMatData[start].FrameMat = FbxMatTofloat4x4(aniFrameData.BoneMatData[start].GlobalAnimation);
					aniFrameData.BoneMatData[start].S = FbxVecTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetS());
					aniFrameData.BoneMatData[start].Q = FbxQuaternionTofloat4(aniFrameData.BoneMatData[start].GlobalAnimation.GetQ());
					aniFrameData.BoneMatData[start].T = FbxVecToTransform(aniFrameData.BoneMatData[start].GlobalAnimation.GetT());
				}
			}
		}
	}
}


// �ִϸ��̼��� ����� ��Ʈ����Ʈ�� �� eMesh�� �༮���� ��������Ƿ� �׳༮���Լ� �ִϸ��̼� �ε� �Լ��� �����Ű�� ������ �Ѵ�.
void GameEngineFBXAnimation::ProcessAnimationLoad(std::shared_ptr <GameEngineFBXMesh> _Mesh, fbxsdk::FbxNode* pNode, int _index)
{
	// ��� ��带 �� �ȱ� ���ؼ�

	fbxsdk::FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();
	if (nullptr != pNodeAttribute)
	{
		fbxsdk::FbxNodeAttribute::EType typetype = pNodeAttribute->GetAttributeType();
		switch (pNodeAttribute->GetAttributeType())
		{
		case fbxsdk::FbxNodeAttribute::eLODGroup:
		{
			if (0 != pNode->GetChildCount())
			{
				ProcessAnimationLoad(_Mesh, pNode->GetChild(0), _index);
				return;
			}
		}
		break;
		case fbxsdk::FbxNodeAttribute::eMesh:
		{
			AnimationLoad(_Mesh, pNode, _index);
		}
		break;
		default:
			break;
		}
	}

	for (int n = 0; n < pNode->GetChildCount(); ++n)
	{
		ProcessAnimationLoad(_Mesh, pNode->GetChild(n), _index);
	}
}


// ���� ���� GameEngineFBXMesh������� �ִϸ��̼� ����� ������.
void GameEngineFBXAnimation::AnimationMatrixLoad(std::shared_ptr <GameEngineFBXMesh> _Mesh, const std::string_view& _Name, int _AnimationIndex)
{
	GameEngineFile DirFile = GetPath();
	DirFile.ChangeExtension(".AnimationFBX");
	if (DirFile.IsExists())
	{
		GameEngineSerializer Ser;
		DirFile.LoadBin(Ser);
		Ser >> AnimationDatas;
		return;
	}

	Initialize();

	if (0 == AnimationDatas.size())
	{
		MsgAssert("�ִϸ��̼� �����Ͱ� �������� �ʴ� �Ž��� �ִϸ��̼��� ������� �߽��ϴ�.");
		return;
	}

	if (0 != AnimationDatas[_AnimationIndex].AniFrameData.size())
	{
		return;
	}

	AnimationDatas[_AnimationIndex].AniFrameData.resize(_Mesh->GetBoneCount());

	if (nullptr == _Mesh->RootNode)
	{
		GameEngineFile File = _Mesh->GetPath();
		GameEngineDirectory Dir = File.GetDirectory();
		_Mesh->FBXInit(Dir.GetPlusFileName(_Mesh->FBXMeshName).GetFullPath());
	}

	ProcessAnimationLoad(_Mesh, _Mesh->RootNode, _AnimationIndex);

	ProcessAnimationCheckState(_Mesh, _AnimationIndex);

	if (false == DirFile.IsExists())
	{
		GameEngineSerializer Ser;
		Ser << AnimationDatas;
		DirFile.SaveBin(Ser);
		return;
	}

	AnimationDatas;
}

#include "PrecompileHeader.h"
#include "FieldMapObject.h"

#include "Fountain.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXBoxComponent.h>

FieldMapObject::FieldMapObject()
{

}

FieldMapObject::~FieldMapObject()
{

}

void FieldMapObject::Start()
{
}

void FieldMapObject::Update(float _DeltaTime)
{
}

void FieldMapObject::MapObj_SetFBXMesh(const std::string_view& _Name, const std::string_view& _Mat)
{
	if (FBXMesh == nullptr)
	{
		MsgAssert("맵 오브젝트의 FBXMesh가 nullptr 입니다.");
	}

	if (GameEngineOption::GetOption("Shader") == GameEngineOptionValue::Low)
	{
		FBXMesh->SetFBXMesh(_Name.data(), _Mat.data() + std::string("_Low"));
	}
	else
	{
		FBXMesh->SetFBXMesh(_Name.data(), _Mat.data());
	}

	//PhysXBox = CreateComponent<PhysXBoxComponent>();
	//physx::PxVec3 PxScale = { FBXMesh->GetMeshScale().x, FBXMesh->GetMeshScale().y, FBXMesh->GetMeshScale().z };
	//PhysXBox->SetObstacleObject();
	//PhysXBox->CreatePhysXActors(PxScale);
}

std::shared_ptr<FieldMapObject> FieldMapObject::CreateFieldMapObj(GameEngineLevel* _Level, FieldMapObjType _FieldMapObjType, const ObjTransformData& _ObjTransform)
{
	std::shared_ptr<FieldMapObject> Result = nullptr;
	switch (_FieldMapObjType)
	{
	case FieldMapObjType::Null:
		MsgAssert("FieldMapObjType이 Null입니다");
		break;
	case FieldMapObjType::Test0:
		Result = _Level->CreateActor<Fountain>();
			break;
	case FieldMapObjType::Test1:
		Result = _Level->CreateActor<FieldMapObject>();
		break;
	default:
		MsgAssert("미구현");
		break;
	}

	Result->GetTransform()->SetLocalPosition(_ObjTransform.Pos);
	Result->GetTransform()->SetLocalScale(_ObjTransform.Scale);
	Result->GetTransform()->SetLocalRotation(_ObjTransform.Rot);

	return Result;
}
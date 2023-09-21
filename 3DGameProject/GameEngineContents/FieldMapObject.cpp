#include "PrecompileHeader.h"
#include "FieldMapObject.h"

#include "TestObj.h"
#include "TestEventZone.h"

FieldMapObject::FieldMapObject()
{

}

FieldMapObject::~FieldMapObject()
{

}



std::shared_ptr<FieldMapObject> FieldMapObject::CreateFieldMapObj(GameEngineLevel* _Level, FieldMapObjType _FieldMapObjType, const class ObjTransformData& _ObjTransform)
{
	std::shared_ptr<FieldMapObject> Result = nullptr;
	switch (_FieldMapObjType)
	{
	case FieldMapObjType::Null:
		MsgAssert("FieldMapObjType�� Null�Դϴ�");
		break;
	case FieldMapObjType::Test0:
		Result = _Level->CreateActor<TestObj>();
		break;
	case FieldMapObjType::Test1:
		Result = _Level->CreateActor<TestEventZone>();
		break;
	default:
		MsgAssert("�̱���");
		break;
	}

	Result->GetTransform()->SetLocalPosition(_ObjTransform.Pos);
	Result->GetTransform()->SetLocalScale(_ObjTransform.Scale);
	Result->GetTransform()->SetLocalRotation(_ObjTransform.Rot);

	return Result;
}

const std::shared_ptr<class GameEngineFBXRenderer> FieldMapObject::GetFBXMesh()
{
	return FBXMesh;
}

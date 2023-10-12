#include "PrecompileHeader.h"
#include "FieldMapObject.h"

#include "StageBaseLevel.h"

#include "TestEventZone.h"
#include "WallLight_On.h"
#include "WallLight_Off.h"
#include "ReflectionSetter.h"
#include "StreetLight_On.h"
#include "StreetLight_Off.h"
#include "DecoLight.h"
#include "Fountain.h"
#include "StreetStall.h"
#include "RedSeal.h"

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
		MsgAssert("FieldMapObjType이 Null입니다");
		break;
	case FieldMapObjType::ReflectionSetter:
		Result = _Level->CreateActor<ReflectionSetter>();
		Result->GetTransform()->SetLocalPosition(_ObjTransform.Pos);
		return Result;
		break;
	case FieldMapObjType::WallLight_On:
		Result = _Level->CreateActor<WallLight_On>();
		break;
	case FieldMapObjType::WallLight_Off:
		Result = _Level->CreateActor<WallLight_Off>();
		break;
	case FieldMapObjType::StreetLight_On:
		Result = _Level->CreateActor<StreetLight_On>();
		break;
	case FieldMapObjType::StreetLight_Off:
		Result = _Level->CreateActor<StreetLight_Off>();
		break;
	case FieldMapObjType::DecoLight:
		Result = _Level->CreateActor<DecoLight>();
		break;
	case FieldMapObjType::Fountain:
		Result = _Level->CreateActor<Fountain>();
		break;
	case FieldMapObjType::StreetStall:
		Result = _Level->CreateActor<StreetStall>();
		break;
	case FieldMapObjType::Bench:
		Result = _Level->CreateActor<StreetStall>();
		break;
	case FieldMapObjType::RedSeal:
		Result = _Level->CreateActor<RedSeal>();
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

const std::shared_ptr<class GameEngineFBXRenderer> FieldMapObject::GetFBXMesh()
{
	return FBXMesh;
}

void FieldMapObject::SetDebugRender()
{
	DebugRenderPivot = CreateComponent<GameEngineComponent>();
	DebugRenderPivot->GetTransform()->SetLocalScale({ 100.f,100.f,100.f });
}

void FieldMapObject::DrawDebugRender(float _DeltaTime)
{
	if (GetLevel()->DynamicThis<StageBaseLevel>()->IsEditLevel)
	{
		GameEngineDebug::DrawSphere(GetLevel()->GetCamera(0).get(), DebugRenderPivot->GetTransform());
	}
}

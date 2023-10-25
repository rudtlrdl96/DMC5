#include "PrecompileHeader.h"
#include "FieldMapObject.h"

#include "StageBaseLevel.h"
#include <GameEngineCore/PhysXBoxComponent.h>

#include "WallLight_On.h"
#include "WallLight_Off.h"
#include "ReflectionSetter.h"
#include "StreetLight_On.h"
#include "StreetLight_Off.h"
#include "DecoLight.h"
#include "Fountain.h"
#include "StreetStall.h"
#include "RedSeal.h"
#include "Bench.h"
#include "StatueB.h"
#include "Location2_EnemySpawner0.h"
#include "Location2_EnemySpawner1.h"
#include "Item_RedOrb.h"
#include "Item_DevilBreaker.h"
#include "ElectronicBillboard.h"
#include "illminary.h"
#include "DivinityStatue.h"
#include "Item_RedOrbLump.h"

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
		Result = _Level->CreateActor<Bench>();
		break;
	case FieldMapObjType::RedSeal:
		Result = _Level->CreateActor<RedSeal>();
		break;
	case FieldMapObjType::StatueB:
		Result = _Level->CreateActor<StatueB>();
		break;
	case FieldMapObjType::Location2_EnemySpawner0:
		Result = _Level->CreateActor<Location2_EnemySpawner0>();
		break;
	case FieldMapObjType::Location2_EnemySpawner1:
		Result = _Level->CreateActor<Location2_EnemySpawner1>();
		break;
	case FieldMapObjType::Item_RedOrb:
		Result = _Level->CreateActor<Item_RedOrb>();
		break;
	case FieldMapObjType::Item_DevilBreaker_Overture:
		Result = _Level->CreateActor<Item_DevilBreaker>();
		Result->DynamicThis<Item_DevilBreaker>()->SetDevilBreaker(DevilBreaker::Overture);
		break;
	case FieldMapObjType::Item_DevilBreaker_Gerbera:
		Result = _Level->CreateActor<Item_DevilBreaker>();
		Result->DynamicThis<Item_DevilBreaker>()->SetDevilBreaker(DevilBreaker::Gerbera);
		break;
	case FieldMapObjType::Item_DevilBreaker_BusterArm:
		Result = _Level->CreateActor<Item_DevilBreaker>();
		Result->DynamicThis<Item_DevilBreaker>()->SetDevilBreaker(DevilBreaker::BusterArm);
		break;
	case FieldMapObjType::ElectronicBillboard:
		Result = _Level->CreateActor<ElectronicBillboard>();
		break;
	case FieldMapObjType::illminary:
		Result = _Level->CreateActor<illminary>();
		break;
	case FieldMapObjType::DivinityStatue:
		Result = _Level->CreateActor<DivinityStatue>();
		break;
	case FieldMapObjType::Item_RedOrbLump:
		Result = _Level->CreateActor<Item_RedOrbLump>();
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


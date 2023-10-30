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
#include "ArcadeGate.h"
#include "ArcadeCafeRoof01.h"
#include "ArcadeCafeRoof02.h"
#include "StoreWhite01.h"
#include "StoreWood01.h"
#include "LevelChangeZone.h"
#include "l02_03_StreetLine.h"
#include "FloatingIsland.h"
#include "FloatingIsland01.h"
#include "FloatingIsland02.h"
#include "tunnelWallLight.h"

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
	case FieldMapObjType::ArcadeGate:
		Result = _Level->CreateActor<ArcadeGate>();
		break;
	case FieldMapObjType::ArcadeCafeRoof01:
		Result = _Level->CreateActor<ArcadeCafeRoof01>();
		break;
	case FieldMapObjType::ArcadeCafeRoof02:
		Result = _Level->CreateActor<ArcadeCafeRoof02>();
		break;
	case FieldMapObjType::StoreWhite01:
		Result = _Level->CreateActor<StoreWhite01>();
		break;
	case FieldMapObjType::StoreWood01:
		Result = _Level->CreateActor<StoreWood01>();
		break;
	case FieldMapObjType::ToLocation11:
		Result = _Level->CreateActor<LevelChangeZone>();
		Result->DynamicThis<LevelChangeZone>()->SetChangeLevel("Location11_Level");
		break;
	case FieldMapObjType::ToLocation19:
		Result = _Level->CreateActor<LevelChangeZone>();
		Result->DynamicThis<LevelChangeZone>()->SetChangeLevel("Location19_Level");
		break;
	case FieldMapObjType::l02_03_StreetLine:
		Result = _Level->CreateActor<l02_03_StreetLine>();
		break;
	case FieldMapObjType::FloatingIsland:
		Result = _Level->CreateActor<FloatingIsland>();
		break;
	case FieldMapObjType::FloatingIsland01:
		Result = _Level->CreateActor<FloatingIsland01>();
		break;
	case FieldMapObjType::FloatingIsland02:
		Result = _Level->CreateActor<FloatingIsland02>();
		break;
	case FieldMapObjType::tunnelWallLight:
		Result = _Level->CreateActor<tunnelWallLight>();
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


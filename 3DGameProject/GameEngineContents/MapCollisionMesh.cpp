#include "PrecompileHeader.h"
#include "MapCollisionMesh.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/PhysXTriangleComponent.h>

MapCollisionMesh::MapCollisionMesh()
{

}

MapCollisionMesh::~MapCollisionMesh()
{

}

std::shared_ptr<MapCollisionMesh> MapCollisionMesh::CreateMapCollisionMesh(GameEngineLevel* _Level, const std::string_view& _FBXName, bool _IsGround)
{
	std::shared_ptr<MapCollisionMesh> Result;
	Result = _Level->CreateActor<MapCollisionMesh>(); // 렌더러 생성
	Result->Renderer = Result->CreateComponent<GameEngineFBXRenderer>();
	Result->Renderer->SetName("Renderer");
	Result->Renderer->SetFBXMesh(_FBXName.data(), "FBX");
	Result->Renderer->ShadowOff();
	Result->Renderer->Off();

	float Ratio = 1.f;

	Result->Collision = Result->CreateComponent<PhysXTriangleComponent>(); // 콜라이더 생성
	Result->Collision->SetName("Collision");
	Result->Collision->SetPhysxMaterial(1.0f, 1.0f, 0.05f);
	
	if (_IsGround)
	{
		Result->Collision->SetGroundObject();
	}
	else
	{
		Result->Collision->SetWallObject();
	}

	Result->Collision->CreatePhysXActors(_FBXName.data(), true, Ratio);

	return Result;
}

std::shared_ptr<MapCollisionMesh> MapCollisionMesh::CreateGroundCollisionMesh(GameEngineLevel* _Level, const std::string_view& _FBXName)
{
	std::shared_ptr<MapCollisionMesh> Result;
	Result = CreateMapCollisionMesh(_Level, _FBXName, true);
	Result->SetName("GroundColMesh");
	return Result;
}

std::shared_ptr<MapCollisionMesh> MapCollisionMesh::CreateWallCollisionMesh(GameEngineLevel* _Level, const std::string_view& _FBXName)
{
	std::shared_ptr<MapCollisionMesh> Result;
	Result = CreateMapCollisionMesh(_Level, _FBXName, false);
	Result->SetName("WallColMesh");
	return Result;
}

void MapCollisionMesh::RenderOn()
{
	Renderer->On();
}

void MapCollisionMesh::RenderOff()
{
	Renderer->Off();
}

bool MapCollisionMesh::RenderIsUpdate()
{
	return Renderer->IsUpdate();
}
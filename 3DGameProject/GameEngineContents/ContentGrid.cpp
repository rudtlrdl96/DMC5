#include "PrecompileHeader.h"
#include "ContentGrid.h"

#include <GameEngineCore/GameEngineRenderer.h>
#include <GameEngineCore/PhysXBoxComponent.h>

ContentGrid::ContentGrid()
{

}

ContentGrid::~ContentGrid()
{

}

void ContentGrid::Start()
{
	EngineGrid::Start();
	physx::PxVec3 GeoMetryScale = { 10000, 1, 10000};
	PhysComponent = CreateComponent<PhysXBoxComponent>();
	PhysComponent->SetPhysxMaterial(0.5f, 0.5f, 0.5f);
	PhysComponent->CreatePhysXActors(GetLevel()->GetLevelScene(), GetLevel()->GetLevelPhysics(), GeoMetryScale);
	PhysComponent->SetPositionSetFromParentFlag(true);
}

void ContentGrid::Update(float _Delta)
{
	EngineGrid::Update(_Delta);
}

void ContentGrid::Render(float _Delta)
{
	EngineGrid::Render(_Delta);
}
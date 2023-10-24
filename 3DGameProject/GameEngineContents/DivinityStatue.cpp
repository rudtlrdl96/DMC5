#include "PrecompileHeader.h"
#include "DivinityStatue.h"
#include <GameEngineCore/GameEngineCollision.h>
DivinityStatue::DivinityStatue()
{
}

DivinityStatue::~DivinityStatue()
{
}

void DivinityStatue::Start()
{
	FBXFileName = "sm0994_statue.fbx";
	std::shared_ptr<GameEngineCollision> Collision = CreateComponent<GameEngineCollision>(CollisionOrder::Shop);
	Collision->GetTransform()->SetLocalPosition({ 0, 90, 0 });
	Collision->GetTransform()->SetLocalScale({ 300, 300, 300 });
	StaticFieldMapObject::Start();
}

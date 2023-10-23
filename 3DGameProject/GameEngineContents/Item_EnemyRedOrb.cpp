#include "PrecompileHeader.h"
#include "Item_EnemyRedOrb.h"
#include "Item_RedOrb.h"
Item_EnemyRedOrb::Item_EnemyRedOrb()
{
}

Item_EnemyRedOrb::~Item_EnemyRedOrb()
{
}

void Item_EnemyRedOrb::OrbOn()
{
	for (int i = 0; i < Orbs.size(); i++)
	{
		Orbs[i]->GetTransform()->SetParent(nullptr);
		Orbs[i]->On();
	}
}

void Item_EnemyRedOrb::Start()
{
	Orbs.resize(8);
	for (int i = 0; i < Orbs.size(); i++)
	{
		Orbs[i] = GetLevel()->CreateActor<Item_RedOrb>();
		Orbs[i]->SetWait();
		Orbs[i]->GetTransform()->SetParent(GetTransform());
		float Random = GameEngineRandom::MainRandom.RandomFloat(-1, 1);
		Orbs[i]->GetTransform()->SetLocalRotation(float4::UP * (static_cast<float>(45 * i) + Random * 40.0f));
	}
}

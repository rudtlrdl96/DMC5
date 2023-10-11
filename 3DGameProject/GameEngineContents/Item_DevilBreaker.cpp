#include "PrecompileHeader.h"
#include "Item_DevilBreaker.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
Item_DevilBreaker::Item_DevilBreaker() 
{
}

Item_DevilBreaker::~Item_DevilBreaker() 
{
}

void Item_DevilBreaker::SetDevilBreaker(DevilBreaker _Value)
{
	Type = _Value;
	ArmRenderer = CreateComponent<GameEngineFBXRenderer>();
	switch (_Value)
	{
	case DevilBreaker::None:
		break;
	case DevilBreaker::Overture:
		ArmRenderer->SetFBXMesh("OvertureArmUI.FBX", "FBX");
		break;
	case DevilBreaker::Gerbera:
		ArmRenderer->SetFBXMesh("GerberaArmUI.FBX", "FBX");
		break;
	case DevilBreaker::BusterArm:
		ArmRenderer->SetFBXMesh("BusterArmUI.FBX", "FBX");
		break;
	default:
		break;
	}
}

DevilBreaker Item_DevilBreaker::Take()
{
	Off();
	return Type;
}

void Item_DevilBreaker::Start()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Mesh");
	NewDir.Move("UIMesh");
	NewDir.Move("NeroItemMesh");

	if (nullptr == GameEngineFBXMesh::Find("NeroItemGlass.FBX"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}

	Col = CreateComponent<GameEngineCollision>(CollisionOrder::DevilBreaker);
	Col->GetTransform()->SetLocalScale({ 150, 150, 150 });
}

void Item_DevilBreaker::Update(float _DeltaTime)
{
}

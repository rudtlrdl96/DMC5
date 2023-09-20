#include "PrecompileHeader.h"
#include "NeroItemGlass.h"
#include "Nero_ItemFont.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include "UIFBXRenderer.h"

NeroItemGlass::NeroItemGlass()
{
}

NeroItemGlass::~NeroItemGlass()
{
}

void NeroItemGlass::Start()
{
	GameEngineDirectory NewDir;
	NewDir.MoveParentToDirectory("ContentResources");
	NewDir.Move("ContentResources");
	NewDir.Move("Mesh");
	NewDir.Move("UIMesh");
	NewDir.Move("NeroUIMesh");
	NewDir.Move("NeroItemMesh");

	if (nullptr == GameEngineFBXMesh::Find("NeroIden123.FBX"))
	{
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".fbx" });
		for (GameEngineFile File : Files)
		{
			GameEngineFBXMesh::Load(File.GetFullPath());
		}
	}

	NeroUI_ItemGlass = CreateComponent<UIFBXRenderer>(0);
	NeroUI_ItemGlass->SetFBXMesh("NeroItemGlass.FBX", "FBX_Low");
	NeroUI_ItemGlass->GetTransform()->SetLocalPosition({560.0f,-380.0f,172.0f });
	NeroUI_ItemGlass->GetTransform()->SetLocalRotation({ -90.0f,0.0f,0.0f });
	NeroUI_ItemGlass->GetTransform()->SetLocalScale({ 0.6f,0.6f,0.6f });
	NeroUI_ItemGlass->LightOff();

	std::shared_ptr<Nero_ItemFont> ItemText = GetLevel()->CreateActor<Nero_ItemFont>();
	ItemText->GetTransform()->SetLocalPosition({ 0.0f,-40.0f,0.0f });

}

void NeroItemGlass::Update(float _DeltaTime)
{

}


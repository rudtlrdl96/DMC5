#include "PrecompileHeader.h"
#include "NeroItemGlass.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineUIRenderer.h>
#include "UIFBXRenderer.h"
#include "UI_HPGaege.h"
#include "UI_DTGauge.h"
#include "UI_BulletGauge.h"
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



}

void NeroItemGlass::Update(float _DeltaTime)
{

}


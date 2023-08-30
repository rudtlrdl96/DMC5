#include "PrecompileHeader.h"
#include "ShaderTestActor.h"

ShaderTestActor::ShaderTestActor() 
{
}

ShaderTestActor::~ShaderTestActor() 
{
}

void ShaderTestActor::Start()
{
	TestRenderer = CreateComponent<GameEngineFBXRenderer>();

	//FBX파일경로를 찾아서 로드
	if (nullptr == GameEngineFBXMesh::Find("em0100.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Character", "Enemy", "em0100", "mesh"
			}, "em0100.FBX");

		GameEngineFBXMesh::Load(Path);
	}

	TestRenderer->SetFBXMesh("em0100.fbx", "FBX_DMC");

	//std::vector<std::vector<std::shared_ptr<GameEngineRenderUnit>>>&  AllUnit = TestRenderer->GetAllRenderUnit();
	//
	//for (size_t i = 0; i < AllUnit.size(); i++)
	//{
	//	for (size_t j = 0; j < AllUnit[i].size(); j++)
	//	{
	//		AllUnit[i][j]->ShaderResHelper.SetTexture("ReflectionTexture", GetLevel()->GetMainCamera()->GetReflectionTarget()->GetTexture(0));
	//	}
	//}

}


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
	if (nullptr == GameEngineFBXMesh::Find("shaderTest.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Mesh", "TestMesh"
			}, "shaderTest.FBX");

		GameEngineFBXMesh::Load(Path);
	}

	TestRenderer->SetFBXMesh("shaderTest.fbx", "FBX_DMC");
}

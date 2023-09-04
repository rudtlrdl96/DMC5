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
	if (nullptr == GameEngineTexture::Find("ShaderTest_Case_0_albm.png"))
	{
		std::string DirectoryPath = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Texture", "ShaderDebug"
			});

		GameEngineTexture::Load(DirectoryPath + "/ShaderTest_Case_0_albm.png");
		GameEngineTexture::Load(DirectoryPath + "/ShaderTest_Case_1_albm.png");
		GameEngineTexture::Load(DirectoryPath + "/ShaderTest_Case_2_albm.png");

		GameEngineTexture::Load(DirectoryPath + "/ShaderTest_Case_0_nrmr.png");
		GameEngineTexture::Load(DirectoryPath + "/ShaderTest_Case_1_nrmr.png");
		GameEngineTexture::Load(DirectoryPath + "/ShaderTest_Case_2_nrmr.png");

		GameEngineTexture::Load(DirectoryPath + "/ShaderTest_Case_0_atos.png");
	}

	TestRenderer = CreateComponent<GameEngineFBXRenderer>();

	//FBX���ϰ�θ� ã�Ƽ� �ε�
	if (nullptr == GameEngineFBXMesh::Find("shaderTest.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Mesh", "TestMesh"
			}, "shaderTest.FBX");

		GameEngineFBXMesh::Load(Path);
	}

	TestRenderer->SetFBXMesh("shaderTest.fbx", "FBX");

	TestRenderer->NormalOff();
}

void ShaderTestActor::InitTest(int _Index)
{	
	//Texture2D DiffuseTexture : register(t0); // ALBM
	//Texture2D NormalTexture : register(t1);  // NRMR
	//Texture2D SpecularTexture : register(t2); // ATOS

	if (0 == _Index)
	{
		// �����Ͻ� 0
		// ��Ż�� 0

		// albm = ShaderTest_Case_0_albm
		// albm = ShaderTest_Case_0_nrmr
		// atos = ShaderTest_Case_0_atos

		TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_0_albm.png");
		TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_0_nrmr.png");
		TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (1 == _Index)
	{
		// �����Ͻ� 0
		// ��Ż�� 0.5

		// albm = ShaderTest_Case_1_albm
		// albm = ShaderTest_Case_0_nrmr
		// atos = ShaderTest_Case_0_atos


		TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_1_albm.png");
		TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_0_nrmr.png");
		TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (2 == _Index)
	{
		// �����Ͻ� 1.0
		// ��Ż�� 1.0

		// albm = ShaderTest_Case_2_albm
		// albm = ShaderTest_Case_2_nrmr
		// atos = ShaderTest_Case_0_atos


		TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_2_albm.png");
		TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_2_nrmr.png");
		TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (3 == _Index)
	{

		// �����Ͻ� 1
		// ��Ż�� 0

		// albm = ShaderTest_Case_0_albm
		// albm = ShaderTest_Case_2_nrmr
		// atos = ShaderTest_Case_0_atos

		TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_0_albm.png");
		TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_2_nrmr.png");
		TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (4 == _Index)
	{
		// �����Ͻ� 0
		// ��Ż�� 1

		// albm = ShaderTest_Case_2_albm
		// albm = ShaderTest_Case_0_nrmr
		// atos = ShaderTest_Case_0_atos

		TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_2_albm.png");
		TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_0_nrmr.png");
		TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}


}

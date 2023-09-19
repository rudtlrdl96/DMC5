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
	//if (nullptr == GameEngineFBXMesh::Find("SkyBox.FBX"))
	//{
	//	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
	//		{
	//			"Map", "TestMap"
	//		}, "SkyBox.FBX");
	//
	//	GameEngineFBXMesh::Load(Path);
	//}
	
	TestRenderer->SetFBXMesh("Box_350x250x300_Mesh.fbx", "FBX_Alpha");
	TestRenderer->GetTransform()->SetLocalScale(float4(0.3f, 0.3f, 0.3f));

	TestRenderer->SetBaseColor(float4(3, 0, 0));
	TestRenderer->NormalOff();

	//TestRenderer = CreateComponent<EffectFBXRenderer>();

	std::shared_ptr<GameEngineLight> PointLight = GetLevel()->CreateSpotLight(GetTransform()->GetWorldPosition(), float4(1024, 1024), 1000.0f, 10.0f);
	PointLight->LightDataValue.LightColor = float4(1, 0, 0);
	PointLight->GetTransform()->SetLocalRotation(float4(-90, 0, 0));
	PointLight->GetTransform()->SetParent(GetTransform());

	if (nullptr == GameEngineFBXMesh::Find("Effect_Mesh_01.FBX"))
	{
		std::string MeshPath = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Effect", "Mesh"
			});		
		
		std::string TexturePath = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Effect", "Texture"
			});
	
		GameEngineTexture::Load(TexturePath + "\\Effect_Texture_01.tga");
		GameEngineTexture::Load(TexturePath + "\\Effect_Texture_02.tga");
		GameEngineTexture::Load(TexturePath + "\\Effect_Texture_03.tga");
		GameEngineTexture::Load(TexturePath + "\\Effect_Texture_04.tga");
	
		GameEngineFBXMesh::Load(MeshPath + "\\Effect_Mesh_01.FBX");
	}

	InitTest(0);
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

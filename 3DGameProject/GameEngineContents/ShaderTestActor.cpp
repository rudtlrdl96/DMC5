#include "PrecompileHeader.h"
#include "ShaderTestActor.h"

ShaderTestActor::ShaderTestActor() 
{
	SetName("ShaderTestActor");
}

ShaderTestActor::~ShaderTestActor() 
{
}

void ShaderTestActor::Start()
{
	//if (nullptr == GameEngineTexture::Find("PaperBurnNoise.jpg"))
	//{
	//	GameEngineTexture::Load(GameEnginePath::GetFileFullPath("ContentResources",
	//		{
	//			"Texture", "BurnNoiseTexture"
	//		}, "PaperBurnNoise.jpg"));
	//}

	TestRenderer = CreateComponent<UIMeshRenderer>();
	 	
	//FBX파일경로를 찾아서 로드
	if (nullptr == GameEngineFBXMesh::Find("House1.FBX"))
	{
		std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Map", "TestMap"
			}, "House1.FBX");
	
		GameEngineFBXMesh::Load(Path);
	}

	// Light 데이터 설정
	TestRenderer->LightDataValue.LightPos = float4(0, 1000, 0);
	TestRenderer->LightDataValue.LightDir = float4(-1, -1, 0).NormalizeReturn();
	TestRenderer->LightDataUpdate();
	
	TestRenderer->SetFBXMesh("House1.FBX", "UIFBXMesh");
	TestRenderer->GetTransform()->SetLocalScale(float4(0.5f, 0.5f, 0.5f));

	//TestRenderer->SetTexture("PaperBurnTexture", "PaperBurnNoise.jpg");

	//TestRenderer->EffectOption.MulColor = float4(1, 1, 1, 0.5f);

	//TestRenderer->SetBaseColor(float4(3, 0, 0));
	//TestRenderer->ShadowOn();
	//TestRenderer->NormalOff();

	//TestRenderer = CreateComponent<EffectRenderer>();

	//if (nullptr == GameEngineFBXMesh::Find("Effect_Mesh_01.FBX"))
	//{
	//	std::string MeshPath = GameEnginePath::GetFileFullPath("ContentResources",
	//		{
	//			"Effect", "Mesh"
	//		});		
	//	
	//	std::string TexturePath = GameEnginePath::GetFileFullPath("ContentResources",
	//		{
	//			"Effect", "Texture"
	//		});
	//
	//	GameEngineTexture::Load(TexturePath + "\\Effect_Texture_01.tga");
	//	GameEngineTexture::Load(TexturePath + "\\Effect_Texture_02.tga");
	//	GameEngineTexture::Load(TexturePath + "\\Effect_Texture_03.tga");
	//	GameEngineTexture::Load(TexturePath + "\\Effect_Texture_04.tga");
	//
	//	GameEngineFBXMesh::Load(MeshPath + "\\Effect_Mesh_01.FBX");
	//}

	//InitTest(0);
}

void ShaderTestActor::InitTest(int _Index)
{	
	//Texture2D DiffuseTexture : register(t0); // ALBM
	//Texture2D NormalTexture : register(t1);  // NRMR
	//Texture2D SpecularTexture : register(t2); // ATOS

	if (0 == _Index)
	{
		// 러프니스 0
		// 메탈릭 0

		// albm = ShaderTest_Case_0_albm
		// albm = ShaderTest_Case_0_nrmr
		// atos = ShaderTest_Case_0_atos

		TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_0_albm.png");
		TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_0_nrmr.png");
		TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (1 == _Index)
	{
		// 러프니스 0
		// 메탈릭 0.5

		// albm = ShaderTest_Case_1_albm
		// albm = ShaderTest_Case_0_nrmr
		// atos = ShaderTest_Case_0_atos


		TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_1_albm.png");
		TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_0_nrmr.png");
		TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (2 == _Index)
	{
		// 러프니스 1.0
		// 메탈릭 1.0

		// albm = ShaderTest_Case_2_albm
		// albm = ShaderTest_Case_2_nrmr
		// atos = ShaderTest_Case_0_atos


		TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_2_albm.png");
		TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_2_nrmr.png");
		TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (3 == _Index)
	{

		// 러프니스 1
		// 메탈릭 0

		// albm = ShaderTest_Case_0_albm
		// albm = ShaderTest_Case_2_nrmr
		// atos = ShaderTest_Case_0_atos

		TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_0_albm.png");
		TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_2_nrmr.png");
		TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (4 == _Index)
	{
		// 러프니스 0
		// 메탈릭 1

		// albm = ShaderTest_Case_2_albm
		// albm = ShaderTest_Case_0_nrmr
		// atos = ShaderTest_Case_0_atos

		TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_2_albm.png");
		TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_0_nrmr.png");
		TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}


}

#include <GameEngineCore/imgui.h>

void ShaderTestActor::DrawEditor()
{
	float4 BC = TestRenderer->GetBaseColor();
	float InputColor[4] = {BC.r, BC.g, BC.b, BC.a};
	ImGui::DragFloat4("BaseColor", InputColor, 0.01f);
	TestRenderer->SetBaseColor({ InputColor[0], InputColor[1], InputColor[2], InputColor[3]});
}
#include "PrecompileHeader.h"
#include "ShaderTestActor.h"
#include <GameEngineCore/GameEngineCaptureTexture.h>

ShaderTestActor::ShaderTestActor() 
{
	SetName("ShaderTestActor");
}

ShaderTestActor::~ShaderTestActor() 
{
}

void ShaderTestActor::Start()
{
	if (nullptr == GameEngineTexture::Find("DistortionSample_02.jpg"))
	{
		GameEngineTexture::Load(GameEnginePath::GetFileFullPath("ContentResources",
			{
				"Texture", "DistortionTexture"
			}, "DistortionSample_02.jpg"));
	}

	// ����� ����Ʈ 
	TestRenderer = CreateComponent<EffectRenderer>();
	TestRenderer->RectInit("Effect_2D");
	TestRenderer->SetTexture("DiffuseTexture", "EngineBaseNormal.png");

	TestRenderer->GetTransform()->SetLocalScale({ 500, 1000, 1 });
	TestRenderer->EffectOption.MulColor = {0.5, 0, 1, 0.2f};
	TestRenderer->SetDistortionTexture("noise_02.png");
	TestRenderer->PlayDistortionAnimation({4.0f, 2.0f}, 8.0f, 8.0f);

	//if (false == GameEngineInput::IsKey("Capture_Self"))
	//{
	//	GameEngineInput::CreateKey("Capture_Self", VK_INSERT);
	//	GameEngineInput::CreateKey("Capture_Core", VK_HOME);
	//	GameEngineInput::CreateKey("SetTexture_Self", VK_DELETE);
	//	GameEngineInput::CreateKey("SetTexture_Core", VK_END);
	//}

	//FBX���ϰ�θ� ã�Ƽ� �ε�
	//if (nullptr == GameEngineFBXMesh::Find("House1.FBX"))
	//{
	//	std::string Path = GameEnginePath::GetFileFullPath("ContentResources",
	//		{
	//			"Map", "TestMap"
	//		}, "House1.FBX");
	//
	//	GameEngineFBXMesh::Load(Path);
	//}

	// Light ������ ����
	//TestRenderer->LightDataValue.LightPos = float4(0, 1000, 0);
	//TestRenderer->LightDataValue.LightDir = float4(-1, -1, 0).NormalizeReturn();
	//TestRenderer->LightDataUpdate();
	
	//TestRenderer->SetFBXMesh("House1.FBX", "FBX");
	//TestRenderer->GetTransform()->SetLocalScale(float4(5.0f, 5.0f, 5.0f));
	//TestRenderer->SetBaseColor({ 20.0f, 0.0f, 0.0f });

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

void ShaderTestActor::Update(float _DeltaTime)
{
	//GameEngineInput::CreateKey("Capture_Self", VK_HOME);
	//GameEngineInput::CreateKey("Capture_Core", VK_END);
	//GameEngineInput::CreateKey("SetTexture_Self", VK_INSERT);
	//GameEngineInput::CreateKey("SetTexture_Core", VK_DELETE);

	//if (true == GameEngineInput::IsDown("Capture_Self"))
	//{
	//	// ȣ���ϴ� ��� ĸ�ĸ� �����մϴ� �̹� �ߺ��� �̸��� ������ ��� �ش� �ؽ��ķ� ĸ���մϴ�
	//	GameEngineCaptureTexture::CaptureTexture("Capture_Self", {1600, 900}, GetLevel()->GetMainCamera()->GetCamTarget());
	//}
	//
	//if (true == GameEngineInput::IsDown("Capture_Core"))
	//{
	//	// �������� ��� ���� �� ĸ�ĸ� �����մϴ� �̹� �ߺ��� �̸��� ������ ��� �ش� �ؽ��ķ� ĸ���մϴ�
	//	GameEngineCore::CaptureTexture("Capture_Core", GetLevel()->GetMainCamera()->GetCamTarget(), {1600, 900});
	//}	
	//
	//if (true == GameEngineInput::IsDown("SetTexture_Self"))
	//{
	//	// ĸ�ĵ� �ؽ��ĸ� ��ȯ�մϴ� �ؽ��İ� �������� ���� ��� nullptr�� ��ȯ�մϴ�.
	//	std::shared_ptr<GameEngineRenderTarget> CaptureTarget = GameEngineCaptureTexture::FindTexture("Capture_Self");
	//
	//	if (nullptr != CaptureTarget)
	//	{
	//		TestRenderer->SetTexture(CaptureTarget->GetTexture(0));
	//	}
	//}

	// ĸ�ĵ� �ؽ��ĸ� �����մϴ�, �޸� ������ ���� �Ⱦ��� �ؽ��ĸ� ������ �ּ���
	//GameEngineCaptureTexture::ReleaseTexture("Capture_Self");
	//GameEngineCaptureTexture::ReleaseTexture("Capture_Core");

	// ĸ�ĵ� ��� �ؽ��ĸ� �����մϴ�, �޸� ������ ���� �Ⱦ��� �ؽ��ĸ� ������ �ּ���
	//GameEngineCaptureTexture::ClearAllTexture();

	//if (true == GameEngineInput::IsDown("SetTexture_Core"))
	//{
	//	// ĸ�ĵ� �ؽ��ĸ� ��ȯ�մϴ� �ؽ��İ� �������� ���� ��� nullptr�� ��ȯ�մϴ�.
	//	std::shared_ptr<GameEngineRenderTarget> CaptureTarget = GameEngineCaptureTexture::FindTexture("Capture_Core");
	//
	//	if (nullptr != CaptureTarget)
	//	{
	//		TestRenderer->SetTexture(CaptureTarget->GetTexture(0));
	//	}
	//}
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

		//TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_0_albm.png");
		//TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_0_nrmr.png");
		//TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (1 == _Index)
	{
		// �����Ͻ� 0
		// ��Ż�� 0.5

		// albm = ShaderTest_Case_1_albm
		// albm = ShaderTest_Case_0_nrmr
		// atos = ShaderTest_Case_0_atos


		//TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_1_albm.png");
		//TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_0_nrmr.png");
		//TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (2 == _Index)
	{
		// �����Ͻ� 1.0
		// ��Ż�� 1.0

		// albm = ShaderTest_Case_2_albm
		// albm = ShaderTest_Case_2_nrmr
		// atos = ShaderTest_Case_0_atos


		//TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_2_albm.png");
		//TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_2_nrmr.png");
		//TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (3 == _Index)
	{

		// �����Ͻ� 1
		// ��Ż�� 0

		// albm = ShaderTest_Case_0_albm
		// albm = ShaderTest_Case_2_nrmr
		// atos = ShaderTest_Case_0_atos

		//TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_0_albm.png");
		//TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_2_nrmr.png");
		//TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
	}
	else if (4 == _Index)
	{
		// �����Ͻ� 0
		// ��Ż�� 1

		// albm = ShaderTest_Case_2_albm
		// albm = ShaderTest_Case_0_nrmr
		// atos = ShaderTest_Case_0_atos

		//TestRenderer->SetTexture("DiffuseTexture", "ShaderTest_Case_2_albm.png");
		//TestRenderer->SetTexture("NormalTexture", "ShaderTest_Case_0_nrmr.png");
		//TestRenderer->SetTexture("SpecularTexture", "ShaderTest_Case_0_atos.png");
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
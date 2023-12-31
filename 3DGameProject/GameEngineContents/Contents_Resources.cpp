#include "PrecompileHeader.h"
#include "ContentsCore.h"

#include <GameEngineCore/GameEngineMaterial.h>
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineCore/GameEnginePixelShader.h>
#include <GameEngineCore/GameEngineVertexShader.h>
#include <GameEngineCore/GameEngineRenderingPipeLine.h>
#include <GameEngineCore/GameEngineBlend.h>
#include <GameEngineCore/GameEngineFBXMesh.h>
#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineGUI.h>
#include <GameEngineCore/HierarchyGUI.h>
#include <GameEngineCore/GameEngineFont.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "NetworkGUI.h"
#include "ConsoleGUI.h"
#include "StageEditGUI.h"
#include "FXSystem.h"
void ContentsCore::ContentsResourcesCreate()
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////// Create ImGUI /////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// GameEngineGUI::GUIWindowCreate<GameEngineCoreWindow>("CoreWindow");
	GameEngineGUI::GUIWindowCreate<NetworkGUI>("NetworkGUI");
	//GameEngineGUI::GUIWindowCreate<ConsoleGUI>("ConsoleGUI");
	GameEngineGUI::GUIWindowCreate<HierarchyGUI>("HierarchyGUI");
	GameEngineGUI::GUIWindowCreate<GameEngineCoreWindow>("CoreWindow");

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////// File Load //////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FontUpload();
	GameEngineFont::Load("궁서");

	// 컨텐츠 쉐이더 파일 로드
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("ContentsShader");

		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".hlsl", ".fx" });


		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineShader::AutoCompile(Files[i]);
		}
	}

	// 반사 텍스쳐 로드
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Texture");
		NewDir.Move("ReflectionTexture");
		// 테스트 메쉬 폴더는 자동으로 로드합니다
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".PNG" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineTexture::Load(Files[i].GetFullPath());
		}
	}

	//컨텐츠 FBX 파일 로드 (임시)
	{
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Mesh");
		NewDir.Move("TestMesh");
		// 테스트 메쉬 폴더는 자동으로 로드합니다
		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".FBX" });
	
		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineFBXMesh::Load(Files[i].GetFullPath());
		}
	}

	// Map
	//{
	//	GameEngineDirectory NewDir;
	//	NewDir.MoveParentToDirectory("ContentResources");
	//	NewDir.Move("ContentResources");
	//	NewDir.Move("Map");
	//	std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".FBX" });

	//	for (size_t i = 0; i < Files.size(); i++)
	//	{
	//		GameEngineFBXMesh::Load(Files[i].GetFullPath());
	//	}
	//}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////// Create Sempler & Blend ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	{
		D3D11_SAMPLER_DESC SamperData = {};

		SamperData.Filter = D3D11_FILTER_ANISOTROPIC;
		SamperData.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamperData.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamperData.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		SamperData.MipLODBias = 0.0f;
		SamperData.MaxAnisotropy = 16;
		SamperData.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamperData.MinLOD = -FLT_MAX;
		SamperData.MaxLOD = FLT_MAX;
		//SamperData.

		GameEngineSampler::ReSetting("ENGINEBASE", SamperData);
	}

	{
		D3D11_SAMPLER_DESC SamperData = {};

		SamperData.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		SamperData.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		SamperData.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		SamperData.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		SamperData.MipLODBias = 0.0f;
		SamperData.MaxAnisotropy = 1;
		SamperData.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		SamperData.MinLOD = -FLT_MAX;
		SamperData.MaxLOD = FLT_MAX;

		GameEngineSampler::Create("WRAP", SamperData);
	}

	{
		D3D11_BLEND_DESC Desc = { 0, };

		Desc.AlphaToCoverageEnable = false;
		Desc.IndependentBlendEnable = false;
		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		GameEngineBlend::Create("OldFilm", Desc);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////// Create PipeLine ///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 2D Sprite
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("My2DTexture");

		Pipe->SetVertexShader("MyShader.fx");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("MyShader.fx");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("EngineDepth");
	}

	// PostEffect
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("Fade");

		Pipe->SetVertexShader("FadeShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("FadeShader.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("EngineDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("OldFilm");

		Pipe->SetVertexShader("OldFilmShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("OldFilmShader.hlsl");
		Pipe->SetBlendState("OldFilm");
		Pipe->SetDepthState("EngineDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("OldTV");

		Pipe->SetVertexShader("OldTVShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("OldTVShader.hlsl");
		Pipe->SetBlendState("OldFilm");
		Pipe->SetDepthState("EngineDepth");
	}	

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("JudgementCut");

		Pipe->SetVertexShader("JudgementCutShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("JudgementCutShader.hlsl");
		Pipe->SetBlendState("MergeBlend");
		Pipe->SetDepthState("EngineDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("ColorEffect");

		Pipe->SetVertexShader("ColorEffectShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("ColorEffectShader.hlsl");
		Pipe->SetBlendState("MergeBlend");
		Pipe->SetDepthState("EngineDepth");
	}		
	
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("BWColorEffect");

		Pipe->SetVertexShader("BWColorShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("BWColorShader.hlsl");
		Pipe->SetBlendState("MergeBlend");
		Pipe->SetDepthState("EngineDepth");
	}	
	
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("FXAA");

		Pipe->SetVertexShader("FXAA.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("FXAA.hlsl");
		Pipe->SetBlendState("MergeBlend");
		Pipe->SetDepthState("EngineDepth");
	}	

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("ZoomEffect");

		Pipe->SetVertexShader("ZoomEffectShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("ZoomEffectShader.hlsl");
		Pipe->SetBlendState("MergeBlend");
		Pipe->SetDepthState("EngineDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("DebugRect");

		Pipe->SetVertexShader("DebugMeshRender.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("DebugMeshRender.hlsl");
		Pipe->SetBlendState("BaseBlend");
		Pipe->SetDepthState("EngineDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("Distortion");

		Pipe->SetVertexShader("DistortionEffectShader.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("DistortionEffectShader.hlsl");
		Pipe->SetBlendState("MergeBlend");
		Pipe->SetDepthState("EngineDepth");
	}	
	
	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("MotionBlur");

		Pipe->SetVertexShader("MotionBlurEffect.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("MotionBlurEffect.hlsl");
		Pipe->SetBlendState("MergeBlend");
		Pipe->SetDepthState("EngineDepth");
	}	

	{
		std::shared_ptr<GameEngineMaterial> Pipe = GameEngineMaterial::Create("HDREffect");

		Pipe->SetVertexShader("HDREffect.hlsl");
		Pipe->SetRasterizer("Engine2DBase");
		Pipe->SetPixelShader("HDREffect.hlsl");
		Pipe->SetBlendState("MergeBlend");
		Pipe->SetDepthState("EngineDepth");
	}

	// FBX
	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("FBX");

		Material->SetVertexShader("ContentsMesh.hlsl");				
		Material->SetRasterizer("Engine3DBase");
		Material->SetPixelShader("ContentsMesh.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("EngineDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("FBX_Alpha");

		Material->SetVertexShader("ContentsMesh_Alpha.hlsl");
		Material->SetRasterizer("Engine3DBase");
		Material->SetPixelShader("ContentsMesh_Alpha.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("AlphaDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("FBX_2DAlpha");

		Material->SetVertexShader("ContentsMesh_Alpha.hlsl");
		Material->SetRasterizer("Engine2DBase");
		Material->SetPixelShader("ContentsMesh_Alpha.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("AlphaDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("FBX_ClipAlpha");

		Material->SetVertexShader("ContentsMesh_ClipAlpha.hlsl");
		Material->SetRasterizer("Engine3DBase");
		Material->SetPixelShader("ContentsMesh_ClipAlpha.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("AlphaDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("FBX_Low");

		Material->SetVertexShader("ContentsMesh_LowOption.hlsl");
		Material->SetRasterizer("Engine2DBase");
		Material->SetPixelShader("ContentsMesh_LowOption.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("EngineDepth");
	}	
	
	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("FBX_2D_Low");

		Material->SetVertexShader("ContentsMesh_LowOption.hlsl");
		Material->SetRasterizer("Engine2DBase");
		Material->SetPixelShader("ContentsMesh_LowOption.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("EngineDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("FBX_2D");

		Material->SetVertexShader("ContentsMesh.hlsl");
		Material->SetRasterizer("Engine2DBase");
		Material->SetPixelShader("ContentsMesh.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("EngineDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("AniFBX");

		Material->SetVertexShader("ContentsAniMesh.hlsl");
		Material->SetRasterizer("Engine2DBase");
		Material->SetPixelShader("ContentsAniMesh.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("EngineDepth");
	}	
	
	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("AniFBX_Alpha");

		Material->SetVertexShader("ContentsAniMesh_Alpha.hlsl");
		Material->SetRasterizer("Engine3DBase");
		Material->SetPixelShader("ContentsAniMesh_Alpha.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("AlphaDepth");
	}	
	
	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("AniFBX_2DAlpha");

		Material->SetVertexShader("ContentsAniMesh_Alpha.hlsl");
		Material->SetRasterizer("Engine2DBase");
		Material->SetPixelShader("ContentsAniMesh_Alpha.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("AlphaDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("AniFBX_Low");

		Material->SetVertexShader("ContentsAniMesh_LowOption.hlsl");
		Material->SetRasterizer("Engine2DBase");
		Material->SetPixelShader("ContentsAniMesh_LowOption.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("EngineDepth");
	}		
	
	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("Empusa");

		Material->SetVertexShader("EmpusaAniMesh.hlsl");
		Material->SetRasterizer("Engine2DBase");
		Material->SetPixelShader("EmpusaAniMesh.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("EngineDepth");
	}	

	// 3D Effect
	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("Effect_2D");

		Material->SetVertexShader("EffectMesh.hlsl");
		Material->SetRasterizer("Engine2DBase");
		Material->SetPixelShader("EffectMesh.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("AlphaDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("Effect_3D");

		Material->SetVertexShader("EffectMesh.hlsl");
		Material->SetRasterizer("Engine3DBase");
		Material->SetPixelShader("EffectMesh.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("AlphaDepth");
	}

	// UIMesh
	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("UIFBXMesh");

		Material->SetVertexShader("ContentsUIMesh.hlsl");
		Material->SetRasterizer("Engine3DBase");
		Material->SetPixelShader("ContentsUIMesh.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("EngineDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Material = GameEngineMaterial::Create("UIFBXAniMesh");

		Material->SetVertexShader("ContentsUIAniMesh.hlsl");
		Material->SetRasterizer("Engine3DBase");
		Material->SetPixelShader("ContentsUIAniMesh.hlsl");
		Material->SetBlendState("BaseBlend");
		Material->SetDepthState("EngineDepth");
	}
}

void ContentsCore::CreateContentsKey()
{
	if (false == GameEngineInput::IsKey("SelectLevel_01"))
	{
		GameEngineInput::CreateKey("SelectLevel_01", '1');
		GameEngineInput::CreateKey("SelectLevel_02", '2');
		GameEngineInput::CreateKey("SelectLevel_03", '3');
		GameEngineInput::CreateKey("SelectLevel_04", '4');
		GameEngineInput::CreateKey("SelectLevel_05", '5');
		GameEngineInput::CreateKey("SelectLevel_06", '6');
		GameEngineInput::CreateKey("SelectLevel_07", '7');
		GameEngineInput::CreateKey("SelectLevel_08", '8');
		GameEngineInput::CreateKey("SelectLevel_09", '9');
		GameEngineInput::CreateKey("ReturnToMainLevel", '0');
	}
	if (false == GameEngineInput::IsKey("UIDEBUGMODE"))
	{
		GameEngineInput::CreateKey("UIDEBUGMODE", 'U');
		GameEngineInput::CreateKey("ENTER", VK_RETURN);
		GameEngineInput::CreateKey("UI_CLICK", VK_LBUTTON);
		GameEngineInput::CreateKey("UI_UP", VK_UP);
		GameEngineInput::CreateKey("UI_Down", VK_DOWN);
		GameEngineInput::CreateKey("UI_Enter", VK_RETURN);
		GameEngineInput::CreateKey("UI_Left", VK_LEFT);
		GameEngineInput::CreateKey("UI_Right", VK_RIGHT);
		GameEngineInput::CreateKey("UI_Tab", VK_TAB);
		GameEngineInput::CreateKey("UI_bullet", VK_RBUTTON);
		GameEngineInput::CreateKey("UI_ESC", VK_ESCAPE);
		GameEngineInput::CreateKey("UI_OrbCheat", VK_F7);
		GameEngineInput::CreateKey("UI_KeyBordDebug", VK_F6);

	}
	if (false == GameEngineInput::IsKey("Player_Up"))
	{
		GameEngineInput::CreateKey("Escape", VK_ESCAPE);
		GameEngineInput::CreateKey("Player_Up", 'W');
		GameEngineInput::CreateKey("Player_Left", 'A');
		GameEngineInput::CreateKey("Player_Down", 'S');
		GameEngineInput::CreateKey("Player_Right", 'D');
		GameEngineInput::CreateKey("Player_Sword", 'I');
		GameEngineInput::CreateKey("Player_Gun", 'J');
		GameEngineInput::CreateKey("Player_Jump", 'K');
		GameEngineInput::CreateKey("Player_Jump2", VK_CONTROL);
		GameEngineInput::CreateKey("Player_Skill", 'L');
		GameEngineInput::CreateKey("Player_Skill2", VK_XBUTTON1);
		GameEngineInput::CreateKey("Player_Exceed", 'Q');
		GameEngineInput::CreateKey("Player_GT_Bomb", 'E');
		GameEngineInput::CreateKey("Player_Provocation", 'R');
		GameEngineInput::CreateKey("Player_LockOn", VK_SPACE);
		GameEngineInput::CreateKey("Player_DT", VK_LSHIFT);
		GameEngineInput::CreateKey("Player_CameraLeft", 'U');
		GameEngineInput::CreateKey("Player_CameraRight", 'O');
		GameEngineInput::CreateKey("Player_CameraUp", 'Y');
		GameEngineInput::CreateKey("Player_CameraDown", 'P');
	}


	if (GameEngineInput::IsKey("FreeCam_SpeedUp") == false)
	{
		GameEngineInput::CreateKey("FreeCam_SpeedUp", VK_UP);
		GameEngineInput::CreateKey("FreeCam_SpeedDown", VK_DOWN);

		GameEngineInput::CreateKey("FreeCam_LookLeft", VK_NUMPAD4);
		GameEngineInput::CreateKey("FreeCam_LookRight", VK_NUMPAD6);
		GameEngineInput::CreateKey("FreeCam_LookUp", VK_NUMPAD8);
		GameEngineInput::CreateKey("FreeCam_LookDown", VK_NUMPAD2);
		GameEngineInput::CreateKey("FreeCam_LookBack", VK_NUMPAD5);
		GameEngineInput::CreateKey("FreeCam_LookFront", VK_NUMPAD0);
	}

	if (GameEngineInput::IsKey("Hierarchy_Position") == false)
	{
		GameEngineInput::CreateKey("Hierarchy_Position", 'G');
		GameEngineInput::CreateKey("Hierarchy_Rotation", 'R');
		GameEngineInput::CreateKey("Hierarchy_Scale", 'F');
	}
}

void ContentsCore::EffectTextureLoad()
{
	FXSystem::EffectTextureLoad();
}

void ContentsCore::SetOption()
{
	//GameEngineOption::ChangeOption("Texture", GameEngineOptionValue::High);
	//GameEngineOption::ChangeOption("Shader", GameEngineOptionValue::Low);

	GameEngineOption::ChangeOption("Texture", GameEngineOptionValue::High);
	GameEngineOption::ChangeOption("Shader", GameEngineOptionValue::High);
	
	// PostEffect 옵션 생성
	GameEngineOption::ChangeOption("PostEffect", GameEngineOptionValue::High);

#ifdef _DEBUG
	GameEngineOption::ChangeOption("DynamicShadow", GameEngineOptionValue::None);
#else
	GameEngineOption::ChangeOption("DynamicShadow", GameEngineOptionValue::Low);
#endif // _DEBUG
}
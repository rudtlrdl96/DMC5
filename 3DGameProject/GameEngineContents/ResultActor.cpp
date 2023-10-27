#include "PrecompileHeader.h"
#include "ResultActor.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
ResultActor::ResultActor()
{
}

ResultActor::~ResultActor()
{
}

void ResultActor::Start()
{
	GetTransform()->SetLocalPosition(float4::FORWARD * 450.0f * GameEngineActor::ScreenRatio.y);
	GetTransform()->SetLocalScale(GameEngineActor::ScreenRatio);
	{
		ScreenShotRenderer = CreateComponent<GameEngineFBXRenderer>();
		ScreenShotRenderer->GetTransform()->SetLocalScale({ 16, 9 });
		ScreenShotRenderer->SetFBXMesh("shape_plane.fbx", "FBX");
		ScreenShotRenderer->SetTexture("DiffuseTexture", "Result_ScreenShot.jpg");
		//Unit->ShaderResHelper.SetTexture("NormalTexture", "gun_center_nm.texout.tga");
		ScreenShotRenderer->SetTexture("NormalTexture", "normalside_nm.texout.tga");
		//ScreenShotRenderer->SetTexture("NormalTexture", "gun_center_nm.texout.tga");
		//ScreenShotRenderer->SetTexture("SpecularTexture", "EngineBaseSpecular.tga");
		ScreenShotRenderer->GetRenderBaseValueRef().IsNormal = 1;
	}

	{
		GlassRenderer = CreateComponent<GameEngineFBXRenderer>();
		GlassRenderer->GetTransform()->SetLocalScale({ 9, 9 });
		GlassRenderer->GetTransform()->SetLocalPosition({ 0, 0, -1 });
		GlassRenderer->SetFBXMesh("shape_plane.fbx", "FBX_Alpha");
		GlassRenderer->SetTexture("DiffuseTexture", "gun_center_im.texout.tga");
		//GlassRenderer->SetTexture("NormalTexture", "gun_center_nm.texout.tga");
		GlassRenderer->SetTexture("SpecularTexture", "gun_center_atos.texout.tga");
		ScreenShotRenderer->GetRenderBaseValueRef().IsNormal = 1;
		ScreenShotRenderer->GetRenderBaseValueRef().IsSpecular = 1;
	}
	//GlassRenderer->Off();
}

void ResultActor::Update(float _DeltaTime)
{
}

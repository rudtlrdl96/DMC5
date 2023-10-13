#include "PrecompileHeader.h"
#include "RedSeal.h"
#include "EffectRenderer.h"
#include "StageBaseLevel.h"

RedSeal::RedSeal()
{

}

RedSeal::~RedSeal()
{

}

void RedSeal::Start()
{
	FBXFileName = "sm7001_seal_02.fbx";

	std::shared_ptr<EffectRenderer> EffectMesh = CreateComponent<EffectRenderer>();
	FBXMesh = EffectMesh;
	EffectMesh->SetFBXMesh(FBXFileName.data(), "Effect_3D");
	EffectMesh->SetDistortionTexture();
}

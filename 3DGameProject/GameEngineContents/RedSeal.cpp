#include "PrecompileHeader.h"
#include "RedSeal.h"
#include "EffectRenderer.h"

RedSeal::RedSeal()
{

}

RedSeal::~RedSeal()
{

}

void RedSeal::Start()
{
	FBXFileName = "sm7001_seal_02_Test.fbx";

	std::shared_ptr<EffectRenderer> EffectMesh = CreateComponent<EffectRenderer>();
	FBXMesh = EffectMesh;
	EffectMesh->SetFBXMesh(FBXFileName.data(), "Effect_3D");
	EffectMesh->SetDistortionTexture();
}

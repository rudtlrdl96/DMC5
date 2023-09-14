#include "PrecompileHeader.h"
#include "Fountain.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>

Fountain::Fountain()
{

}

Fountain::~Fountain()
{

}

void Fountain::Start()
{
	FBXMesh = CreateComponent<GameEngineFBXRenderer>();
	MapObj_SetFBXMesh("sm0041_fountain01.fbx", "FBX_Low");
}

void Fountain::Update(float _DeltaTime)
{
}

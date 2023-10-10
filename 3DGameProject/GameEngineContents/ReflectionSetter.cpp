#include "PrecompileHeader.h"
#include "ReflectionSetter.h"
#include <GameEngineCore/GameEngineCollision.h>

ReflectionSetter::ReflectionSetter()
{

}

ReflectionSetter::~ReflectionSetter()
{

}

void ReflectionSetter::Start()
{
	Probe = CreateComponent<ReflectionProbe>();
	SetDebugRender();
}

void ReflectionSetter::Update(float _DeltaTime)
{
	DrawDebugRender(_DeltaTime);
}

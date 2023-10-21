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
}

void ReflectionSetter::Update(float _DeltaTime)
{
}

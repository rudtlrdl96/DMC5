#include "PrecompileHeader.h"
#include "ReflectionSetter.h"

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

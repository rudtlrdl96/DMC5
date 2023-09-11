#include "PrecompileHeader.h"
#include "testarea.h"
#include <GameEngineCore/GameEngineUIRenderer.h>
testarea::testarea() 
{
}

testarea::~testarea() 
{
}

void testarea::Start()
{
	testaaa = CreateComponent<GameEngineUIRenderer>();
	testaaa->SetScaleToTexture("testarea.png");

}


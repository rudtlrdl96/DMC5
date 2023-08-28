#include "PrecompileHeader.h"
#include "ShaderTestLevel.h"
#include "TestObject.h"
#include "Plane.h"

#include "JudgementCut.h"
#include "ColorEffect.h"

ShaderTestLevel::ShaderTestLevel() 
{
}

ShaderTestLevel::~ShaderTestLevel() 
{
}

void ShaderTestLevel::Start()
{
	GetCamera(0)->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 50.0f, -100.0f });

	Effect = GetCamera(0)->GetCamTarget()->CreateEffect<JudgementCut>();
}

void ShaderTestLevel::LevelChangeStart()
{
	std::shared_ptr<TestObject> NewTestObject = CreateActor<TestObject>();
	std::shared_ptr<Plane> Flat = CreateActor<Plane>();
}


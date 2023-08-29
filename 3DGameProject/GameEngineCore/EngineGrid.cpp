#include "PrecompileHeader.h"
#include "EngineGrid.h"
#include "GameEngineLevel.h"

EngineGrid::EngineGrid()
{
}

EngineGrid::~EngineGrid()
{
}

void EngineGrid::Start()
{
	std::shared_ptr<GameEngineRenderer> Ptr = CreateComponent<GameEngineRenderer>();
	Ptr->SetMesh("GridRect");
	Ptr->SetMaterial("GridShader");
	Ptr->GetShaderResHelper().SetConstantBufferLink("GridData", GridDataInst);


	GridDataInst.GridScale = MinGridInter;

	GetTransform()->SetLocalScale({ 10000.0f, 1.0f, 10000.0f });
}

void EngineGrid::Update(float _Delta)
{
	float4 CameraPos = GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition();
	float Y = abs(CameraPos.y);
	float ScalePlus = Y * 10.0f;

	GetTransform()->SetLocalScale({ 10000.0f + ScalePlus, 1.0f, 10000.0f + ScalePlus });
	CameraPos.y = 0.0f;

	GridDataInst.GridScale.w = 1 - (Y / 1000.0f);

	GetTransform()->SetLocalPosition(CameraPos);
}

void EngineGrid::Render(float _Delta)
{

}
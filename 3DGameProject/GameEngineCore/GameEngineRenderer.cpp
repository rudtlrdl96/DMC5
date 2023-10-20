#include "PrecompileHeader.h"
#include "GameEngineRenderer.h"
#include <GameEnginePlatform/GameEngineWindow.h>

#include "GameEngineLevel.h"
#include "GameEngineCamera.h"
#include "GameEngineMaterial.h"
#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"
#include "GameEngineShaderResHelper.h"
#include "GameEngineMesh.h"
#include "GameEngineInputLayOut.h"
#include "GameEngineComputeShader.h"

GameEngineRenderUnit::GameEngineRenderUnit()
{
	InputLayOutPtr = std::make_shared<GameEngineInputLayOut>();
}

void GameEngineRenderUnit::SetRenderer(GameEngineRenderer* _Renderer)
{
	ParentRenderer = _Renderer;
}

void GameEngineComputeUnit::Execute()
{
	ShaderResHelper.Setting();
	ComputeShader->Setting();
	GameEngineDevice::GetContext()->Dispatch(m_iGroupX, m_iGroupY, m_iGroupZ);
	ShaderResHelper.AllResourcesReset();
}

void GameEngineComputeUnit::SetComputeShader(const std::string_view& _Name)
{
	ComputeShader = GameEngineComputeShader::Find(_Name);

	if (nullptr == ComputeShader)
	{
		MsgAssert(std::string(_Name) + "존재하지 않는 컴퓨트 쉐이더를 세팅하려고 했습니다");
	}

	const GameEngineShaderResHelper& Res = ComputeShader->GetShaderResHelper();
	ShaderResHelper.Copy(Res);
}

void GameEngineRenderUnit::SetMesh(const std::string_view& _Name)
{
	Mesh = GameEngineMesh::Find(_Name);

	if (nullptr == Mesh)
	{
		MsgAssert("존재하지 않는 매쉬를 세팅하려고 했습니다.")
	}

	if (false == InputLayOutPtr->IsCreate() && nullptr != Material)
	{
		InputLayOutPtr->ResCreate(Mesh->GetVertexBuffer(), Material->GetVertexShader());
	}
}

void GameEngineRenderUnit::SetMesh(std::shared_ptr<GameEngineMesh> _Mesh)
{
	Mesh = _Mesh;

	if (false == InputLayOutPtr->IsCreate() && nullptr != Material)
	{
		InputLayOutPtr->ResCreate(Mesh->GetVertexBuffer(), Material->GetVertexShader());
	}
}

void GameEngineRenderUnit::SetMaterial(const std::string_view& _Name, RenderPath _Path /*= RenderPath::None*/, bool _IsRender /*= true*/)
{
	if (nullptr != GetRenderer() && nullptr != Material)
	{
		GetRenderer()->GetCamera()->PopRenderUnit(shared_from_this(), _Path);
	}

	ShaderResHelper.AllResourcesRelease();

	Material = GameEngineMaterial::Find(_Name);

	if (nullptr == Material)
	{
		MsgAssert("존재하지 않는 머티리얼을 사용했습니다.");
		return;
	}

	{
		const GameEngineShaderResHelper& Res = Material->GetVertexShader()->GetShaderResHelper();
		ShaderResHelper.Copy(Res);
	}

	{
		const GameEngineShaderResHelper& Res = Material->GetPixelShader()->GetShaderResHelper();
		ShaderResHelper.Copy(Res);
	}

	if (false == InputLayOutPtr->IsCreate() && nullptr != Mesh)
	{
		InputLayOutPtr->ResCreate(Mesh->GetVertexBuffer(), Material->GetVertexShader());
	}

	if (true == ShaderResHelper.IsConstantBuffer("TransformData"))
	{
		const TransformData& Data = ParentRenderer->GetTransform()->GetTransDataRef();
		ShaderResHelper.SetConstantBufferLink("TransformData", Data);
	}

	if (true == ShaderResHelper.IsConstantBuffer("RenderBaseValue"))
	{
		ShaderResHelper.SetConstantBufferLink("RenderBaseValue", ParentRenderer->BaseValue);
	}

	if (true == ShaderResHelper.IsConstantBuffer("LightDatas") && nullptr != ParentRenderer)
	{
		LightDatas& Data = ParentRenderer->GetActor()->GetLevel()->LightDataObject;
		ShaderResHelper.SetConstantBufferLink("LightDatas", Data);
	}

	if (true == _IsRender && nullptr != GetRenderer())
	{
		GetRenderer()->GetCamera()->PushRenderUnit(shared_from_this(), _Path);
	}
}

void GameEngineRenderUnit::Setting()
{
	if (nullptr == Mesh)
	{
		MsgAssert("매쉬가 존재하지 않는 유니트 입니다");
	}

	if (nullptr == Material)
	{
		MsgAssert("파이프라인이 존재하지 않는 유니트 입니다");
	}

	InputLayOutPtr->Setting();

	Mesh->Setting();
	Material->Setting();
	ShaderResHelper.Setting();
}

void GameEngineRenderUnit::ShadowSetting()
{
	if (nullptr == Mesh)
	{
		MsgAssert("매쉬가 존재하지 않는 유니트 입니다");
	}

	if (nullptr == Material)
	{
		MsgAssert("파이프라인이 존재하지 않는 유니트 입니다");
	}

	ShadowInputLayOutPtr->Setting();

	Mesh->Setting();
	Material->Setting();
	ShaderResHelper.Setting();
}

void GameEngineRenderUnit::ShadowOn()
{
	IsShadow = true;

	std::shared_ptr<GameEngineVertexShader> ShadowPtr = GameEngineVertexShader::Find("OrthogonalShadow.hlsl");
	if (nullptr == ShadowInputLayOutPtr)
	{
		ShadowInputLayOutPtr = std::make_shared<GameEngineInputLayOut>();
		ShadowInputLayOutPtr->ResCreate(Mesh->GetVertexBuffer(), ShadowPtr);
	}
}

void GameEngineRenderUnit::ShadowOff()
{
	IsShadow = false;
}

void GameEngineRenderUnit::Draw()
{
	UINT IndexCount = Mesh->IndexBufferPtr->GetIndexCount();
	GameEngineDevice::GetContext()->DrawIndexed(IndexCount, 0, 0);
}

void GameEngineRenderUnit::Draw(UINT _IndexCount, UINT _IndexStart)
{
	UINT IndexCount = Mesh->IndexBufferPtr->GetIndexCount();
	GameEngineDevice::GetContext()->DrawIndexed(_IndexCount, _IndexStart, 0);
}

void GameEngineRenderUnit::Render(float _DeltaTime)
{
	if (nullptr != RenderFunction)
	{
		RenderFunction(_DeltaTime);
		return;
	}

	Setting();
	Draw();
}

GameEngineRenderer::GameEngineRenderer()
{
	BaseValue.ScreenScale = GameEngineWindow::GetScreenSize();
	BaseValue.NoiseResolution = { 1024, 1024 };
	SetName("GameEngineRenderer");
}

GameEngineRenderer::~GameEngineRenderer()
{
}

void GameEngineRenderer::Start()
{
	PushCameraRender(0);
}

void GameEngineRenderer::RenderTransformUpdate(GameEngineCamera* _Camera)
{
	if (nullptr == _Camera)
	{
		assert(false);
		return;
	}

	GetTransform()->SetCameraMatrix(_Camera->GetView(), _Camera->GetProjection());
}

void GameEngineRenderer::RenderBaseValueUpdate(float _Delta)
{
	BaseValue.SumDeltaTime += _Delta;
	BaseValue.DeltaTime = _Delta;
}

void GameEngineRenderer::Render(float _Delta)
{
	RenderBaseValueUpdate(_Delta);
}

std::shared_ptr<GameEngineMaterial> GameEngineRenderer::GetMaterial(int _index/* = 0*/)
{
	if (Units.size() <= _index)
	{
		MsgAssert("존재하지 않는 랜더 유니트를 사용하려고 했습니다.");
	}

	return Units[_index]->Material;
}

void GameEngineRenderer::PushCameraRender(int _CameraOrder)
{
	GetLevel()->PushCameraRenderer(DynamicThis<GameEngineRenderer>(), _CameraOrder);
}

void GameEngineRenderer::CalSortZ(GameEngineCamera* _Camera)
{
	// View행렬로 해야 카메라가 원점에 오고 그 원점을 기준으로 카메라가 위치한곳의 z로 처리한다.

	switch (_Camera->ProjectionType)
	{
	case CameraType::Orthogonal:
	{
		// 직교 투영이기 때문에 카메라의 View행렬을 곱해서 원점을 기준으로 
		// 위치를 카메라의 원점을 기준으로한 위치로 바꾸고 그 z를 사용하면 확실한
		// 직교에서의 카메라와의 z거리가 된다.
		float4 View = GetTransform()->GetWorldPosition() * _Camera->View;
		CalZ = View.z;
		break;
	}
	case CameraType::Perspective:
	{
		float4 View = GetTransform()->GetWorldPosition() * _Camera->View;
		CalZ = View.Size();
		break;
	}
	default:
		break;
	}
}

std::shared_ptr<GameEngineRenderUnit> GameEngineRenderer::CreateRenderUnit(std::string_view _Mesh, std::string_view _Material)
{
	std::shared_ptr<GameEngineRenderUnit> Unit = CreateRenderUnit();

	Unit->SetMesh(_Mesh);
	Unit->SetMaterial(_Material);
	return Unit;
}

std::shared_ptr<GameEngineRenderUnit> GameEngineRenderer::CreateRenderUnit()
{
	std::shared_ptr<GameEngineRenderUnit> Unit = std::make_shared<GameEngineRenderUnit>();

	Unit->SetRenderer(this);
	Units.push_back(Unit);

	return Unit;
}

std::shared_ptr<GameEngineRenderUnit> GameEngineRenderer::CreateRenderUnitToIndex(unsigned int _Index)
{
	if (_Index >= Units.size())
	{
		Units.resize(_Index + 1);
	}

	std::shared_ptr<GameEngineRenderUnit> Unit = std::make_shared<GameEngineRenderUnit>();
	Unit->SetRenderer(this);
	Units[_Index] = Unit;

	return Unit;
}

void GameEngineRenderer::SetMaterial(const std::string_view& _Name, int _index /*= 0*/)
{
	std::shared_ptr<GameEngineRenderUnit> Unit = GetUnit(_index);

	if (nullptr == Unit)
	{
		Unit = CreateRenderUnitToIndex(_index);
	}

	Unit->SetMaterial(_Name);
}

void GameEngineRenderer::SetMesh(const std::string_view& _Name, int _index /*= 0*/)
{
	std::shared_ptr<GameEngineRenderUnit> Unit = GetUnit(_index);

	if (nullptr == Unit)
	{
		Unit = CreateRenderUnitToIndex(_index);
	}

	Unit->SetMesh(_Name);
}
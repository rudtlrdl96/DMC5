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

GameEngineRenderUnit::GameEngineRenderUnit()
{
	InputLayOutPtr = std::make_shared<GameEngineInputLayOut>();
}

void GameEngineRenderUnit::SetRenderer(GameEngineRenderer* _Renderer)
{
	ParentRenderer = _Renderer;
}

void GameEngineRenderUnit::SetMesh(const std::string_view& _Name)
{
	Mesh = GameEngineMesh::Find(_Name);

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

void GameEngineRenderUnit::SetMaterial(const std::string_view& _Name)
{
	// GetCamera()->Units[0];

	Material = GameEngineMaterial::Find(_Name);

	if (nullptr == Material)
	{
		MsgAssert("�������� �ʴ� ��Ƽ������ ����߽��ϴ�.");
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

	if (nullptr != ParentRenderer)
	{
		ParentRenderer->GetCamera()->PushRenderUnit(shared_from_this());
	}


	// ī�޶� ���� �ϴ¼���.


	if (true == ShaderResHelper.IsConstantBuffer("TransformData"))
	{
		const TransformData& Data = ParentRenderer->GetTransform()->GetTransDataRef();
		ShaderResHelper.SetConstantBufferLink("TransformData", Data);
	}

	if (true == ShaderResHelper.IsConstantBuffer("RenderBaseValue"))
	{
		ShaderResHelper.SetConstantBufferLink("RenderBaseValue", ParentRenderer->BaseValue);
	}

}

void GameEngineRenderUnit::Render(float _DeltaTime)
{
	if (nullptr == Mesh)
	{
		MsgAssert("�Ž��� �������� �ʴ� ����Ʈ �Դϴ�");
	}

	if (nullptr == Material)
	{
		MsgAssert("������������ �������� �ʴ� ����Ʈ �Դϴ�");
	}

	InputLayOutPtr->Setting();

	Mesh->Setting();
	Material->RenderingPipeLineSetting();
	ShaderResHelper.Setting();
	// Pipe->Render();

	UINT IndexCount = Mesh->IndexBufferPtr->GetIndexCount();
	GameEngineDevice::GetContext()->DrawIndexed(IndexCount, 0, 0);
}

GameEngineRenderer::GameEngineRenderer()
{
	BaseValue.ScreenScale = GameEngineWindow::GetScreenSize();
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

	// RenderCamera = _Camera;

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
	// GameEngineDevice::GetContext()->VSSetConstantBuffers();
	// GameEngineDevice::GetContext()->PSSetConstantBuffers();

	// ���� ����Ʈ�� 1���� �Ž� 1���� ��Ƽ������ ������ �ϴ� �뵵�Դϴ�.
	// 3D�����ԵǸ� �̰� �ȵǿ�.
	// ĳ���Ͱ� �Ž��� 1���� �ƴϾ�.
	// �ٸ� �� ����

	// �ؽ�ó ���� ������� ���� �̷��͵��� ���δ� ó�� �ȴ�.
	//for (size_t i = 0; i < Units.size(); i++)
	//{
	//	Units[i]->Render(_Delta);
	//}

}

std::shared_ptr<GameEngineMaterial> GameEngineRenderer::GetMaterial(int _index/* = 0*/)
{
	if (Units.size() <= _index)
	{
		MsgAssert("�������� �ʴ� ���� ����Ʈ�� ����Ϸ��� �߽��ϴ�.");
	}

	return Units[_index]->Material;
}

//// �̰� ����ϰԵǸ� �� �������� ����Ʈ�� �ڽŸ��� Ŭ�� ������������ ������ �ȴ�.
//std::shared_ptr<GameEngineMaterial> GameEngineRenderer::GetPipeLineClone(int _index/* = 0*/)
//{
//	if (Units.size() <= _index)
//	{
//		MsgAssert("�������� �ʴ� ���� ����Ʈ�� ����Ϸ��� �߽��ϴ�.");
//	}
//
//	if (false == Units[_index]->Pipe->IsClone())
//	{
//		Units[_index]->Pipe = Units[_index]->Pipe->Clone();
//	}
//
//	return Units[_index]->Pipe;
//}
//
//
//void GameEngineRenderer::SetMesh(const std::string_view& _Name, int _index /*= 0*/)
//{
//	if (Units.size() + 1 <= _index)
//	{
//		MsgAssert("�ʹ�ū ��������Ʈ Ȯ���� �Ϸ��� �߽��ϴ�");
//	}
//
//	if (Units.size() <= _index)
//	{
//		Units.resize(_index + 1);
//		Units[_index] = std::make_shared<GameEngineRenderUnit>();
//	}
//
//	std::shared_ptr<GameEngineRenderUnit> Unit = Units[_index];
//
//	if (nullptr == Unit)
//	{
//		MsgAssert("�������� �ʴ� ��������Ʈ�� ����Ϸ��� �߽��ϴ�.");
//	}
//
//
//	std::shared_ptr<GameEngineMesh> Mesh = GameEngineMesh::Find(_Name);
//
//	Unit->SetMesh(Mesh);
//}
//
//void GameEngineRenderer::SetPipeLine(const std::string_view& _Name, int _index)
//{
//	//if (0 >= Units.size())
//	//{
//	//	MsgAssert("���� ����Ʈ�� �������� �ʽ��ϴ�.");
//	//}
//
//	if (Units.size() + 1 <= _index)
//	{
//		MsgAssert("�ʹ�ū ��������Ʈ Ȯ���� �Ϸ��� �߽��ϴ�");
//	}
//
//	if (Units.size() <= _index)
//	{
//		Units.resize(_index + 1);
//		Units[_index] = std::make_shared<GameEngineRenderUnit>();
//	}
//
//	std::shared_ptr<GameEngineRenderUnit> Unit = Units[_index];
//
//	if (nullptr == Unit)
//	{
//		MsgAssert("�������� �ʴ� ��������Ʈ�� ����Ϸ��� �߽��ϴ�.");
//	}
//
//
//	Unit->SetPipeLine(_Name);
//
//	if (true == Unit->ShaderResHelper.IsConstantBuffer("TransformData"))
//	{
//		const TransformData& Data = GetTransform()->GetTransDataRef();
//		Unit->ShaderResHelper.SetConstantBufferLink("TransformData", Data);
//	}
//
//	if (true == Unit->ShaderResHelper.IsConstantBuffer("RenderBaseValue"))
//	{
//		Unit->ShaderResHelper.SetConstantBufferLink("RenderBaseValue", BaseValue);
//	}
//
//	GetTransform()->GetWorldMatrix();
//}

void GameEngineRenderer::PushCameraRender(int _CameraOrder)
{
	GetLevel()->PushCameraRenderer(DynamicThis<GameEngineRenderer>(), _CameraOrder);
}

void GameEngineRenderer::CalSortZ(GameEngineCamera* _Camera)
{
	// View��ķ� �ؾ� ī�޶� ������ ���� �� ������ �������� ī�޶� ��ġ�Ѱ��� z�� ó���Ѵ�.

	switch (_Camera->ProjectionType)
	{
	case CameraType::Orthogonal:
	{
		// ���� �����̱� ������ ī�޶��� View����� ���ؼ� ������ �������� 
		// ��ġ�� ī�޶��� ������ ���������� ��ġ�� �ٲٰ� �� z�� ����ϸ� Ȯ����
		// ���������� ī�޶���� z�Ÿ��� �ȴ�.
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


std::shared_ptr<GameEngineRenderUnit> GameEngineRenderer::CreateRenderUnit()
{
	std::shared_ptr<GameEngineRenderUnit> Unit = std::make_shared<GameEngineRenderUnit>();

	// Unit->shared_from_this();
	Unit->SetRenderer(this);
	Units.push_back(Unit);
	return Unit;
}


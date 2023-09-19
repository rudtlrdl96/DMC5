#include "PrecompileHeader.h"
#include "FieldMap.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineMaterial.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "FieldMapObject.h"


FieldMap::FieldMap()
{

}

FieldMap::~FieldMap()
{
	
}

std::shared_ptr<FieldMap> FieldMap::CreateFieldMap(GameEngineLevel* _Level, const std::vector<std::string>& _FBXNames, const std::vector<ObjTransformData>& _CullingCols, const std::vector<FieldMapObjData>& _FieldMapObjs)
{
	std::shared_ptr<FieldMap> Result;
	Result = _Level->CreateActor<FieldMap>();
	Result->SetName("FieldMap");
	std::vector<std::shared_ptr<GameEngineFBXRenderer>>& MapRenderersRef = Result->FieldMapRenderer;
	MapRenderersRef.resize(_FBXNames.size());
	for (size_t i = 0; i < MapRenderersRef.size(); i++)
	{
		MapRenderersRef[i] = Result->CreateComponent<GameEngineFBXRenderer>();
		if (GameEngineOption::GetOption("Shader") == GameEngineOptionValue::Low)
		{
			MapRenderersRef[i]->SetFBXMesh(_FBXNames[i].data(), "FBX_Low");
		}
		else
		{
			MapRenderersRef[i]->SetFBXMesh(_FBXNames[i].data(), "FBX");
		}

		MapRenderersRef[i]->SetStatic();
		MapRenderersRef[i]->Off();
	}

	std::vector<std::shared_ptr<GameEngineCollision>>& MapCullingColRef = Result->FieldMapCullingCol;
	MapCullingColRef.resize(_CullingCols.size());
	for (size_t i = 0; i < MapCullingColRef.size(); i++)
	{
		MapCullingColRef[i] = Result->CreateComponent<GameEngineCollision>(CollisionOrder::CullingCol);
		MapCullingColRef[i]->SetColType(ColType::OBBBOX3D);
		MapCullingColRef[i]->GetTransform()->SetLocalPosition(_CullingCols[i].Pos);
		MapCullingColRef[i]->GetTransform()->SetLocalScale(_CullingCols[i].Scale);
		MapCullingColRef[i]->GetTransform()->SetLocalRotation(_CullingCols[i].Rot);
	}

	std::vector<std::shared_ptr<GameEngineActor>>& FieldMapObjRef = Result->FieldMapObj;
	FieldMapObjRef.resize(_FieldMapObjs.size());
	for (size_t i = 0; i < FieldMapObjRef.size(); i++)
	{
		FieldMapObjRef[i] = FieldMapObject::CreateFieldMapObj(_Level, _FieldMapObjs[i].Type, _FieldMapObjs[i].ObjTransform);
		FieldMapObjRef[i]->Off();
	}

	return Result;
}

void FieldMap::PushNode(const std::vector<std::shared_ptr<FieldMap>>& _RenderOn, const std::vector<std::shared_ptr<FieldMap>>& _RenderOff)
{
	std::copy(_RenderOn.begin(), _RenderOn.end(), std::back_inserter(RenderOnNode));
	std::copy(_RenderOff.begin(), _RenderOff.end(), std::back_inserter(RenderOffNode));
}

void FieldMap::EraseFieldMap()
{
	ClearFieldMapRenderer();
	ClearFieldMapCullingCol();
	ClearRenderNode();
	ClearFieldMapObj();
}

void FieldMap::Update(float _DeltaTime)
{
	if (IsPlayerCollsionToCullingCol())
	{
		MapCulling();
	}
}

void FieldMap::ClearFieldMapRenderer()
{
	if (FieldMapRenderer.empty())
	{
		return;
	}

	for (size_t i = 0; i < FieldMapRenderer.size(); i++)
	{
		FieldMapRenderer[i]->Death();
		FieldMapRenderer[i] = nullptr;
	}
	FieldMapRenderer.clear();
}

bool FieldMap::IsPlayerCollsionToCullingCol()
{
	for (size_t i = 0; i < FieldMapCullingCol.size(); i++)
	{
		if (nullptr != FieldMapCullingCol[i]->Collision(CollisionOrder::Player, ColType::OBBBOX3D, ColType::OBBBOX3D))
		{
			return true;
		}
	}
	return false;
}

void FieldMap::ClearRenderNode()
{
	if (RenderOnNode.empty())
	{
		return;
	}
	RenderOnNode.clear();

	if (RenderOffNode.empty())
	{
		return;
	}
	RenderOffNode.clear();
}

void FieldMap::MapCulling()
{
	for (size_t i = 0; i < RenderOnNode.size(); i++)
	{
		RenderOnNode[i].lock()->FieldMapRendererOn();
		RenderOnNode[i].lock()->FieldMapObjOn();
	}

	for (size_t i = 0; i < RenderOffNode.size(); i++)
	{
		RenderOffNode[i].lock()->FieldMapRendererOff();
		RenderOffNode[i].lock()->FieldMapObjOff();
	}
}

void FieldMap::FieldMapObjOn()
{
	for (size_t i = 0; i < FieldMapObj.size(); i++)
	{
		FieldMapObj[i]->On();
	}
}

void FieldMap::FieldMapObjOff()
{
	for (size_t i = 0; i < FieldMapObj.size(); i++)
	{
		FieldMapObj[i]->Off();
	}
}

void FieldMap::FieldMapRendererOn()
{
	for (size_t i = 0; i < FieldMapRenderer.size(); i++)
	{
		FieldMapRenderer[i]->On();
	}
}

void FieldMap::FieldMapRendererOff()
{
	for (size_t i = 0; i < FieldMapRenderer.size(); i++)
	{
		FieldMapRenderer[i]->Off();
	}
}

void FieldMap::ClearFieldMapCullingCol()
{
	if (FieldMapCullingCol.empty())
	{
		return;
	}

	for (size_t i = 0; i < FieldMapCullingCol.size(); i++)
	{
		FieldMapCullingCol[i]->Death();
		FieldMapCullingCol[i] = nullptr;
	}
	FieldMapCullingCol.clear();
}

void FieldMap::ClearFieldMapObj()
{
	if (FieldMapObj.empty())
	{
		return;
	}

	for (size_t i = 0; i < FieldMapObj.size(); i++)
	{
		FieldMapObj[i]->Death();
		FieldMapObj[i] = nullptr;
	}
	FieldMapObj.clear();
}



#include <GameEngineCore/imgui.h>

void FieldMap::DrawEditor()
{
	ImGui::Spacing();
	ImGui::Spacing();

	float4 Color = FieldMapRenderer[0]->GetBaseColor();

	float InputColor[4] = { Color.x, Color.y, Color.z, Color.w };
	ImGui::DragFloat4("Color", InputColor, 0.01f, 0.0f, 1.0f);

	for (size_t i = 0; i < FieldMapRenderer.size(); i++)
	{
		FieldMapRenderer[i]->SetBaseColor(float4(InputColor[0], InputColor[1], InputColor[2], InputColor[3]));
	}
}



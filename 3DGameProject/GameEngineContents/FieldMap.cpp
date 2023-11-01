#include "PrecompileHeader.h"
#include "FieldMap.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineMaterial.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "FieldMapObject.h"
#include "ReflectionSetter.h"
#include "FieldMapObject.h"
#include "StageBaseLevel.h"
#include "BasePlayerActor.h"
#include "MotionBlurEffect.h"


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
	std::vector<std::weak_ptr<GameEngineFBXRenderer>>& MapRenderersRef = Result->FieldMapRenderer;
	MapRenderersRef.resize(_FBXNames.size());
	for (size_t i = 0; i < MapRenderersRef.size(); i++)
	{
		if (alphaCheck(_FBXNames[i]))
		{
			MapRenderersRef[i] = Result->CreateComponent<GameEngineFBXRenderer>();
			if (GameEngineOption::GetOption("Shader") == GameEngineOptionValue::Low)
			{
				MapRenderersRef[i].lock()->SetFBXMesh(_FBXNames[i].data(), "FBX_Alpha");
			}
			else
			{
				MapRenderersRef[i].lock()->SetFBXMesh(_FBXNames[i].data(), "FBX_Alpha");
			}
		}
		else
		{
			MapRenderersRef[i] = Result->CreateComponent<GameEngineFBXRenderer>();
			if (GameEngineOption::GetOption("Shader") == GameEngineOptionValue::Low)
			{
				MapRenderersRef[i].lock()->SetFBXMesh(_FBXNames[i].data(), "FBX_Low");
			}
			else
			{
				MapRenderersRef[i].lock()->SetFBXMesh(_FBXNames[i].data(), "FBX");
			}
		}

		MapRenderersRef[i].lock()->ShadowOn();
		MapRenderersRef[i].lock()->SetStatic();
		//MapRenderersRef[i]->Off();

		if (nullptr == GameEngineTexture::Find("WaterNoise.png"))
		{
			std::string Path = GameEnginePath::GetFileFullPath(
				"ContentResources",
				{ "Texture",  "WaterNoise"},
				"WaterNoise.png");

			GameEngineTexture::Load(Path);
		}

		if (waterCheck(_FBXNames[i]))
		{
			MapRenderersRef[i].lock()->SetBaseColor(float4(0, 1, 0));
			MapRenderersRef[i].lock()->SetTexture("WaterNoiseTexture", "WaterNoise.png");
		}
		// ����ҳ� �Ѿ���
		// MapRenderersRef[i].lock()->SetBaseColor(float4(0, 1, 0));
		MapRenderersRef[i].lock()->SetTexture("WaterNoiseTexture", "WaterNoise.png");
	}

	std::vector<std::weak_ptr<GameEngineCollision>>& MapCullingColRef = Result->FieldMapCullingCol;
	MapCullingColRef.resize(_CullingCols.size());
	for (size_t i = 0; i < MapCullingColRef.size(); i++)
	{
		MapCullingColRef[i] = Result->CreateComponent<GameEngineCollision>(CollisionOrder::CullingCol);
		MapCullingColRef[i].lock()->SetColType(ColType::OBBBOX3D);
		MapCullingColRef[i].lock()->GetTransform()->SetLocalPosition(_CullingCols[i].Pos);
		MapCullingColRef[i].lock()->GetTransform()->SetLocalScale(_CullingCols[i].Scale);
		MapCullingColRef[i].lock()->GetTransform()->SetLocalRotation(_CullingCols[i].Rot);
	}

	std::vector<std::weak_ptr<FieldMapObject>>& FieldMapObjRef = Result->FieldMapObj;
	std::list<std::weak_ptr<FieldMapObject>>& CullingObjRef = Result->CullingObj;
	FieldMapObjRef.resize(_FieldMapObjs.size());

	for (size_t i = 0; i < FieldMapObjRef.size(); i++)
	{
		FieldMapObjRef[i] = FieldMapObject::CreateFieldMapObj(_Level, _FieldMapObjs[i].Type, _FieldMapObjs[i].ObjTransform);
		
		if (!FieldMapObjRef[i].lock()->CullingExeption)
		{
			CullingObjRef.emplace_back(FieldMapObjRef[i]);
		}

		if (_FieldMapObjs[i].Type == FieldMapObjType::ReflectionSetter)
		{
			Result->Reflection = FieldMapObjRef[i].lock()->DynamicThis<ReflectionSetter>();
		}
	}

	return Result;
}

void FieldMap::PushNode(const std::vector<std::weak_ptr<FieldMap>>& _RenderOn, const std::vector<std::weak_ptr<FieldMap>>& _RenderOff)
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

void FieldMap::ReflectionSetting()
{
	if (GetLevel()->DynamicThis<StageBaseLevel>()->IsEditLevel)
	{
		return;
	}

	if (Reflection.lock() == nullptr)
	{
		//MsgAssert("Reflection Setter�� nullptr�Դϴ�");
		return;
	}

	static int n = 0;

	MotionBlurEffect::EffectOff();

	Reflection.lock()->Init("TestReflection" + std::to_string(n++), float4(512, 512));

	if (FieldMapRenderer.empty())
	{
		MsgAssert("�ݻ� �ؽ��� ������ �� �� �������� �����ϴ�");
		return;
	}
	
	for (size_t i = 0; i < FieldMapRenderer.size(); i++)
	{
		FieldMapRenderer[i].lock()->SetTexture("ReflectionTexture", Reflection.lock()->GetReflectionCubeTexture());
	}

	for (size_t i = 0; i < FieldMapObj.size(); i++)
	{
		if (FieldMapObj[i].lock()->GetFBXMesh() == nullptr)
		{
			continue;
		}
		FieldMapObj[i].lock()->GetFBXMesh()->SetTexture("ReflectionTexture", Reflection.lock()->GetReflectionCubeTexture());
	}


	MotionBlurEffect::EffectOn();
}

void FieldMap::Update(float _DeltaTime)
{
	if (GetLevel()->DynamicThis<StageBaseLevel>()->IsEditLevel)
	{
		return;
	}

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
		FieldMapRenderer[i].lock()->Death();
		FieldMapRenderer[i].lock() = nullptr;
	}
	FieldMapRenderer.clear();
}


bool FieldMap::IsPlayerCollsionToCullingCol()
{
	for (size_t i = 0; i < FieldMapCullingCol.size(); i++)
	{
		std::shared_ptr<BasePlayerActor> MyPlayer = GetLevel()->DynamicThis<StageBaseLevel>()->GetMyPlayer();
		if (MyPlayer == nullptr)
		{
			MsgAssert("MyPlayer�� nullptr�Դϴ� (CullingCol)");
		}

		const CollisionData& MyPlayerColData = MyPlayer->GetTransform()->GetCollisionData();
		const CollisionData& CullingColData = FieldMapCullingCol[i].lock()->GetTransform()->GetCollisionData();

		if (GameEngineTransform::AABBToOBB(MyPlayerColData, CullingColData))
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
		if (!RenderOnNode[i].expired())
		{
			RenderOnNode[i].lock()->FieldMapRendererOn();
			RenderOnNode[i].lock()->FieldMapObjOn();
		}
	}

	for (size_t i = 0; i < RenderOffNode.size(); i++)
	{
		if (!RenderOffNode[i].expired())
		{
			RenderOffNode[i].lock()->FieldMapRendererOff();
			RenderOffNode[i].lock()->FieldMapObjOff();
		}
	}
}

void FieldMap::FieldMapObjOn()
{
	for (auto& i : CullingObj)
	{
		if (!i.expired())
		{
			i.lock()->On();
		}
	}
}

void FieldMap::FieldMapObjOff()
{
	for (auto& i : CullingObj)
	{
		if (!i.expired())
		{
			i.lock()->Off();
		}
	}
}

void FieldMap::FieldMapRendererOn()
{
	for (size_t i = 0; i < FieldMapRenderer.size(); i++)
	{
		if (!FieldMapRenderer[i].expired())
		{
			FieldMapRenderer[i].lock()->On();
		}
	}
}

void FieldMap::FieldMapRendererOff()
{
	for (size_t i = 0; i < FieldMapRenderer.size(); i++)
	{
		if (!FieldMapRenderer[i].expired())
		{
			FieldMapRenderer[i].lock()->Off();
		}
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
		FieldMapCullingCol[i].lock()->Death();
		FieldMapCullingCol[i].lock() = nullptr;
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
		FieldMapObj[i].lock()->Death();
		FieldMapObj[i].lock() = nullptr;
	}
	FieldMapObj.clear();
}



#include <GameEngineCore/imgui.h>

void FieldMap::DrawEditor()
{
	ImGui::Spacing();
	ImGui::Spacing();

	float4 Color = FieldMapRenderer[0].lock()->GetBaseColor();

	float InputColor[4] = { Color.x, Color.y, Color.z, Color.w };
	ImGui::DragFloat4("Color", InputColor, 0.01f, 0.0f, 1.0f);

	for (size_t i = 0; i < FieldMapRenderer.size(); i++)
	{
		FieldMapRenderer[i].lock()->SetBaseColor(float4(InputColor[0], InputColor[1], InputColor[2], InputColor[3]));
	}
}




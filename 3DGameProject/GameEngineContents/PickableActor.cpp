#include "PrecompileHeader.h"
#include "PickableActor.h"

#include "Picker.h"

std::shared_ptr<GameEngineCollision> PickableActor::CurPicking_Collision;

PickableActor::PickableActor()
{

}

PickableActor::~PickableActor()
{
}

void PickableActor::Start()
{

}

void PickableActor::Update(float _DeltaTime)
{
	CheckPickingRay();

	CheckSelected();
}

// ActorPicker에 나 충돌했어요 알려줌
void PickableActor::CheckPickingRay()
{
	if (nullptr == Collision_Picking_)
	{
		MsgAssert("맵 에디터용 엑터에 피킹용 콜리전이 생성되지 않았습니다");
	}

	// 충돌했으면 PickedActors세트에 추가
	//bool Pickable = Collision_Picking_->IsCollision(ColType::CT_OBB, ColType::Ray, ColType::CT_OBB,
	//	[=](std::shared_ptr<GameEngineCollision> _This, std::shared_ptr<GameEngineCollision> _Other)
	//	{
	//		Picker::PickedActors.insert(std::dynamic_pointer_cast<PickableActor>(shared_from_this()));

	//		return CollisionReturn::ContinueCheck;
	//	});

	// 충돌안했으면 PickedActors세트 에서 제거
	//if (false == Pickable)
	//{
	//	Picker::PickedActors.erase(std::dynamic_pointer_cast<PickableActor>(shared_from_this()));
	//}
}

void PickableActor::CheckSelected()
{
	// 선택된 엑터는 Axis콜리전을 활성화하고 피킹 콜리전을 비활성화
	//if (shared_from_this() == std::dynamic_pointer_cast<GameEngineUpdateObject>(Picker::SelectedActor))
	//{
	//	Collision_Picking_->Off();
	//}
	//else
	//{
	//	Collision_Picking_->On();
	//}
}

void PickableActor::SetStaticMesh(const std::string& _FBX, const std::string& _Texture)
{
	//IsAxis_ = false;

	//FBXRenderer_ = CreateComponent<GameEngineFBXStaticRenderer>();
	//FBXRenderer_->SetFBXMesh(_FBX, "Texture");

	//// 메쉬 고유의 크기 받고 비율에 따라 월드크기
	//float4 FBXScale = FBXRenderer_->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	////float4 Ratio(SIZE_MAGNIFICATION_RATIO * 0.25f);

	//// 피킹용 콜리전
	//float4 CollisionScale = FBXScale * 0.25f;
	//Collision_Picking_ = CreateComponent<GameEngineCollision>();
	//Collision_Picking_->GetTransform().SetLocalScale(CollisionScale);
	//Collision_Picking_->SetDebugSetting(CollisionType::CT_OBB, float4(0.5f, 0.0f, 0.0f, 0.25f));
	//Collision_Picking_->ChangeOrder(CollisionGroup::Picking);

	// 렌더러
	//FBXRenderer_->GetTransform().SetLocalScale(FBXScale);

	// 피벗
	//FBXRenderer_->GetTransform().SetLocalMove(float4(CollisionScale.x * -0.5f, CollisionScale.y * -0.5f, 0.0f));
	//if (0 != _Texture.compare(""))
	//{
	//	//std::vector<std::vector<GameEngineRenderUnit>>& UnitSet = FBXRenderer_->GetAllRenderUnit();
	//	std::vector<std::vector< std::shared_ptr<GameEngineRenderUnit>>>& UnitSet = FBXRenderer_->GetAllRenderUnit();
	//	for (std::vector< std::shared_ptr<GameEngineRenderUnit>>& Units : UnitSet)
	//	{
	//		for (std::shared_ptr<GameEngineRenderUnit>& Unit : Units)
	//		{
	//			if (Unit->ShaderResources.IsTexture("DiffuseTexture"))
	//			{
	//				Unit->ShaderResources.SetTexture("DiffuseTexture", _Texture);
	//			}
	//		}
	//	}
	//}
}

void PickableActor::SetCollisionOnly(const float4& _Scale)
{
	// 처음 세팅해줄 때
	//if (nullptr == CollisionRenderer_)
	//{
	//	IsAxis_ = false;

	//	CollisionRenderer_ = CreateComponent<GameEngineTextureRenderer>();
	//	CollisionRenderer_->SetMesh("Box");
	//	CollisionRenderer_->SetMaterial("Color");
	//	ResultColor_ = float4(0.0f, 1.0f, 0.0f, 0.5f);
	//	CollisionRenderer_->GetRenderUnit()->ShaderResources.SetConstantBufferLink("ResultColor", ResultColor_);

	//	// 피킹용 콜리전
	//	Collision_Picking_ = CreateComponent<GameEngineCollision>();
	//	Collision_Picking_->GetTransform().SetLocalScale(_Scale);
	//	Collision_Picking_->SetDebugSetting(CollisionType::CT_OBB, float4(0.5f, 0.0f, 0.0f, 0.25f));

	//	Collision_Picking_->ChangeOrder(CollisionGroup::Picking);

	//	// 렌더러
	//	CollisionRenderer_->GetTransform().SetLocalScale(_Scale);
	//}
	//// 사이즈만 변경하고 싶을 때
	//else
	//{
	//	CollisionRenderer_->GetTransform().SetLocalScale(_Scale);
	//	Collision_Picking_->GetTransform().SetLocalScale(_Scale);
	//}

}

void PickableActor::SetAxisMove(float4 _Color, float4 _Scale, float4 _MoveDir)
{
	IsAxis_ = true;
	MoveDir_ = _MoveDir;

	//Collision_Picking_ = CreateComponent<GameEngineCollision>();
	//Collision_Picking_->GetTransform().SetLocalScale(_Scale);
	//Collision_Picking_->SetDebugSetting(CollisionType::CT_OBB, _Color);
	//Collision_Picking_->ChangeOrder(CollisionGroup::Picking);

}


void PickableActor::SetAxisRot(float4 _Color, float4 _Scale)
{
	IsAxis_ = true;
	//MoveDir_ = _MoveDir;

	//Collision_Picking_ = CreateComponent<GameEngineCollision>();
	//Collision_Picking_->GetTransform().SetLocalScale(_Scale);
	//Collision_Picking_->SetDebugSetting(CollisionType::CT_OBB, _Color);
	//Collision_Picking_->ChangeOrder(CollisionGroup::Picking);
}

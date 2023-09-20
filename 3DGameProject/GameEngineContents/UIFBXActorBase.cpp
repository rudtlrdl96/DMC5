#include "PrecompileHeader.h"
#include "UIFBXActorBase.h"
#include "UIFBXRenderer.h"
UIFBXActorBase::UIFBXActorBase() 
{
}

UIFBXActorBase::~UIFBXActorBase() 
{
}


void UIFBXActorBase::Start()
{
}

void UIFBXActorBase::Update(float _Delta)
{
}

std::shared_ptr<UIFBXRenderer> UIFBXActorBase::CreateGaugeBar(float4 _Pos,float4 _Scale, float4 _Rotation, const std::string_view& _FBXName)
{
	std::shared_ptr<UIFBXRenderer> Gaege = nullptr;
	Gaege = CreateComponent<UIFBXRenderer>(3);
	Gaege->SetFBXMesh(_FBXName.data(), "FBX_Low");
	Gaege->GetTransform()->SetLocalRotation(_Rotation);
	Gaege->GetTransform()->SetLocalScale(_Scale);
	Gaege->GetTransform()->SetLocalPosition(_Pos);
	return Gaege;
}

std::shared_ptr<UIFBXRenderer> UIFBXActorBase::CreateUIFBX(std::shared_ptr<class UIFBXRenderer> _Render, float4 _Pos, float4 _Scale, float4 _Rotation, const std::string_view& _FBXName , const std::string_view& _Meterial)
{
	_Render = CreateComponent<UIFBXRenderer>(3);
	_Render->SetFBXMesh(_FBXName.data(), _Meterial.data());
	_Render->GetTransform()->SetLocalRotation(_Rotation);
	_Render->GetTransform()->SetLocalScale(_Scale);
	_Render->GetTransform()->SetLocalPosition(_Pos);
	return _Render;
}

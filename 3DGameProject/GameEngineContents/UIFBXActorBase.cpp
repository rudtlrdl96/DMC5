#include "PrecompileHeader.h"
#include "UIFBXActorBase.h"
#include "UIFBXRenderer.h"
#include <GameEngineBase/GameEngineRandom.h>
#include "UIMeshRenderer.h"
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

std::shared_ptr<UIMeshRenderer> UIFBXActorBase::CreateGaugeBar(float4 _Pos,float4 _Scale, float4 _Rotation, const std::string_view& _FBXName, const std::string_view& _Meterial)
{
	std::shared_ptr<UIMeshRenderer> Gaege = nullptr;
	Gaege = CreateComponent<UIMeshRenderer>(3);
	Gaege->SetFBXMesh(_FBXName.data(), _Meterial.data());
	Gaege->GetTransform()->SetLocalRotation(_Rotation);
	Gaege->GetTransform()->SetLocalScale(_Scale);
	Gaege->GetTransform()->SetLocalPosition(_Pos);
	Gaege->LightDataValue.LightPos = Gaege->GetTransform()->GetWorldPosition();
	Gaege->LightDataValue.LightDir = { 0, 0, 1 };
	Gaege->LightDataValue.LightColor = { 1, 1, 1, 1 };
	Gaege->LightDataValue.LightPower = 1.0f;
	Gaege->LightDataUpdate();
	return Gaege;
}

std::shared_ptr<UIMeshRenderer> UIFBXActorBase::CreateUIFBX(std::shared_ptr<class UIMeshRenderer> _Render, float4 _Pos, float4 _Scale, float4 _Rotation, const std::string_view& _FBXName , const std::string_view& _Meterial)
{
	_Render = CreateComponent<UIMeshRenderer>(0);
	_Render->SetFBXMesh(_FBXName.data(), _Meterial.data());
	_Render->GetTransform()->SetLocalRotation(_Rotation);
	_Render->GetTransform()->SetLocalScale(_Scale);
	_Render->GetTransform()->SetLocalPosition(_Pos);
	_Render->LightDataValue.LightPos = _Render->GetTransform()->GetWorldPosition();
	_Render->LightDataValue.LightDir = { 0, 0, 1 };
	_Render->LightDataValue.LightColor = { 1, 1, 1, 1 };
	_Render->LightDataValue.LightPower = 1.0f;
	_Render->LightDataUpdate();
	return _Render;
}

std::shared_ptr<class UIFBXRenderer> UIFBXActorBase::CreateNoneLightFBX(std::shared_ptr<class UIFBXRenderer> _Render, float4 _Pos, float4 _Scale, float4 _Rotation, const std::string_view& _FBXName, const std::string_view& _Meterial)
{
	_Render = CreateComponent<UIFBXRenderer>(0);
	_Render->SetFBXMesh(_FBXName.data(), _Meterial.data());
	_Render->GetTransform()->SetLocalRotation(_Rotation);
	_Render->GetTransform()->SetLocalScale(_Scale);
	_Render->GetTransform()->SetLocalPosition(_Pos);
	return _Render;

}

void UIFBXActorBase::ShakeUIRender(std::shared_ptr<class UIMeshRenderer> _Render, float4 _ShakeArea, float _Delta)
{
	float4 RenderPos = _Render->GetTransform()->GetLocalPosition();
	float ElapsedTime = 0.0f;
	while (ElapsedTime < ShakeTime)
	{
		float X = GameEngineRandom::MainRandom.RandomFloat(RenderPos.x - _ShakeArea.x, RenderPos.x + _ShakeArea.x);
		float Y = GameEngineRandom::MainRandom.RandomFloat(RenderPos.y - _ShakeArea.y, RenderPos.y + _ShakeArea.y);
		_Render->GetTransform()->SetLocalPosition(float4::LerpClamp(RenderPos, float4(X, Y, RenderPos.z), _Delta * ShakeSpeed));
		ElapsedTime += _Delta;
	}
	_Render->GetTransform()->GetLocalPosition() = RenderPos;
}

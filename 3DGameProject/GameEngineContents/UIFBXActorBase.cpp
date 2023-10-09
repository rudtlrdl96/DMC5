#include "PrecompileHeader.h"
#include "UIFBXActorBase.h"
#include "UIFBXRenderer.h"
#include <GameEngineBase/GameEngineRandom.h>
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

std::shared_ptr<UIFBXRenderer> UIFBXActorBase::CreateGaugeBar(float4 _Pos,float4 _Scale, float4 _Rotation, const std::string_view& _FBXName, const std::string_view& _Meterial)
{
	std::shared_ptr<UIFBXRenderer> Gaege = nullptr;
	Gaege = CreateComponent<UIFBXRenderer>(3);
	Gaege->SetFBXMesh(_FBXName.data(), _Meterial.data());
	Gaege->GetTransform()->SetLocalRotation(_Rotation);
	Gaege->GetTransform()->SetLocalScale(_Scale);
	Gaege->GetTransform()->SetLocalPosition(_Pos);
	return Gaege;
}

std::shared_ptr<UIFBXRenderer> UIFBXActorBase::CreateUIFBX(std::shared_ptr<class UIFBXRenderer> _Render, float4 _Pos, float4 _Scale, float4 _Rotation, const std::string_view& _FBXName , const std::string_view& _Meterial)
{
	_Render = CreateComponent<UIFBXRenderer>(0);
	_Render->SetFBXMesh(_FBXName.data(), _Meterial.data());
	_Render->GetTransform()->SetLocalRotation(_Rotation);
	_Render->GetTransform()->SetLocalScale(_Scale);
	_Render->GetTransform()->SetLocalPosition(_Pos);
	return _Render;
}

void UIFBXActorBase::ShakeUIRender(std::shared_ptr<class UIFBXRenderer> _Render, float4 _ShakeArea, float _Delta)
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

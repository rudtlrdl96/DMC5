#include "PrecompileHeader.h"
#include "GaugeBaseActor.h"
#include "UIFBXRenderer.h"
GaugeBaseActor::GaugeBaseActor() 
{
}

GaugeBaseActor::~GaugeBaseActor() 
{
}


void GaugeBaseActor::Start()
{
}

void GaugeBaseActor::Update(float _Delta)
{
}

std::shared_ptr<UIFBXRenderer> GaugeBaseActor::CreateGaugeBar(float4 _Pos,float4 _Scale, float4 _Rotation, const std::string_view& _FBXName)
{
	std::shared_ptr<UIFBXRenderer> Gaege = nullptr;
	Gaege = CreateComponent<UIFBXRenderer>(3);
	Gaege->SetFBXMesh(_FBXName.data(), "FBX_Low");
	Gaege->GetTransform()->SetLocalRotation(_Rotation);
	Gaege->GetTransform()->SetLocalScale(_Scale);
	Gaege->GetTransform()->SetLocalPosition(_Pos);
	return Gaege;
}

#include "PrecompileHeader.h"
#include "StageBase.h"
#include "NetworkManager.h"

using namespace std;

StageBase::StageBase()
{
}

StageBase::~StageBase()
{
}

void StageBase::Start()
{
}

void StageBase::Update(float _DeltaTime)
{
	
}

void StageBase::StartProcess(GameEngineLevel* Level)
{
	NetworkManager::Update_PacketProcess(Level);
	/*string_view asd=Level->GetName();*/
}

void StageBase::SetCamera(float4 _Position)
{
	if (GetMainCamera() != nullptr)
	{
		GetMainCamera()->SetProjectionType(CameraType::Perspective);
	}
	GetMainCamera()->GetTransform()->SetLocalPosition(_Position);
}

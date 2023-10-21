#include "PrecompileHeader.h"
#include "BaseLevel.h"

#include "NetworkManager.h"

std::map<Net_LevelType, BaseLevel*> BaseLevel::AllNetLevel;


//BaseLevel* BaseLevel::GetNetLevel(Net_LevelType _Type)
//{
//	if (false == AllNetLevel.contains(_Type))
//	{
//		MsgAssert("�������� ���� ��Ʈ��ũ ����Ÿ������ ���������͸� �޾ƿ����� �߽��ϴ�");
//		return nullptr;
//	}
//
//	return AllNetLevel[_Type];
//}




BaseLevel::BaseLevel()
{

}

BaseLevel::~BaseLevel()
{

}

void BaseLevel::Start() 
{

}

void BaseLevel::SetNetLevelType(Net_LevelType _Type)
{
	if (Net_LevelType::UNKNOWN == _Type)
	{
		MsgAssert("��Ʈ��ũ ���� Ÿ���� UNKNOWN���� ������ ���� �����ϴ�");
		return;
	}

	NetLevelType = _Type;

	if (true == AllNetLevel.contains(_Type))
	{
		MsgAssert("�̹� ������ ��Ʈ��ũ ���� Ÿ���Դϴ�");
		return;
	}

	AllNetLevel[_Type] = this;
}

void BaseLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();

}


void BaseLevel::Update(float _DeltaTime)
{
	GameEngineLevel::Update(_DeltaTime);

	//���Ź��� ��Ŷ�� ó���ϴ� �κ��Դϴ�.
	//��Ʈ��ũ �Ŵ������� ��Ŷó��
	NetworkManager::Update_PacketProcess(this);
	//������ GameEngineNetObject���� ��Ŷó��
	GameEngineNetObject::Update_ProcessPackets();
}


void BaseLevel::NetworkUpdate(float _DeltaTime)
{
	GameEngineLevel::NetworkUpdate(_DeltaTime);

	//GameEngineNetObject�� Update_PushPacket�� ȣ��
	GameEngineNetObject::Update_SendPackets(_DeltaTime);
	GameEngineNetObject::ReleaseNetObject();

	NetTimer += _DeltaTime;
	if (NetTimer < NetworkManager::PacketFlushTime)
		return;

	NetworkManager::FlushPackets();
	NetTimer -= NetworkManager::PacketFlushTime;
}
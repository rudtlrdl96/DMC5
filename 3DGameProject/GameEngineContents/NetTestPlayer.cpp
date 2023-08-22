#include "PrecompileHeader.h"
#include "NetTestPlayer.h"

#include <GameEngineBase/GameEngineNet.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "NetworkManager.h"

#include "ObjectUpdatePacket.h"
#include "ServerWindow.h"
#include "ContentsEnum.h"

NetTestPlayer::NetTestPlayer()
{

}

NetTestPlayer::~NetTestPlayer()
{

}

/*
	�÷��̾��� ������
	�÷��̾ LevelChangeStart���� �����Ҷ�,
	Player->SetUserControll�� ȣ������ּ���
	(�׷��� UserControll��尡 �Ǿ ���������� ������ �� �ֽ��ϴ�)
*/

void NetTestPlayer::Start() 
{
	//������ ����� �� �� ���Ͱ� � �������� �����ϴ� �κ��Դϴ�.
	//ObjectUpdatePacket�� ����ϱ� ���ؼ��� �ݵ�� �� �κ��� ȣ�����ּž� �մϴ�
	SetNetObjectType(Net_ActorType::Nero);

	//�̰� �׳� ���� ������
	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.FBX", "NoneAlphaMesh");
}

void NetTestPlayer::Update_ProcessPacket() 
{
	//�÷��̾��� ��� ��Ʈ��ũ ��Ʈ�� �϶��� ��Ŷ�� ó���մϴ�
	if (NetControllType::NetControll != GameEngineNetObject::GetControllType())
		return;

	//��Ŷ�� �� ó���� �� ����
	while (GameEngineNetObject::IsPacket())
	{
		//���� ó���� ��Ŷ�� Ÿ���� �˾ƿɴϴ�
		PacketEnum Type = GameEngineNetObject::GetFirstPacketType<PacketEnum>();

		switch (Type)
		{
			//������Ʈ ��Ŷ�� ��쿣
		case PacketEnum::ObjectUpdatePacket:
		{
			//��Ŷ�� ���ø� �����ͷ� �����ɴϴ�(Enum���� �����Ͱ��� �°� ���ּž� �ϴ� �κ� ���Ǻ�Ź�����)
			std::shared_ptr<ObjectUpdatePacket> ObjectUpdate = PopFirstPacket<ObjectUpdatePacket>();

			//��Ŷ�� ������ ���� �ڽ��� �� ����
			GetTransform()->SetLocalPosition(ObjectUpdate->Position);
			GetTransform()->SetLocalRotation(ObjectUpdate->Rotation);
			ObjectUpdate->TimeScale;
			break;
		}
		default:
		{
			MsgAssert("ó������ ���ϴ� ��Ŷ�� �÷��̾�� ���ƿԽ��ϴ�.");
			return;
		}
		}
	}
}

void NetTestPlayer::Update(float _DeltaTime) 
{
	//�÷��̾��� ��� UserControll�϶��� FSM�� ���۵Ǿ�� �մϴ�.
	if (NetControllType::UserControll != GameEngineNetObject::GetControllType())
		return;


	//�Ϲ����� Update�κ��Դϴ�.
}

void NetTestPlayer::Update_SendPacket(float _DeltaTime) 
{
	//�÷��̾��� ��� UserControll�϶��� ��Ŷ�� �����մϴ�
	if (NetControllType::UserControll != GameEngineNetObject::GetControllType())
		return;

	//NetworkManager�� ���ؼ� ������Ʈ ��Ŷ�� ������ �˴ϴ�.
	//�� �� ��Ŷ�� �ٸ������� ������ ��������ϴ�.(�Ƹ���)
	NetworkManager::SendUpdatePacket(this, GetTransform(), 1.f);

	//��Ŷ�� ������ ��� NetworkManager�� ���ؼ� ���� �����Դϴ�.
	//���� �پ��� ��Ŷ ���� ����
}


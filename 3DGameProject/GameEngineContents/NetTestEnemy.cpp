#include "PrecompileHeader.h"
#include "NetTestEnemy.h"

#include <GameEngineBase/GameEngineNet.h>
#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "NetworkManager.h"

#include "ObjectUpdatePacket.h"
#include "ServerWindow.h"
#include "ContentsEnum.h"

NetTestEnemy::NetTestEnemy()
{

}

NetTestEnemy::~NetTestEnemy()
{

}


void NetTestEnemy::Start()
{
	//������ ����� �� �� ���Ͱ� � �������� �����ϴ� �κ��Դϴ�.
	//ObjectUpdatePacket�� ����ϱ� ���ؼ��� �ݵ�� �� �κ��� ȣ�����ּž� �մϴ�
	SetNetObjectType(Net_ActorType::Nero);

	//�̰� �׳� ���� ������
	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.FBX", "NoneAlphaMesh");
}

void NetTestEnemy::Update_ProcessPacket()
{
	//������ ��� Ŭ���̾�Ʈ�϶��� ��Ŷ�� ó���Ѵٴ� �ǹ�
	if (false == NetworkManager::IsClient())
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

void NetTestEnemy::Update(float _DeltaTime)
{
	//������ ��� ������ FSM�� ���۵Ǿ�� �մϴ�.
	if (false == NetworkManager::IsServer())
		return;


	//�Ϲ����� Update�κ��Դϴ�.
}

void NetTestEnemy::Update_SendPacket(float _DeltaTime)
{
	//������ ��� �����϶��� ��Ŷ�� �����ϴ�.
	if (false == NetworkManager::IsServer())
		return;

	//NetworkManager�� ���ؼ� ������Ʈ ��Ŷ�� ������ �˴ϴ�.
	//�� �� ��Ŷ�� �ٸ������� ������ ��������ϴ�.(�Ƹ���)
	NetworkManager::SendUpdatePacket(this, GetTransform(), 1.f);

	//��Ŷ�� ������ ��� NetworkManager�� ���ؼ� ���� �����Դϴ�.
	//���� �پ��� ��Ŷ ���� ����
}

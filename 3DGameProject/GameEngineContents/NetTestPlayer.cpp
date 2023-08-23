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

	if (false == GameEngineInput::IsKey("NetTestLeft"))
	{
		GameEngineInput::CreateKey("NetTestLeft", 'A');
		GameEngineInput::CreateKey("NetTestRight", 'D');
		GameEngineInput::CreateKey("NetTestForward", 'W');
		GameEngineInput::CreateKey("NetTestBack", 'S');
		GameEngineInput::CreateKey("NetTestConnect", 'H');
	}
}

void NetTestPlayer::Update_ProcessPacket() 
{
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
	float4 MoveDir = float4::ZERO;
	if (GameEngineInput::IsPress("NetTestLeft"))
	{
		MoveDir += float4::LEFT;
	}
	if (GameEngineInput::IsPress("NetTestRight"))
	{
		MoveDir += float4::RIGHT;
	}
	if (GameEngineInput::IsPress("NetTestForward"))
	{
		MoveDir += float4::FORWARD;
	}
	if (GameEngineInput::IsPress("NetTestBack"))
	{
		MoveDir += float4::BACK;
	}
	
	GetTransform()->AddLocalPosition(MoveDir * 300.f * _DeltaTime);

	if (GameEngineInput::IsDown("NetTestConnect"))
	{
		if (false == NetworkManager::IsNet())
			return;

		if (-1 != GetNetObjectID())
			return;

		NetworkManager::LinkNetwork(this);
	}
}

void NetTestPlayer::Update_SendPacket(float _DeltaTime) 
{
	//NetworkManager�� ���ؼ� ������Ʈ ��Ŷ�� ������ �˴ϴ�.
	//�� �� ��Ŷ�� �ٸ������� ������ ��������ϴ�.(�Ƹ���)
	NetworkManager::SendUpdatePacket(this, GetTransform(), 1.f);

	//��Ŷ�� ������ ��� NetworkManager�� ���ؼ� ���� �����Դϴ�.
	//���� �پ��� ��Ŷ ���� ����
}


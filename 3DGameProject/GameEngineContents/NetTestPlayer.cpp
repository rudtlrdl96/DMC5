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


void NetTestPlayer::Start() 
{
	//������ ����� �� �� ���Ͱ� � �������� �����ϴ� �κ��Դϴ�.
	//ObjectUpdatePacket�� ����ϱ� ���ؼ��� �ݵ�� �� �κ��� ȣ�����ּž� �մϴ�
	SetNetObjectType(Net_ActorType::TestPlayer);

	//�̰� �׳� ���� ������
	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.FBX", "FBX");

	if (false == GameEngineInput::IsKey("NetTestLeft"))
	{
		GameEngineInput::CreateKey("NetTestLeft", 'A');
		GameEngineInput::CreateKey("NetTestRight", 'D');
		
		GameEngineInput::CreateKey("NetTestUp", 'W');
		GameEngineInput::CreateKey("NetTestDown", 'S');

		GameEngineInput::CreateKey("NetTestForward", 'E');
		GameEngineInput::CreateKey("NetTestBack", 'Q');

		GameEngineInput::CreateKey("NetTestDestroy", 'J');
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
	if (NetControllType::ActiveControll != GameEngineNetObject::GetControllType())
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
	if (GameEngineInput::IsPress("NetTestUp"))
	{
		MoveDir += float4::UP;
	}
	if (GameEngineInput::IsPress("NetTestDown"))
	{
		MoveDir += float4::DOWN;
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

	
	if (GameEngineInput::IsDown("NetTestDestroy"))
	{
		int ID = GetNetObjectID();
		Death();
	}
}

void NetTestPlayer::Update_SendPacket(float _DeltaTime) 
{
	int ID = GetNetObjectID();

	//NetworkManager�� ���ؼ� ������Ʈ ��Ŷ�� ������ �˴ϴ�.
	//�� �� ��Ŷ�� �ٸ������� ������ ��������ϴ�.(�Ƹ���)
	NetworkManager::PushUpdatePacket(UpdatePacketParameter
		{
			.ObjPtr = this,
			.TimeScale = 1.f
		});

	//��Ŷ�� ������ ��� NetworkManager�� ���ؼ� ���� �����Դϴ�.
	//���� �پ��� ��Ŷ ���� ����
}

#include <GameEngineCore/imgui.h>

void NetTestPlayer::DrawEditor()
{
	ImGui::Text(GameEngineString::UniCodeToAnsi(L"Test Code").data());
}